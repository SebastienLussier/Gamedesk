/**
*  @file       Package.h
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
#ifndef     _PACKAGE_H_
#define     _PACKAGE_H_


namespace Gamedesk {


class Package;


class CORE_API PackageManager
{
public:
    /**
    *  Destructor.
    *  @brief  Destructor.
    */
    virtual ~PackageManager();

    /**
    *  Access the PackageManager singleton.
    *  @brief  Singleton access method.
    *  @return the package manager singleton.
    */
    static PackageManager* Instance()
    {
        return &mInstance;
    }

    Package* CreatePackage( const String& pPackageName );
    Package* GetPackage( const String& pPackageName );
    
private:
    Map<String,Package*>    mPackages;  //!< Map containing loaded packages.
    static PackageManager   mInstance;  //!< Singleton instance.
};


namespace PackageHelper
{
    class InternalInputStream;
    class InternalOutputStream;
}

class CORE_API Package : public Object
{
    friend class PackageManager;
    
    friend class PackageHelper::InternalInputStream;
    friend class PackageHelper::InternalOutputStream;

    DECLARE_CLASS(Package,Object)

public:
    virtual ~Package();

    void Load();
    void Save();

protected:
    Package();

    void Reset();

    class Header
    {
    public:
        UInt32                  mTag;
        UInt32                  mVersion;
        String                  mName;
        UInt32                  mDependencyCount;
        UInt32                  mInternalObjectCount;
        UInt32                  mExternalObjectCount;
    };

    class InternalObject
    {
    public:
        Object*     mObject;        //!< Actual object (only valid once resolved/created).
        String      mObjectName;    //!< Name of the object.
        String      mParentName;    //!< Name of parent object.
        String      mClassName;     //!< Name of the class.
        UInt32      mSize;          //!< Size of this object data in the package.

        InternalObject() 
            : mObject(NULL)
            , mObjectName()
            , mClassName()
            , mSize(0)
        {
        }

        friend Stream& operator << ( Stream& pStream, InternalObject& pInternalObject )
        {
            pStream << pInternalObject.mObjectName;
            pStream << pInternalObject.mClassName;
            pStream << pInternalObject.mSize;

            return pStream;
        }
    };

    class ExternalObject
    {
    public:
        Object*     mObject;        //!< Actual object (only valid once resolved).
        String      mObjectName;    //!< Name of the object.
        String      mPackageName;   //!< Name of package.

        ExternalObject() 
            : mObject(NULL)
            , mObjectName()
            , mPackageName()
        {
        }

        friend Stream& operator << ( Stream& pStream, ExternalObject& pExternalObject )
        {
            pStream << pExternalObject.mObjectName;
            pStream << pExternalObject.mPackageName;

            return pStream;
        }
    };

    void    SerializeHeader( Stream& pStream );
    Object* GetObjectFromIndex( Int32 pIndex );


protected:
    Header                  mHeader;
    Vector<String>          mDependencies;
    Vector<InternalObject>  mInternalObjects;
    Vector<ExternalObject>  mExternalObjects;

    Map<Object*, Int32>     mObjectIndices; // Save only
    

    Bool mMustBeSaved;
};


} // namespace Gamedesk


#endif  //  _PACKAGE_H_
