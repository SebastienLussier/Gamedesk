/**
 *  @file       Maths.h
 *  @brief      Useful maths classes, functions and constants.
 *  @author     Sebastien Lussier.
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
#ifndef     _MATHS_H_
#define     _MATHS_H_


namespace Gamedesk {


/**
 *  Namespace containing all kind of maths stuff like Vector3D, Matrix etc.
 *  @brief  Math namespace.
 *  @author Sebastien Lussier.
 *  @date   19/01/02
 */
namespace Maths
{
    const Float     HALF_PI         = 1.5707963267948966192313216916398f;    //!< PI.
    const Float     PI              = 3.1415926535897932384626433832795f;    //!< PI.
    const Float     PI_2            = 6.283185307179586476925286766559f;    //!< 2*PI.
    const Float     PI_ON_180       = 0.017453292519943295769236907684886f; //!< PI/180.
    const Float     ONE_180_ON_PI   = 57.295779513082320876798154814105f;   //!< 180/PI
    const Float     EPSILON         = 1e-6f;                                //!< A very small float value.

    /**
     *  Compute a random T value between fMin and fMax.
     *  @brief  Random for T, given a range.
     *  @param  fMin    a T, minimum value wanted.
     *  @param  fMax    a T, maximum value wanted.
     *  @return A random real value between fMin and fMax.
     */
    template <class T>
    INLINE T Rand( const T& pMin, const T& pMax )
    {
        return pMin + (pMax - pMin) * rand() / (T)RAND_MAX;
    }

    /**
     *  Compute a random int value between iMin and iMax.
     *  @brief  Random for an integer, given a range.
     *  @param  iMin    an integer, minimum value wanted.
     *  @param  iMax    an integer, maximum value wanted.
     *  @return A random integer value between iMin and iMax.
     */
    template <>
    INLINE Int32 Rand( const Int32& pMin, const Int32& pMax )
    {
        return (rand() % (pMax - pMin + 1)) + pMin;
    }
    
    //! Returns the maximum value between 2 values.
    template <class T>
    INLINE T Max( const T& pValueA, const T& pValueB )
    {
        return pValueA >= pValueB ? pValueA : pValueB;
    }

    //! Returns the maximum value between 3 values.
    template <class T>
    INLINE T Max( const T& pValueA, const T& pValueB, const T& pValueC )
    {
        return pValueA >= pValueB ? (pValueA >= pValueC ? pValueA : pValueC) : (pValueB >= pValueC ? pValueB : pValueC);
    }

	//! Returns the minimum value between 2 values.
    template <class T>
    INLINE T Min( const T& pValueA, const T& pValueB )
    {
        return pValueA <= pValueB ? pValueA : pValueB;
    }    

    //! Returns the minimum value between 3 values.
    template <class T>
    INLINE T Min( const T& pValueA, const T& pValueB, const T& pValueC )
    {
        return pValueA <= pValueB ? (pValueA <= pValueC ? pValueA : pValueC) : (pValueB <= pValueC ? pValueB : pValueC);
    }

    template <class T>
    INLINE T Floor( const T& pValue )
    {
        return pValue;
    }

    template <>
    INLINE Float Floor( const Float& pValue )
    {
        return floor(pValue);
    }

    template <>
    INLINE Double Floor( const Double& pValue )
    {
        return floor(pValue);
    }

    template <class T>
    INLINE T Ceil( const T& pValue )
    {
        return pValue;
    }

    template <>
    INLINE Float Ceil( const Float& pValue )
    {
        return ceil(pValue);
    }

    template <>
    INLINE Double Ceil( const Double& pValue )
    {
        return ceil(pValue);
    }

    /**
     *  Convert angles in degree to their equivalent in radians.
     *  @brief  Degrees to radians conversion.
     *  @param  pAngle  a float, the angle in degree.
     *  @return The angle in radians, as a float.
     */
    INLINE Float    ToRadians( Float    pAngle ) { return pAngle * PI_ON_180; }

    /**
     *  Convert angles in degree to their equivalent in radians.
     *  @brief  Degrees to radians conversion.
     *  @param  pAngle  a double, the angle in degree.
     *  @return The angle in radians, as a double.
     */
    INLINE Double   ToRadians( Double   pAngle ) { return pAngle * PI_ON_180; }


    /**
     *  Convert angles in radians to their equivalent in degrees.
     *  @brief  Radians to degrees conversion.
     *  @param  pAngle  a float, the angle in radians.
     *  @return The angle in degrees, as a float.
     */
    INLINE Float    ToDegrees( Float    pAngle ) { return pAngle * ONE_180_ON_PI; }

