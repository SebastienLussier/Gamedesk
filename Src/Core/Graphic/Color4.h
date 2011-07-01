/**
 *  @file       Color4.h
 *  @brief      Declaration of the Color4 class.
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
#ifndef     _COLOR4_H_
#define     _COLOR4_H_


#include "Maths/Maths.h"


namespace Gamedesk {


/**
 *  Representation of a color using red, green, blue and alpha components.
 *  @brief  RGBA Color class.
 */
template <class T>
class Color4
{
public:
    /**
     *  Constructor.
     */
    Color4() {}

    /**
     *  Construct a Color4 using RGBA values in the range [0,1].
     *  @param  pRed    Red value.
     *  @param  pGreen  Green value.
     *  @param  pBlue   Blue value.
     *  @param  pAlpha  Alpha value.
     */
    Color4( T pRed, T pGreen, T pBlue, T pAlpha = static_cast<T>(1.0) )
        : R(pRed)
        , G(pGreen)
        , B(pBlue)
        , A(pAlpha)
    {}

    /**
     *  Construct a Color4 using RGBA values in the range [0,255].
     *  @param  pRed    Red value (as a byte).
     *  @param  pGreen  Green value (as a byte).
     *  @param  pBlue   Blue value (as a byte).
     *  @param  pAlpha  Alpha value (as a byte).
     */
    Color4( Byte pRed, Byte pGreen, Byte pBlue, Byte pAlpha = Number<Byte>::Max )
        : R(pRed   / static_cast<T>(Number<Byte>::Max))
        , G(pGreen / static_cast<T>(Number<Byte>::Max))
        , B(pBlue  / static_cast<T>(Number<Byte>::Max))
        , A(pAlpha / static_cast<T>(Number<Byte>::Max))
    {}

    /**
     *  Construct a Color4 using an hexadecimal code (0xRRGGBBAA).
     *  @param  pRGBA    Value representing the color in hex.
     */
    Color4( UInt32 pRGBA )
        : R( ((pRGBA >> 24) & 0xFF) / static_cast<T>(Number<Byte>::Max) )
        , G( ((pRGBA >> 16) & 0xFF) / static_cast<T>(Number<Byte>::Max) )
        , B( ((pRGBA >>  8) & 0xFF)  / static_cast<T>(Number<Byte>::Max) )
        , A( ((pRGBA >>  0) & 0xFF)  / static_cast<T>(Number<Byte>::Max) )

    {
    }

    UInt32 ToR8G8B8A8() const
    {
        return Byte(R * 255) << 24 | Byte(G * 255) << 16 | Byte(B * 255) << 8 | Byte(A * 255);
    }

    UInt32 ToR8G8B8() const
    {
        return Byte(R * 255) << 24 | Byte(G * 255) << 16 | Byte(B * 255) << 8 | 255;
    }

    UInt32 ToA8B8G8R8() const
	{
		return Byte(A * 255) << 24 | Byte(B * 255) << 16 | Byte(G * 255) << 8 | Byte(R * 255);
	}

	UInt32 ToB8G8R8() const
	{
		return Byte(B * 255) << 24 | Byte(G * 255) << 16 | Byte(R * 255) << 8 | 255;
	}

    /**
     *  @brief  Multiplication operator (with a Color4).
     *  @param  pOther  Color used for the multiplication.
     *  @return A new color which is the result of the operation.
     */
    Color4 operator * ( const Color4& pOther ) const
    {
        return Color4( R*pOther.R, G*pOther.G, B*pOther.B, A*pOther.A );
    }

    /**
     *  @brief  Multiplication assigment operator (with a Color4).
     *  @param  pOther  Color used for the multiplication.
     *  @return A reference to this color.
     */
    Color4& operator *= ( const Color4& pOther )
    {
        R *= pOther.R;  G *= pOther.G;  B *= pOther.B;  A *= pOther.A;
        return *this;
    }

