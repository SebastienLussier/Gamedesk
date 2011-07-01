/**
 *  @file       Number.h
 *  @brief      Number class for the integers, single and double precision floating points.
 *  @author     Sebastien Lussier.
 *  @date       27/03/04.
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
#ifndef     _NUMBER_H_
#define     _NUMBER_H_


namespace Gamedesk {
	

template <class T>
class Number
{
public:
    static const T Min = 0;
    static const T Max = 0;
    inline static const Char* Name() { return ""; }
};


template <> 
class Number<Char>
{
public:
    static const Char Min = -128;
    static const Char Max = 0x7F;
    inline static const Char* Name() { return "Char"; }
};

template <> 
class Number<Byte>
{
public:
    static const Byte Min = 0x00;
    static const Byte Max = 0xFF;
    inline static const Char* Name() { return "Byte"; }
};

template <> 
class Number<Int16>
{
public:
    static const Int16 Min = -32768;
    static const Int16 Max = 0x7FFF;
    inline static const Char* Name() { return "Int16"; }
};

template <> 
class Number<UInt16>
{
public:
    static const UInt16 Min = 0x0000;
    static const UInt16 Max = 0xFFFF;
    inline static const Char* Name() { return "UInt16"; }
};

template <> 
class Number<Int32>
{
public:
    static const Int32 Min = (Int32)0x80000000;
    static const Int32 Max = 0x7FFFFFFF;
    inline static const Char* Name() { return "Int32"; }
};

template <> 
class Number<UInt32>
{
public:
    static const UInt32 Min = 0x00000000;
    static const UInt32 Max = 0xFFFFFFFF;
    inline static const Char* Name() { return "UInt32"; }
};

template <> 
class Number<Int64>
{
public:
    static const Int64 Min = (Int64)0x8000000000000000LL;
    static const Int64 Max = 0x7FFFFFFFFFFFFFFFLL;
    inline static const Char* Name() { return "Int64"; }
};

template <> 
class Number<UInt64>
{
public:
    static const UInt64 Min = 0x0000000000000000ULL;
    static const UInt64 Max = 0xFFFFFFFFFFFFFFFFULL;
    inline static const Char* Name() { return "UInt64"; }
};

template <> 
class CORE_API Number<Float>
{
public:
    static const Float Min;
    static const Float Max;
    inline static const Char* Name() { return "Float"; }
};

template <> 
class CORE_API Number<Double>
{
public:
    static const Double Min;
    static const Double Max;
    inline static const Char* Name() { return "Double"; }
};



template <class T>
class NumberValidator
{
public:
    NumberValidator() :
        mMin(Number<T>::Min), 
        mMax(Number<T>::Max)        
    {
    }

    NumberValidator( T pMin, T pMax ) :
        mMin( pMin ),
        mMax( pMax )
    {
    }

    void SetMin( T pMin )
    {
        mMin = pMin;
    }

    T GetMin() const
    {
        return mMin;
    }

    void SetMax( T pMax )
    {
        mMax = pMax;
    }

    T GetMax() const
    {
        return mMax;
    }

    Bool Validate( T pValue )
    {
        return pValue >= mMin && pValue <= mMax;
    }

    Bool ConvertAndValidate( const String& pString, T& pVal )
    {
        if( ::StringTo( pString, pVal ) )
            return Validate( pVal );
        
        return false;
    }    

private:
    T mMin;
    T mMax;    
};


} // namespace Gamedesk


#endif  //  _NUMBER_H_