    /**
     *  Convert angles in radians to their equivalent in degrees.
     *  @brief  Radians to degrees conversion.
     *  @param  pAngle  a double, the angle in radians.
     *  @return The angle in degrees, as a double.
     */
    INLINE Double   ToDegrees( Double   pAngle ) { return pAngle * ONE_180_ON_PI; }

    /**
     *  Obtain the sinus of an angle in radians.
     *  @brief  Obtain the sinus of an angle in radians.
     *  @param  pAngle  a float, the angle in radians.
     *  @return The sinus value of the angle, as a float.
     */
    INLINE Float    Sin( Float pAngle )     {   return sinf( pAngle ); }

    /**
     *  Obtain the sinus of an angle in radians.
     *  @brief  Obtain the sinus of an angle in radians.
     *  @param  pAngle  a double, the angle in radians.
     *  @return The sinus value of the angle, as a double.
     */
    INLINE Double   Sin( Double pAngle )    {   return sin( pAngle );   }

	/**
     *  Obtain the arcsinus (the angle in radian) of a sinus value.
     *  @brief  Obtain the arcsinus (the angle in radian) of a sinus value.
     *  @param  pValue a float, the sinus value.
     *  @return The angle in radian of the sinus value, as a float.
     */
    INLINE Float    ASin( Float pValue )     {   return asinf( pValue ); }

    /**
     *  Obtain the arcsinus (the angle in radian) of a sinus value.
     *  @brief  Obtain the arcsinus (the angle in radian) of a sinus value.
     *  @param  pValue a double, the sinus value.
     *  @return The angle in radian of the sinus value, as a double.
     */
    INLINE Double   ASin( Double pValue )    {   return asin( pValue );   }

    /**
     *  Obtain the cosinus of an angle in radians.
     *  @brief  Obtain the cosinus of an angle in radians.
     *  @param  pAngle  a float, the angle in radians.
     *  @return The cosinus value of the angle, as a float.
     */
    INLINE Float    Cos( Float pAngle )     {   return cosf( pAngle ); }

    /**
     *  Obtain the cosinus of an angle in radians.
     *  @brief  Obtain the cosinus of an angle in radians.
     *  @param  pAngle  a double, the angle in radians.
     *  @return The cosinus value of the angle, as a double.
     */
    INLINE Double   Cos( Double pAngle )    {   return cos( pAngle );   }

	/**
     *  Obtain the arccosinus (the angle in radian) of a cosinus value.
     *  @brief  Obtain the arccosinus (the angle in radian) of a cosinus value.
     *  @param  pValue a float, the cosinus value.
     *  @return The angle in radian of the cosinus value, as a float.
     */
    INLINE Float    ACos( Float pValue )     {   return acosf( pValue ); }

    /**
     *  Obtain the arccosinus (the angle in radian) of a cosinus value.
     *  @brief  Obtain the arccosinus (the angle in radian) of a cosinus value.
     *  @param  pValue a double, the cosinus value.
     *  @return The angle in radian of the cosinus value, as a double.
     */
    INLINE Double   ACos( Double pValue )    {   return acos( pValue );   }

	/**
     *  Obtain the tangent of an angle in radians.
     *  @brief  Obtain the tangent of an angle in radians.
     *  @param  pAngle  a float, the angle in radians.
     *  @return The tangent value of the angle, as a float.
     */
    INLINE Float    Tan( Float pAngle )     {   return tanf( pAngle ); }

    /**
     *  Obtain the tangent of an angle in radians.
     *  @brief  Obtain the tangent of an angle in radians.
     *  @param  pAngle  a double, the angle in radians.
     *  @return The tangent value of the angle, as a double.
     */
    INLINE Double   Tan( Double pAngle )    {   return tan( pAngle );   }

	/**
     *  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @brief  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @param  pValue a float, the tangent value.
     *  @return The angle in radian of the tangent value, as a float.
     */
    INLINE Float    ATan( Float pValue )     {   return atanf( pValue ); }

    /**
     *  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @brief  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @param  pValue a double, the tangent value.
     *  @return The angle in radian of the tangent value, as a double.
     */
    INLINE Double   ATan( Double pValue )    {   return atan( pValue );   }

    /**
     *  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @brief  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @param  pValue a float, the tangent value.
     *  @return The angle in radian of the tangent value, as a float.
     */
    INLINE Float    ATan2( Float pValueY, Float pValueX )     {   return atan2f( pValueY, pValueX ); }