    /**
     *  @brief  Multiplication operator (with a scalar).
     *  @param  pScalar     Value that will divide each element of the color.
     *  @return A new color which is the result of the operation.
     */
    Color4 operator * ( const T& pScalar ) const
    {
        return Color4( R*pScalar, G*pScalar, B*pScalar, A*pScalar );
    }

    /**
     *  @brief  Multiplication assigment operator (with a scalar).
     *  @param  pScalar     Value that will multiply each element of the color.
     *  @return A reference to this color.
     */
    Color4& operator *= ( const T& pScalar )
    {
        R *= pScalar;  G *= pScalar;  B *= pScalar;  A *= pScalar;
        return *this;
    }

    /**
     *  @brief  Division operator (with a scalar).
     *  @param  pScalar     Value that will divide each element of the color.
     *  @return A new color which is the result of the operation.
     */
    Color4 operator / ( const T& pScalar ) const
    {
        return Color4( R/pScalar, G/pScalar, B/pScalar, A/pScalar );
    }

    /**
     *  @brief  Division assigment operator (with a scalar).
     *  @param  pScalar     Value that will divide each element of the color.
     *  @return A reference to this color.
     */
    Color4&	operator /= ( const T& pScalar )
    {
        R /= pScalar;  G /= pScalar;  B /= pScalar;  A /= pScalar;
        return *this;
    }

    /**
     *  @brief  Addition operator.
     *  @param  pOther  Color used for the addition.
     *  @return A new color which is the result of the operation.
     */
    Color4 operator + ( const Color4& pOther ) const
    {
        return Color4( R+pOther.R, G+pOther.G, B+pOther.B, A+pOther.A );
    }

    /**
     *  @brief  Addition assigment operator.
     *  @param  pOther  Color used for the addition.
     *  @return A reference to this color.
     */
    Color4&	operator += ( const Color4& pOther )
    {
        R += pOther.R;  G += pOther.G;  B += pOther.B;  A += pOther.A;
        return *this;
    }

    /**
     *  @brief  Substraction operator.
     *  @param  pOther  Color used for the substraction.
     *  @return A new color which is the result of the operation.
     */
    Color4	operator - ( const Color4& pOther ) const
    {
        return Color4( R-pOther.R, G-pOther.G, B-pOther.B, A-pOther.A );
    }

    /**
     *  @brief  Substraction assigment operator.
     *  @param  pOther  Color used for the substraction.
     *  @return A reference to this color.
     */
    Color4&	operator -= ( const Color4& pOther )
    {
        R -= pOther.R;  G -= pOther.G;  B -= pOther.B;  A -= pOther.A;
        return *this;
    }

    /**
     *  @brief  Equality operator.
     *  @param  pOther  Color used for the comparision.
     *  @return True if colors are equals, false otherwise.
     */
	Bool operator == ( const Color4& pOther ) const
    {
        return (R == pOther.R) && (G == pOther.G) && (B == pOther.B) && (A == pOther.A);
    }

    /**
     *  @brief  Not equal operator.
     *  @param  pOther  Color used for the comparision.
     *  @return False if colors are equals, true otherwise.
     */
	Bool	operator != ( const Color4& pOther ) const
    {
        return (R != pOther.R) || (G != pOther.G) || (B != pOther.B) || (A != pOther.A);
    }

    /**
     *  @brief  Assigment operator (with a Color4).
     *  @param  pOther  Color used for the assignation.
     *  @return A reference to this color.
     */
    Color4&	operator = ( const Color4& pOther )
    {
        R = pOther.R;   G = pOther.G;   B = pOther.B;   A = pOther.A;
        return *this;
    }

    /**
     *  @brief  Assigment operator (with a scalar).
     *  @param  pScalar     Value to be assigned to each element of the color.
     *  @return A reference to this color.
     */
	Color4&	operator = ( const T& pScalar )
    {
        R = pScalar;    G = pScalar;    B = pScalar;    A = pScalar;
        return *this;
    }

