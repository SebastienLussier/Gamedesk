/**
*  @file       Package.cpp
*  @brief      Collection of Gamedesk objects stored in a file.
*  @author     Sébastien Lussier.
*  @date       21/03/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
*
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  Gamedesk
*  http://gamedesk.type-cast.com
*
*/
#include "Core.h"
#include "Package.h"

#include "FileManager/FileManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Package)


// Singleton Instance
PackageManager PackageManager::mInstance;

PackageManager::~PackageManager()
{
}

Package* PackageManager::CreatePackage( const String& pPackageName )
{
    // Check if package already exist
    Package* newPackage = GetPackage( pPackageName );
    if( newPackage != NULL )
        return newPackage;

    newPackage = GD_NEW(Package, this, "Core::Package::PackageManager");
    newPackage->SetName( pPackageName );

    return newPackage;
}

Package* PackageManager::GetPackage( const String& pPackageName )
{
    Map<String,Package*>::const_iterator itFind = mPackages.find(pPackageName);
    
    if( itFind != mPackages.end() )    
        return itFind->second;

    return NULL;
}

Package::Package()
    : mMustBeSaved(false)
{
}


Package::~Package()
{
}


namespace PackageHelper
{

class PreprocessOuputStream : public OutputStream
{
    CLASS_DISABLE_COPY(PreprocessOuputStream);

public:
    PreprocessOuputStream()
        : mPos(0)
        , mNbExternalObjects(0)
    {
    }

    Stream& operator << ( Object*& pObj )
    {
        // Check if an object is referenced, but is not part of this package.
        if( pObj && !pObj->HasFlags(Object::OBJ_Internal) && !pObj->HasFlags(Object::OBJ_External) )
        {
            pObj->SetFlags(Object::OBJ_External);
            mNbExternalObjects++;
        }

        // For each reference to an object, an index is serialized.
        Serialize( NULL, sizeof(Int32) );

        if( pObj && pObj->GetOwner() )
        {
            Object* owner = pObj->GetOwner();
            (*this) << owner;
        }

        return *this;
    }

    void Serialize( void* /*pData*/, UInt32 pLen )
    {
        mPos += pLen;
    }

    UInt32 GetPos() const
    {
        return mPos;
    }

    UInt32 GetNbExternalObjects() const
    {
        return mNbExternalObjects;
    }

private:
    UInt32  mPos;
    UInt32  mNbExternalObjects;
};


class InternalOutputStream : public OutputStream
{
    CLASS_DISABLE_COPY(InternalOutputStream);

public:
    InternalOutputStream( Stream& pStream, Package* pPackage )
        : mStream(pStream)
        , mPackage(pPackage)
    {
    }
    
    void Serialize( void* pData, UInt32 pLen )
    {
        // Forward to the actual stream
        mStream.Serialize( pData, pLen );
    }

    Stream& operator << ( class Object*& pObj ) 
    {
        // Save entry in package mObjectIndices
        mStream << mPackage->mObjectIndices[pObj];

        if( pObj != NULL )
        {
            Object* owner = pObj->GetOwner();
            (*this) << owner;            
        }

        return *this;
    }

private:
    Stream&   mStream;
    Package*  mPackage;
};


// Serialize data and fixup pointers using the package mInt
class InternalInputStream : public InputStream
{
    CLASS_DISABLE_COPY(InternalInputStream);

public:
    InternalInputStream( Stream& pStream, Package* pPackage )
        : mStream(pStream)
        , mPackage(pPackage)
    {
    }

    void Serialize( void* pData, UInt32 pLen )
    {
        // Forward to the actual stream
        mStream.Serialize( pData, pLen );
    }

    Stream& operator << ( class Object*& pObj ) 
    {
        // Load index and map it to actual pointer
        Int32 entry;
        mStream << entry;
        pObj = mPackage->GetObjectFromIndex(entry);

        if( pObj != NULL )
        {
            Object* owner;
            (*this) << owner;
            pObj->SetOwner(owner);
        }

        return *this;
    }

private:
    Stream&         mStream;
    Package*        mPackage;
};

}

void Package::Reset()
{
    mHeader.mTag                    = ('G') + ('D'<<8) + ('P'<<16) + ('K'<<24);
    mHeader.mVersion                = 0;
    mHeader.mName                   = GetName();
    mHeader.mDependencyCount        = 0;
    mHeader.mInternalObjectCount    = 0;
    mHeader.mExternalObjectCount    = 0;

    mDependencies.clear();
    mInternalObjects.clear();
    mExternalObjects.clear();
    mObjectIndices.clear();

    mObjectIndices[NULL] = 0;

    mMustBeSaved = false;
}

Object* Package::GetObjectFromIndex( Int32 pIndex )
{    
    if( pIndex > 0 )
        return mInternalObjects[pIndex - 1].mObject;
    else if( pIndex < 0 )
        return mExternalObjects[-pIndex - 1].mObject;

    return NULL;
}

