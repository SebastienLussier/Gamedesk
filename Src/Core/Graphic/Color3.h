/**
 *  @file       Color3.h
 *  @brief      Declaration of the Color3 class.
 *  @author     Sébastien Lussier.
 *  @date       25/11/03.
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
#ifndef     _COLOR3_H_
#define     _COLOR3_H_


#include "Maths/Maths.h"


namespace Gamedesk {
	

/**
 *  Representation of a color using red, green, blue components (no alpha).
 *  @brief  RGB Color class.
 */
template <class T>
class Color3
{
public:
    /**
     *  Constructor.
     */
    Color3() {}

    /**
     *  Construct a Color3 using RGB values in the range [0,1].
     *  @param  pRed    Red value.
     *  @param  pGreen  Green value.
     *  @param  pBlue   Blue value.
     */
    Color3( T pRed, T pGreen, T pBlue ) :
        R(pRed),
        G(pGreen),
        B(pBlue)
    {}

    /**
     *  Construct a Color3 using RGB values in the range [0,255].
     *  @param  pRed    Red value (as a byte).
     *  @param  pGreen  Green value (as a byte).
     *  @param  pBlue   Blue value (as a byte).
     */
    Color3( Byte pRed, Byte pGreen, Byte pBlue ) :
        R(pRed   / static_cast<T>(255)),
        G(pGreen / static_cast<T>(255)),
        B(pBlue  / static_cast<T>(255))
    {}

    UInt32 ToR8G8B8A8() const
    {
        return Byte(R * 255) << 24 | Byte(G * 255) << 16 | Byte(B * 255) << 8 | 255;
    }

    UInt32 ToR8G8B8() const
    {
        return Byte(R * 255) << 24 | Byte(G * 255) << 16 | Byte(B * 255) << 8 | 255;
    }

    /**
     *  @brief  Multiplication operator (with a Color3).
     *  @param  pOther  Color used for the multiplication.
     *  @return A new color which is the result of the operation.
     */
    Color3 operator * ( const Color3& pOther ) const
    {
        return Color3( R*pOther.R, G*pOther.G, B*pOther.B );
    }

    /**
     *  @brief  Multiplication assigment operator (with a Color3).
     *  @param  pOther  Color used for the multiplication.
     *  @return A reference to this color.
     */
    Color3& operator *= ( const Color3& pOther )
    {
        R *= pOther.R;  G *= pOther.G;  B *= pOther.B;
        return *this;
    }

    /**
     *  @brief  Multiplication operator (with a scalar).
     *  @param  pScalar     Value that will divide each element of the color.
     *  @return A new color which is the result of the operation.
     */
    Color3 operator * ( const T& pScalar ) const
    {
        return Color3( R*pScalar, G*pScalar, B*pScalar );
    }

    /**
     *  @brief  Multiplication assigment operator (with a scalar).
     *  @param  pScalar     Value that will multiply each element of the color.
     *  @return A reference to this color.
     */
    Color3& operator *= ( const T& pScalar )
    {
        R *= pScalar;  G *= pScalar;  B *= pScalar;
        return *this;
    }

    /**
     *  @brief  Division operator (with a scalar).
     *  @param  pScalar     Value that will divide each element of the color.
     *  @return A new color which is the result of the operation.
     */
    Color3 operator / ( const T& pScalar ) const
    {
        return Color3( R/pScalar, G/pScalar, B/pScalar );
    }

    /**
     *  @brief  Division assigment operator (with a scalar).
     *  @param  pScalar     Value that will divide each element of the color.
     *  @return A reference to this color.
     */
    Color3&	operator /= ( const T& pScalar )
    {
        R /= pScalar;  G /= pScalar;  B /= pScalar;
        return *this;
    }

    /**
     *  @brief  Addition operator.
     *  @param  pOther  Color used for the addition.
     *  @return A new color which is the result of the operation.
     */
    Color3 operator + ( const Color3& pOther ) const
    {
        return Color3( R+pOther.R, G+pOther.G, B+pOther.B );
    }

    /**
     *  @brief  Addition assigment operator.
     *  @param  pOther  Color used for the addition.
     *  @return A reference to this color.
     */
    Color3&	operator += ( const Color3& pOther )
    {
        R += pOther.R;  G += pOther.G;  B += pOther.B;
        return *this;
    }

    /**
     *  @brief  Substraction operator.
     *  @param  pOther  Color used for the substraction.
     *  @return A new color which is the result of the operation.
     */
    Color3	operator - ( const Color3& pOther ) const
    {
        return Color3( R-pOther.R, G-pOther.G, B-pOther.B );
    }

    /**
     *  @brief  Substraction assigment operator.
     *  @param  pOther  Color used for the substraction.
     *  @return A reference to this color.
     */
    Color3&	operator -= ( const Color3& pOther )
    {
        R -= pOther.R;  G -= pOther.G;  B -= pOther.B;;
        return *this;
    }

    /**
     *  @brief  Equality operator.
     *  @param  pOther  Color used for the comparision.
     *  @return True if colors are equals, false otherwise.
     */
	Bool operator == ( const Color3& pOther ) const
    {
        return (R == pOther.R) && (G == pOther.G) && (B == pOther.B);
    }