    /**
     *  Get a pointer to the internal representation of the color.
     *  @brief  Indirection operator (const).
     *  @return A pointer to a T array.
     */
    operator const T* () const
    {
        return RGBA;
    }

    /**
     *  Get a pointer to the internal representation of the color.
     *  @brief  Indirection operator.
     *  @return A pointer to a T array.
     */
    operator T* ()
    {
        return RGBA;
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
               B >= static_cast<T>(0.0) && B <= static_cast<T>(1.0) &&
               A >= static_cast<T>(0.0) && A <= static_cast<T>(1.0);
    }

    /**
     *  @brief  Bring back all of the color elements to the range [pMin, pMax]
     *  @param  pMin    Minimum value accepted for a color component.
     *  @param  pMax    Maximum value accepted for a color component.
     *  @return A reference to this color, now clamped to the given range.
     */
    Color4& Clamp( T pMin = static_cast<T>(0.0), T pMax = static_cast<T>(1.0) )
    {
        Maths::Clamp( R, pMin, pMax );
        Maths::Clamp( G, pMin, pMax );
        Maths::Clamp( B, pMin, pMax );
        Maths::Clamp( A, pMin, pMax );

        return *this;
    }

    /**
     *  Check if two colors are equals using an epsilon.
     *  @param  pOther  The other color.
     *  @param  pEps    The epsilon value.
     *  @return \bTrue if the colors values fit in the epsilon range.
     */
    Bool EqualEps( const Color4& pOther, T pEps = Maths::EPSILON ) const
    {
        return  R > (pOther.R - pEps) && R < (pOther.R + pEps) &&
                G > (pOther.G - pEps) && G < (pOther.G + pEps) &&
                B > (pOther.B - pEps) && B < (pOther.B + pEps) &&
                A > (pOther.A - pEps) && A < (pOther.A + pEps);
    }

    Color4 RGBToHSV() const
    {
        T minVal = Maths::Min<T>(R, G, B);

        T h;
        T s;
        T v = Maths::Max<T>(R, G, B);

        T delta = v - minVal;

        // Calculate saturation: saturation is 0 if r, g and b are all 0
        if( v == T(0) )
            s = T(0);
        else
            s = delta / v;

        if( s == T(0) )
        {
            h = T(0);   // Achromatic: When s = 0, h is undefined but who cares
        }
        else // chromatic
        {
            if( v == R )                    // Between yellow and magenta.
                h = T(0)   + T(60) * (G-B) / delta;
            else if( v == G )               // Between cyan and yellow.
                h = T(120) + T(60) * (B-R) / delta;
            else                            // Between magenta and cyan.
                h = T(240) + T(60) * (R-G) / delta;
        }

        if( h < T(0) )
            h += T(360);

        return Color4(h, s, v, A);
    }

    Color4 HSVToRGB() const
    {
        T   r, g, b;
        T   hTemp;

        if( S == 0 ) // Color is on black-and-white center line achromatic: shades of gray
            return Color4( V, V, V, A );

        // Chromatic color
        if( H == T(360) )
            hTemp = T(0);
        else
            hTemp = H;

        hTemp = hTemp / T(60);
        Byte i = Maths::Floor(hTemp);

        T f = hTemp - i;
        T p = V * (T(1) - S);
        T q = V * (T(1) - (S*f));
        T t = V * (T(1) - (S*(T(1)-f)));

        switch( i )
        {
        case 0:
            r = V;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = V;
            b = p;
            break;

        case 2:
            r = p;
            g = V;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = V;
            break;

        case 4:
            r = t;
            g = p;
            b = V;
            break;

        case 5:
        default:
            r = V;
            g = p;
            b = q;
            break;
        }

        return Color4(r, g, b, A);
    }