void Package::Save()
{
    Reset();

    mHeader.mName = GetName();

    // We must build the tables.

    // Iterate through all the objects, add objects owned by this package to the list.
    // Flag them as OBJ_Internal
    for( ObjectIterator<Object> itObj; itObj; ++itObj )
    {
        if( (*itObj)->IsOwnedBy(this) )
        {
            (*itObj)->SetFlags( Object::OBJ_Internal );
            mHeader.mInternalObjectCount++;
        }
    }

    // Create the internal objects table.
    mInternalObjects.resize( mHeader.mInternalObjectCount );

    // Next, iterate through all the objects.
    // Serialize the objects flagged as internal to a special stream.
    // This stream will look for references to objects not in this package.
    // Flag them as OBJ_External.
    // This stream will also count the number of bytes outputted.
    PackageHelper::PreprocessOuputStream packageHelperStream;
    Vector<InternalObject>::iterator itInternalObj = mInternalObjects.begin();

    UInt32 previousPos = 0;
    UInt32 currentPos  = 0;
    Int32  index       = 1; // Index 0 is reserved for NULL objects.
    for( ObjectIterator<Object> itObj; itObj; ++itObj )
    {
        if( (*itObj)->HasFlags( Object::OBJ_Internal ) )
        {
            mObjectIndices[*itObj]       = index++;

            (*itInternalObj).mObject     = (*itObj);
            (*itInternalObj).mObjectName = (*itObj)->GetName();
            (*itInternalObj).mClassName  = (*itObj)->SerializeAs()->GetName();
            (*itInternalObj).mParentName = (*itObj)->GetOwner() ? (*itObj)->GetOwner()->GetName() : "";

            // Compute object size...
            previousPos = currentPos;
            (*itObj)->Serialize( packageHelperStream );
            currentPos = packageHelperStream.GetPos();
            (*itInternalObj).mSize = currentPos - previousPos;

            ++itInternalObj;
        }
    }

    // Create the external objects table.
    index = -1;  // Index 0 is reserved for NULL!
    mHeader.mExternalObjectCount = packageHelperStream.GetNbExternalObjects();
    mExternalObjects.resize( mHeader.mExternalObjectCount );
    Vector<ExternalObject>::iterator itExternalObj = mExternalObjects.begin();
    for( ObjectIterator<Object> itObj; itObj; ++itObj )
    {
        if( (*itObj)->HasFlags( Object::OBJ_External ) )
        {
            mObjectIndices[*itObj]        = index--;

            (*itExternalObj).mObject      = (*itObj);
            (*itExternalObj).mObjectName  = (*itObj)->GetName();

            Package* pPackage = (*itObj)->GetPackage();
            if( pPackage != NULL )
                mDependencies.push_back( pPackage->GetName() );

            (*itExternalObj).mPackageName = pPackage ? pPackage->GetName() : "None";
            ++itExternalObj;
        }
    }

    mHeader.mDependencyCount = mDependencies.size();

    Stream* stream = FileManager::CreateOutputStream( GetName() );

    SerializeHeader( *stream );

    // Save all internal objects
    PackageHelper::InternalOutputStream internalStream(*stream, this);
    for( Vector<InternalObject>::iterator it = mInternalObjects.begin(); it != mInternalObjects.end(); ++it )
        (*it).mObject->Serialize( internalStream );

    GD_DELETE(stream);
}

void Package::Load()
{
    Reset();

    Stream* stream = FileManager::CreateInputStream( GetName() );

    SerializeHeader( *stream );
    SetName( mHeader.mName );

    // First, make sure all dependencies are loaded.
    for( Vector<String>::iterator itDep = mDependencies.begin(); itDep != mDependencies.end(); ++itDep )
    {
        Package* package = PackageManager::Instance()->GetPackage( *itDep );
        GD_ASSERT_M( package != NULL, "Package dependency could not be resolved; package not found" );
        package->Load();
    }

    // Resolve internal objects.
    Vector<InternalObject>::iterator itInternalObj = mInternalObjects.begin();
    Vector<InternalObject>::iterator itInternalEnd = mInternalObjects.end();
    for( ; itInternalObj != itInternalEnd; ++itInternalObj )
    {
        // Check if already resolved (possible by a call to ResolveObject())...
        if( (*itInternalObj).mObject != NULL )
            continue;

        (*itInternalObj).mObject = Object::FindObject( (*itInternalObj).mObjectName, (*itInternalObj).mParentName );
        if( (*itInternalObj).mObject == NULL )
        {
            Class* objClass = Class::GetClassByName( (*itInternalObj).mClassName.c_str() );
            GD_ASSERT_M( objClass, "Class not found!" );

            // Create it
            (*itInternalObj).mObject = Class::GetClassByName( (*itInternalObj).mClassName.c_str() )->AllocateNew( (*itInternalObj).mObjectName );
            (*itInternalObj).mObject->SetOwner( this );
        }
    }

    // Resolve external objects.
    Vector<ExternalObject>::iterator itExternalObj = mExternalObjects.begin();
    Vector<ExternalObject>::iterator itExternalEnd = mExternalObjects.end();
    for( ; itExternalObj != itExternalEnd; ++itExternalObj )
    {
        (*itExternalObj).mObject = Object::FindObject( (*itExternalObj).mObjectName, (*itExternalObj).mPackageName );
        if( (*itExternalObj).mObject == NULL )
        {
            GD_ASSERT_M( (*itExternalObj).mObject != NULL, "Object not found in package!" );
        }
    }

    // Load all internal objects
    PackageHelper::InternalInputStream internalStream(*stream, this);
    for( Vector<InternalObject>::iterator it = mInternalObjects.begin(); it != mInternalObjects.end(); ++it )
        (*it).mObject->Serialize( internalStream );

    GD_DELETE(stream);
}

void Package::SerializeHeader( Stream& pStream )
{
    // Serialize the header
    pStream << mHeader.mTag;
    pStream << mHeader.mVersion;
    pStream << mHeader.mName;
    pStream << mHeader.mDependencyCount;
    pStream << mHeader.mInternalObjectCount;
    pStream << mHeader.mExternalObjectCount;

    // Serialize dependencies list.
    pStream << mDependencies;

    // Serialize objects tables.
    pStream << mInternalObjects;
    pStream << mExternalObjects;
}


} // namespace Gamedesk
