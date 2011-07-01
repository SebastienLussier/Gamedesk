/**
 *  @file       Variant.h
 *  @brief      Declaration of the Variant class.
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
#ifndef     _VARIANT_H_
#define     _VARIANT_H_


namespace Gamedesk {
	
	
/**
 *  Use this to work with variables without even knowing their type.
 *  @author Sébastien Lussier
 */
class CORE_API Variant
{
public:
    //! Variant type.
    enum eVariantType
    {
        Variant_None,       //!< No type.
        Variant_Bool,       //!< Boolean.
        Variant_Int,        //!< Integer.
        Variant_Float,      //!< Float.
        Variant_String,     //!< String.
    };

public:
 
    Variant();                             //!< Default constructor. Variant will be of type Variant_None.
    Variant( Bool           pValue );      //!< Boolean constructor. Variant will be of type Variant_Bool.
    Variant( Int32          pValue );      //!< Integer constructor. Variant will be of type Variant_Int.
    Variant( Float          pValue );      //!< Float constructor. Variant will be of type Variant_Float.
    Variant( const String&  pValue );      //!< String constructor. Variant will be of type Variant_String.
    Variant( const Variant& pOther );      //!< Copy constructor. 
    
    virtual ~Variant();                    //!< Destructor.
    
    //@{
    //! operator equal for each possible data types.
    const Variant& operator = ( Bool            pValue );
    const Variant& operator = ( Int32           pValue );
    const Variant& operator = ( Float           pValue );
    const Variant& operator = ( const String&   pValue );
    //@}
 
    //@{
    //! cast operator for each possible data types.
    operator Int32  () const;
    operator Float  () const; 
    operator String () const;
    operator Bool   () const;
    //@}

    /**
     *  Return true if the variant is of the type passed in parameter.
     *  @param  pType   Type to be tested with this variant's type.
     *  @return true if this variant is of type pType.
     */
    Bool          IsOfType( Variant::eVariantType pType ) const;

    /**
     *  Get the type of the variant.
     *  @return type of the variant.
     */
    Variant::eVariantType GetType() const;

protected:
    /** @internal
     *  Internal cleanup.
     */
    void Clear();

    eVariantType  mType;                       //!< Type of variable.

    union
    {
        Bool    mBoolValue;                    //!< Value as a boolean.
        Int32   mIntValue;                     //!< Value as an integer.
        Float   mFloatValue;                   //!< Value as a float.
        String* mStringValue;                  //!< Value as a string.
    };
};


} // namespace Gamedesk


#endif  //  _VARIANT_H_
