/**
 *  @file       Class.cpp
 *  @brief      Class description associated to each classes (reflective factory).
 *  @author     Sébastien Lussier.
 *  @date       19/01/02.
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
#include "Class.h"
#include "ObjectIterator.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Class);


Class::Class()
{
    mName           = "None";   // Name of class
    mSuperClass     = NULL;     // Super class Class object
    mClassSize      = 0;        // class size in bytes
    mFuncClassNew   = NULL;     // Function pointer to the constructor
    mAbstract       = true;
}


Class::Class( const Char* pStrName, Class* pClsSuper, UInt32 pClassSize, Object* (*pFuncClassNew)(), void (*pFuncRegisterProperties)(Class*), Bool pAbstract )
{
    mName           = pStrName;         // Name of class
    mSuperClass     = pClsSuper;        // Super class Class object
    mClassSize      = pClassSize;       // class size in bytes
    mFuncClassNew   = pFuncClassNew;    // Function pointer to the constructor
    mAbstract       = pAbstract;        // Class is abstract or not ?

    // Register properties...
    if( pFuncRegisterProperties )
        pFuncRegisterProperties( this );
}

// Function used to find if a class is derived from another one
Bool Class::IsDerivedFrom( Class* pClass ) const
{
    if( !pClass )
        return false;

    // If this class is the class we're looking for, then it's not
    // derived from itself.
    if( this == pClass )
        return false;

    for( const Class* classObject = GetSuper(); classObject; classObject = classObject->GetSuper() )
    {
        if( classObject == pClass )
            return true;
    }

    return false;
}


Class* Class::GetClassByName( const Char* pStrName )
{
    guard(Class::GetClassByName);

    for( ObjectIterator<Class> itClass; itClass.IsValid(); ++itClass )
    {
        if( strcmp((*itClass)->GetName().c_str(), pStrName) == 0 )
        {
            return *itClass;
        }
    }

    throw ClassNotFoundException( pStrName, Here );

    unguard;
}


} // namespace Gamedesk