    /**
     *  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @brief  Obtain the arctangent (the angle in radian) of a tangent value.
     *  @param  pValue a double, the tangent value.
     *  @return The angle in radian of the tangent value, as a double.
     */
    INLINE Double   ATan2( Double pValueY, Double pValueX )    {   return atan2( pValueY, pValueX );   }


    /**
     *  Obtain the absolute value of an integer.
     *  @brief  Obtain the absolute value of an integer.
     *  @param  pValue  an integer, the value to be converted.
     *  @return The absolute value of \a pValue.
     */
    INLINE Int32    Abs( Int32 pValue )     {   return abs( pValue );   }

    /**
     *  Obtain the absolute value of a float.
     *  @brief  Obtain the absolute value of a float.
     *  @param  pValue  a float, the value to be converted.
     *  @return The absolute value of \a pValue.
     */
    INLINE Float    Abs( Float pValue )     {   return fabsf( pValue ); }

    /**
     *  Obtain the absolute value of a double.
     *  @brief  Obtain the absolute value of a double.
     *  @param  pValue  a double, the value to be converted.
     *  @return The absolute value of \a pValue.
     */
    INLINE Double   Abs( Double pValue )    {   return fabs( pValue );  }

    /**
     *  Obtain the square root of an integer.
     *  @brief  Obtain the square root of an integer.
     *  @param  pValue  an integer, the number we want the square root.
     *  @return The square root of \a pValue, as a float.
     */
    INLINE Float    Sqrt( Int32 pValue )        {   return sqrtf( (Float)pValue );  }

    /**
     *  Obtain the square root of a float.
     *  @brief  Obtain the square root of a float.
     *  @param  pValue  a float, the number we want the square root.
     *  @return The square root of \a fValue, as a float.
     */
    INLINE Float    Sqrt( Float pValue )    {   return sqrtf( pValue ); }

    /**
     *  Obtain the square root of a double.
     *  @brief  Obtain the square root of a double.
     *  @param  pValue  a double, the number we want the square root.
     *  @return The square root of \a fValue, as a double.
     */
    INLINE Double   Sqrt( Double pValue )   {   return sqrt( pValue );  }

    /**
     *  Obtain the approximated inverse of the square root of a float.
     *  @brief  This code compute a fast 1 / sqrtf(v) approximation.
     *  @note   Originaly from Matthew Jones (Infogrames).
     *  @param  pValue  a float, the number we want the square root.
     *  @return The square root of \a fValue, as a float.
     */
    INLINE Float FastRSqrt( Float pValue )    
    {  
        union FloatInt
        {
            float f;
            int   i;
        };
        FloatInt floatInt;
        floatInt.f = pValue;
        
        const Int32 MAGIC_NUMBER = 0x5f3759df;
        
        
        Float v_half = pValue * 0.5f;
        floatInt.i = MAGIC_NUMBER - (floatInt.i >> 1);
        pValue = floatInt.f;
        return pValue * (1.5f - v_half * pValue * pValue);
    }

    /**
     *  Obtain the square of a float
     */
    INLINE Float    Square( Float pValue )  {   return pValue * pValue; }

    /**
     *  Obtain the square of a double
     */
    INLINE Double   Square( Double pValue ) {   return pValue * pValue; }

	/** 
	 *	Returns base raised to the power of exp
	 */
	INLINE Float	Pow(Float base, Float exp)		{	return pow(base, exp);		}

	/** 
	 *	Returns base raised to the power of exp
	 */
	INLINE Double	Pow(Double base, Double exp)	{	return pow(base, exp);		}

    /**
     *  Clamp the specified value between a minimum and a maximum.
     *  @brief  Clamp the specified value between a minimum and a maximum.
     *  @param  Value   the variable we want to be clamped.
     *  @param  Min     minimum possible value.
     *  @param  Max     maximum possible value.
     *  @return A reference to Value.
     */
    template <class T> INLINE T& Clamp( T& pValue, const T& pMin, const T& pMax )
    {
        pValue = (pValue < pMin) ? pMin : ((pValue > pMax) ? pMax : pValue);
        return pValue;
    }

    template <class VAL, class TIME> INLINE VAL Interpolate( const VAL& pValueStart, const VAL& pValueEnd, const TIME& pTime )
    {
        return (pValueStart * (TIME(1) - pTime)) + (pValueEnd * pTime);
    }

}   // END namespace Maths


} // namespace Gamedesk


#endif  //  _MATHS_H_