    Color4 Darken( T pFactor ) const
    {
        if( pFactor <= T(0) )               // Invalid darkness factor.
            return *this;
        else if( pFactor < T(1) )           // Makes color lighter.
            return Lighten( T(1) / pFactor );

        Color4 hsv = RGBToHSV();
        hsv.V = (hsv.V) / pFactor;

        // Convert back to same color spec as original color.
        return hsv.HSVToRGB();
    }

    Color4 Lighten( T pFactor ) const
    {
        if( pFactor <= T(0) )                // Invalid darkness factor.
            return *this;
        else if( pFactor < T(1) )           // Makes color lighter.
            return Darken( T(1) / pFactor );

        Color4 hsv = RGBToHSV();

        hsv.V *= pFactor;
        if( hsv.V > T(1) )
        {
            // Overflow... adjust saturation
            hsv.S -= hsv.V - T(1);
            if( hsv.S < T(0) )
                hsv.S = T(0);

            hsv.V = T(1);
        }

        // Convert back to same color spec as original color.
        return hsv.HSVToRGB();
    }

    Color4 Blend( Color4 pOther, T pTime = T(0.5) ) const
    {
        Color4 newColor;

        newColor.R = Maths::Interpolate( R, pOther.R, pTime );
        newColor.G = Maths::Interpolate( G, pOther.G, pTime );
        newColor.B = Maths::Interpolate( B, pOther.B, pTime );
        newColor.A = Maths::Interpolate( A, pOther.A, pTime );

        return newColor;
    }

    /**
     *  Serialize this color to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Color4<T> pColor )
    {
        pStream << pColor.R << pColor.G << pColor.B << pColor.A;
        return pStream;
    }

    static const Color4 BLACK;      //!< Black color.
    static const Color4 WHITE;      //!< White color.
    static const Color4 GRAY;       //!< Gray color.
    static const Color4 RED;        //!< Red color.
    static const Color4 GREEN;      //!< Green color.
    static const Color4 BLUE;       //!< Blue color.
    static const Color4 YELLOW;     //!< Yellow color.
    static const Color4 BROWN;      //!< Brown color.
    static const Color4 CYAN;       //!< Cyan color.
    static const Color4 MAGENTA;    //!< Magenta color.

public:
    union
    {
        //! RGBA
        struct
        {
            union
            {
                T   R;      //!< Red.
                T   H;      //!< Hue.
            };

            union
            {
                T   G;      //!< Green.
                T   S;      //!< Saturation.
            };

            union
            {
                T   B;      //!< Blue.
                T   V;      //!< Value.
            };

            T   A;          //!< Alpha.
        };

        T   RGBA[4];        //!< Array access to the component of the color.
        T   HSVA[4];        //!< Array access to the component of the color.
    };
};


typedef Color4<Float>    Color4f;
typedef Color4<Double>   Color4d;

// Constants
template<class T> const Color4<T> Color4<T>::BLACK     ( T(0), T(0), T(0), T(1) );
template<class T> const Color4<T> Color4<T>::WHITE     ( T(1), T(1), T(1), T(1) );
template<class T> const Color4<T> Color4<T>::RED       ( T(1), 0.0f, T(0), T(1) );
template<class T> const Color4<T> Color4<T>::GREEN     ( T(0), 1.0f, T(0), T(1) );
template<class T> const Color4<T> Color4<T>::BLUE      ( T(0), T(0), T(1), T(1) );
template<class T> const Color4<T> Color4<T>::YELLOW    ( T(1), T(1), T(0), T(1) );
template<class T> const Color4<T> Color4<T>::CYAN      ( T(0), T(1), T(1), T(1) );
template<class T> const Color4<T> Color4<T>::MAGENTA   ( T(1), T(0), T(1), T(1) );
template<class T> const Color4<T> Color4<T>::GRAY      ( T(0.5), T(0.5), T(0.5), T(1) );


} // namespace Gamedesk


#endif  //  _COLOR4_H_