    /**
     *  @brief  Not equal operator.
     *  @param  pOther  Color used for the comparision.
     *  @return False if colors are equals, true otherwise.
     */
	Bool	operator != ( const Color3& pOther ) const
    {
        return (R != pOther.R) || (G != pOther.G) || (B != pOther.B);
    }

    /**
     *  @brief  Assigment operator (with a Color3).
     *  @param  pOther  Color used for the assignation.
     *  @return A reference to this color.
     */
    Color3&	operator = ( const Color3& pOther )
    {
        R = pOther.R;   G = pOther.G;   B = pOther.B;
        return *this;
    }

    /**
     *  @brief  Assigment operator (with a scalar).
     *  @param  pScalar     Value to be assigned to each element of the color.
     *  @return A reference to this color.
     */
	Color3&	operator = ( const T& pScalar )
    {
        R = pScalar;    G = pScalar;    B = pScalar;
        return *this;
    }

    /**
     *  Get a pointer to the internal representation of the color.
     *  @brief  Indirection operator (const).
     *  @return A pointer to a T array.
     */
    operator const T* () const
    {
        return RGB;
    }

    /**
     *  Get a pointer to the internal representation of the color.
     *  @brief  Indirection operator.
     *  @return A pointer to a T array.
     */
    operator T* ()
    {
        return RGB;
    }

    /**
     *  Test the validity of this color by ensuring that all it's elements are in the range [0,1].
     *  @brief  Test the validity of this color.
     *  @return True if all elements are in the rangle [0,1] or false otherwise.
     */
    Bool IsValid() const
    {
        return R >= static_cast<T>(0.0) && R <= static_cast<T>(1.0) &&
               G >= static_cast<T>(0.0) && G <= static_cast<T>(1.0) &&
               B >= static_cast<T>(0.0) && B <= static_cast<T>(1.0);
    }

    /**
     *  @brief  Bring back all of the color elements to the range [pMin, pMax]
     *  @param  pMin    Minimum value accepted for a color component.
     *  @param  pMax    Maximum value accepted for a color component.
     *  @return A reference to this color, now clamped to the given range.
     */
    Color3& Clamp( T pMin = static_cast<T>(0.0), T pMax = static_cast<T>(1.0) )
    {
        Maths::Clamp( R, pMin, pMax );
        Maths::Clamp( G, pMin, pMax );
        Maths::Clamp( B, pMin, pMax );

        return *this;
    }

    /**
     *  Check if two colors are equals using an epsilon.
     *  @param  pOther  The other color.
     *  @param  pEps    The epsilon value.
     *  @return \bTrue if the colors values fit in the epsilon range.
     */
    Bool EqualEps( const Color3& pOther, T pEps = Maths::EPSILON ) const
    {
        return  R > (pOther.R - pEps) && R < (pOther.R + pEps) &&
                G > (pOther.G - pEps) && G < (pOther.G + pEps) &&
                B > (pOther.B - pEps) && B < (pOther.B + pEps);
    }

    /**
     *  Serialize this color to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Color3<T> pColor )
    {
        pStream << pColor.R << pColor.G << pColor.B;
        return pStream;
    }

    static const Color3 BLACK;      //!< Black color.
    static const Color3 WHITE;      //!< White color.
    static const Color3 GRAY;       //!< Gray color.
    static const Color3 RED;        //!< Red color.
    static const Color3 GREEN;      //!< Green color.
    static const Color3 BLUE;       //!< Blue color.
    static const Color3 YELLOW;     //!< Yellow color.
    static const Color3 BROWN;      //!< Brown color.
    static const Color3 CYAN;       //!< Cyan color.
    static const Color3 MAGENTA;    //!< Magenta color.

public:
    union
    {
        struct
        {
            T   R;      //!< Red component of the color.
            T   G;      //!< Green component of the color.
            T   B;      //!< Blue component of the color.
        };

        T   RGB[3];     //!< Array access to the component of the color.
    };
};


typedef Color3<Float>    Color3f;
typedef Color3<Double>   Color3d;

// Constants
template<class T> const Color3<T> Color3<T>::BLACK     ( T(0), T(0), T(0) );
template<class T> const Color3<T> Color3<T>::WHITE     ( T(1), T(1), T(1) );
template<class T> const Color3<T> Color3<T>::RED       ( T(1), 0.0f, T(0) );
template<class T> const Color3<T> Color3<T>::GREEN     ( T(0), 1.0f, T(0) );
template<class T> const Color3<T> Color3<T>::BLUE      ( T(0), T(0), T(1) );
template<class T> const Color3<T> Color3<T>::YELLOW    ( T(1), T(1), T(0) );
template<class T> const Color3<T> Color3<T>::CYAN      ( T(0), T(1), T(1) );
template<class T> const Color3<T> Color3<T>::MAGENTA   ( T(1), T(0), T(1) );
template<class T> const Color3<T> Color3<T>::GRAY      ( T(0.5), T(0.5), T(0.5) );


} // namespace Gamedesk


#endif  //  _COLOR3_H_
