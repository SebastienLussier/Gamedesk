/**
 *  @file       Variant.cpp
 *  @brief	    Definition of the Variant class.
 *  @author     Sébastien Lussier.
 *  @date       24/10/03.
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
#include "Variant.h"


namespace Gamedesk {


///////////////////////////////////////////////////////////////////////////////
// Variant
Variant::Variant()                          : mType( Variant_None ){}
Variant::Variant( Bool          pValue )    : mType( Variant_None ){   *this = pValue;     }
Variant::Variant( Int32         pValue )    : mType( Variant_None ){   *this = pValue;     }
Variant::Variant( Float         pValue )    : mType( Variant_None ){   *this = pValue;     }
Variant::Variant( const String& pValue )    : mType( Variant_None ){   *this = pValue;     }

Variant::~Variant()
{
    Clear();   
}

Variant::Variant( const Variant& pOther )   
{  
    switch( pOther.mType )
    {
    case Variant_Bool:   
        *this = pOther.operator Bool();
        break;
        
    case Variant_Int:
        *this = pOther.operator Int32();
        break;
        
    case Variant_Float:
        *this = pOther.operator Float();
        break;
        
    case Variant_String: 
        *this = pOther.operator String();
        break;
        
    default:
        break;
    }
}

const Variant& Variant::operator = ( Bool pValue )
{
    Clear();
    
    mType       = Variant_Bool;
    mBoolValue  = pValue;

    return *this;
}

const Variant& Variant::operator = ( Int32 pValue )
{
    Clear();

    mType       = Variant_Int;
    mIntValue   = pValue;

    return *this;
}

const Variant& Variant::operator = ( Float pValue )
{
    Clear();

    mType       = Variant_Float;
    mFloatValue = pValue;

    return *this;
}

const Variant& Variant::operator = ( const String& pValue )
{
    if( mType != Variant_String )
    {
        Clear();
        mStringValue = GD_NEW(String, this, "Core::Config::Variant::String");
    }

    mType         = Variant_String;
    *mStringValue = pValue;

    return *this;
}

Variant::operator Bool () const
{
    switch( mType )
    {
    case Variant_Bool:        
        return mBoolValue;
        break;
        
    case Variant_Int:         
        return mIntValue != 0;
        break;
        
    case Variant_String:      
        if( ToUpper( *mStringValue ) == "TRUE" ) 
            return true;
        break;
        
    default:
        break;
    }

    return false;
}

Variant::operator Int32 () const
{
    switch( mType )
    {
    case Variant_Int:
        return mIntValue;
        
    case Variant_Bool:
        return mBoolValue == true ? 1 : 0;
            
    case Variant_String:
        return StringTo<Int32>( mStringValue->c_str() );
        
    default:
        break;
    }

    return 0;
}

Variant::operator Float () const
{
    switch( mType )
    {
    case Variant_Float:
        return mFloatValue;
        
    case Variant_Int:
        return mIntValue;
        
    case Variant_Bool:
        return mBoolValue == true ? 1.0f : 0.0f;
            
    case Variant_String:
        return StringTo<Float>( mStringValue->c_str() );
        
    default:
        break;
    }

    return 0;
}

Variant::operator String () const
{
    switch( mType )
    {
    case Variant_String:
        return (*mStringValue);
        
    case Variant_Bool:
        return mBoolValue ? "True" : "False";
            
    case Variant_Int:           
        return ToString( mIntValue );

    case Variant_Float:
        return ToString( mFloatValue );

    default:
        break;
    }

    return "";
}

void Variant::Clear()
{
    if( mType == Variant_String )
        GD_DELETE(mStringValue);
}

Bool Variant::IsOfType( Variant::eVariantType pType ) const
{
    return mType == pType;
}

Variant::eVariantType Variant::GetType() const
{
    return mType;
}


} // namespace Gamedesk
