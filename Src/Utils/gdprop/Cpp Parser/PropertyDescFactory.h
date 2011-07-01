/**
 *  @file      PropertyDescFactory.h
 *  @brief	   PropertyDesc factory.
 *  @author    Sébastien Lussier.
 *  @date      28/02/06.
 */
/*
 *  Copyright (C) 2006 Gamedesk
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
#ifndef     _CPP_PROPERTY_DESC_FACTORY_
#define     _CPP_PROPERTY_DESC_FACTORY_


#include "Cpp Parser/PropertyDesc.h"
#include "Cpp Parser/PropertyDescSpecializations.h"


namespace Cpp
{
    struct NameID
    {
        const Char* TypeName;
        UInt32      TypeID;

        NameID(const Char* pTypeName, UInt32 pTypeID)
            : TypeName(pTypeName)
            , TypeID(pTypeID)
        {
        }
    };

    Bool IsASupportedPropertyType( const String& pType, UInt32& pTypeID )
    {
        static NameID supportedTypes[] =
        { 
            NameID("Bool",        PropertyBool::ID()),
            NameID("Char",        PropertyChar::ID()),
            NameID("Int16",       PropertyInt16::ID()),
            NameID("Int32",       PropertyInt32::ID()),
            NameID("Int64",       PropertyInt64::ID()),
            NameID("Byte",        PropertyByte::ID()),
            NameID("UInt16",      PropertyUInt16::ID()),
            NameID("UInt32",      PropertyUInt32::ID()),
            NameID("UInt64",      PropertyUInt64::ID()),
            NameID("Float",       PropertyFloat::ID()),
            NameID("Double",      PropertyDouble::ID()),
            NameID("Vector3f",    PropertyVector3f::ID()),
            NameID("Quaternionf", PropertyQuaternionf::ID()),
            NameID("Color3f",     PropertyColor3f::ID()),
            NameID("Color4f",     PropertyColor4f::ID()),
            NameID("String",      PropertyString::ID())
        };

        for( UInt32 i = 0; i < (sizeof(supportedTypes) / sizeof(supportedTypes[0])); ++i )
        {
            if( strcmp(pType.c_str(), supportedTypes[i].TypeName) == 0 )
            {
                pTypeID = supportedTypes[i].TypeID;
                return true;
            }
        }

        return false;
    }

    PropertyDesc* CreatePropertyDesc( UInt32 pTypeID, const String& pPropertyType, const String& pPropertyName )
    {
        if( pTypeID == PropertyBool::ID() )
            return GD_NEW(BoolPropertyDesc, 0, "BoolPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyChar::ID() )
            return GD_NEW(CharPropertyDesc, 0, "CharPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyInt16::ID() )
            return GD_NEW(Int16PropertyDesc, 0, "Int16PropertyDesc")(pPropertyName);
        
        else if( pTypeID == PropertyInt32::ID() )
            return GD_NEW(Int32PropertyDesc, 0, "Int32PropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyInt64::ID() )
            return GD_NEW(Int64PropertyDesc, 0, "Int64PropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyByte::ID() )
            return GD_NEW(BytePropertyDesc, 0, "BytePropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyUInt16::ID() )
            return GD_NEW(UInt16PropertyDesc, 0, "UInt16PropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyUInt32::ID() )
            return GD_NEW(UInt32PropertyDesc, 0, "UInt32PropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyUInt64::ID() )
            return GD_NEW(UInt64PropertyDesc, 0, "UInt64PropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyFloat::ID() )
            return GD_NEW(FloatPropertyDesc, 0, "FloatPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyDouble::ID() )
            return GD_NEW(DoublePropertyDesc, 0, "DoublePropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyVector3f::ID() )
            return GD_NEW(Vector3fPropertyDesc, 0, "Vector3fPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyQuaternionf::ID() )
            return GD_NEW(QuaternionfPropertyDesc, 0, "QuaternionfPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyColor3f::ID() )
            return GD_NEW(Color3fPropertyDesc, 0, "Color3fPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyColor4f::ID() )
            return GD_NEW(Color4fPropertyDesc, 0, "Color4fPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyString::ID() )
            return GD_NEW(StringPropertyDesc, 0, "StringPropertyDesc")(pPropertyName);

        else if( pTypeID == PropertyEnum::ID() )
            return GD_NEW(EnumPropertyDesc, 0, "EnumPropertyDesc")(pPropertyType, pPropertyName);

        GD_ASSERT_M(false, "Unsupported property type!");
        return NULL;
    };
}


#endif  //  _CPP_PROPERTY_DESC_FACTORY_
