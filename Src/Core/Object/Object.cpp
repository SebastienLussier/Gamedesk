/**
 *  @file       Object.cpp
 *  @brief      Abstract base class for the object hierarchy.
 *  @author     Sébastien Lussier.
 *  @date       29/09/02.
 */
/*
 *  Copyright (C) 2004 Gamedesk
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
#include "Object.h"
#include "Package/Package.h"


namespace Gamedesk {
	
	
IMPLEMENT_OBJECT_CLASS(Object);


Object*  Object::mFirstObject   = NULL;
Object*  Object::mLastObject    = NULL;


Object::Object() :
    mOwner(NULL),
    mFlags(0),
    mNextObject( NULL ),
    mPrevObject( NULL )
{
    Link();
}

void Object::Init()
{

}

void Object::Kill()
{
}

Object::~Object()
{
    UnLink();
}

Bool Object::IsA( Class* pClass ) const
{
    return GetClass() == pClass || GetClass()->IsDerivedFrom(pClass);
}

void Object::Link()
{
    GD_ASSERT(mNextObject==NULL);
    GD_ASSERT(mPrevObject==NULL);

    if( mLastObject )
    {
        mPrevObject = mLastObject;
        mLastObject->mNextObject = this;
        mLastObject = this;
        mNextObject = NULL;
    }
    else
    {
        mLastObject = mFirstObject = this;
        mPrevObject = mNextObject  = NULL;
    }
}

void Object::UnLink()
{
    if( mLastObject == this )
        mLastObject = mPrevObject;

    if( mFirstObject == this )
        mFirstObject = mNextObject;

    if( mPrevObject )
        mPrevObject->mNextObject = mNextObject;

    if( mNextObject )
        mNextObject->mPrevObject = mPrevObject;
}

void Object::SetOwner( Object* pOwner )
{
    mOwner = pOwner;
}

Object* Object::GetOwner() const
{
    return mOwner;
}

Bool Object::IsOwnedBy( Object* pOwner )
{
    GD_ASSERT_M( pOwner != NULL, "This test is useless!" );

    if( pOwner == this )
        return true;

    Object* owner = GetOwner();
    while( owner )
    {
        if( owner == pOwner )
            return true;

        owner = owner->GetOwner();
    }

    return false;
}

void Object::SetName( const String& pName )
{
    if( pName != "" )
    {
        mName = pName;
    }
    else
    {
        UInt32 suffix = 0;
        Bool   found = false;
        String fullName;

        while( !found )
        {
            if( suffix < 10 )
                fullName = GetClass()->GetName() + String("_00") + ToString(suffix);
            else if( suffix < 100 )
                fullName = GetClass()->GetName() + String("_0") + ToString(suffix);

            if( FindObject( fullName, "" ) == NULL )
                found = true;

            suffix++;
        }
        
        mName = fullName;
    }
}

const String& Object::GetName() const
{
    return mName;
}

Package* Object::GetPackage()
{
    Object* owner = this;
    while( owner )
    {
        if( owner->mOwner == NULL )
            return owner->IsA(Package::StaticClass()) ? Cast<Package>(owner) : NULL;

        owner = owner->mOwner;
    }

    return NULL;
}

Object* Object::FindObject( const String& pName, const String& pOwnerName )
{
    // ARGGGHHH!
    if( pOwnerName == "" )
    {
        // ARGGGHHH!
        for( ObjectIterator<Object> it; it; ++it )
        {
            if( (*it)->GetName() == pName )
            {
                return *it;
            }
        }
    }
    else
    {
        // ARGGGHHH!
        for( ObjectIterator<Object> it; it; ++it )
        {
            if( (*it)->GetName() == pName && (*it)->GetOwner() && (*it)->GetOwner()->GetName() == pOwnerName )
            {
                return *it;
            }
        }
    }

    return NULL;
}


} // namespace Gamedesk
