/**
 *  @file       TestString.cpp
 *  @brief      Tests for the String class.
 *  @author     Sébastien Lussier.
 *  @date       27/10/03.
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
#include "UnitTests.h"
#include "Test/TestCase.h"


class UNITTESTS_API StringConversionTest : public TestCase
{
    DECLARE_CLASS( StringConversionTest, TestCase );

public:
    StringConversionTest()
    {
    }
    
    virtual void Run()
    {
        Bool    aBool   = false;
        Float   aFloat  = 123.45f;
        Double  aDouble = 678.987654321;
        Char    aChar   = 'a';
        Int16   aInt16  = -32000;
        Int32   aInt32  = -2000000000;
        Int64   aInt64  = -1234567890000;
        Byte    aByte   = 255;
        UInt16  aUInt16 = 65535;
        UInt32  aUInt32 = 4200000000;
        UInt64  aUInt64 = 1234567890000;

        String  tBool   = ToString( aBool );
        String  tFloat  = ToString( aFloat, 5 );
        String  tDouble = ToString( aDouble, 12 );
        String  tChar   = ToString( aChar );
        String  tInt16  = ToString( aInt16 );
        String  tInt32  = ToString( aInt32 );
        String  tInt64  = ToString( aInt64 );
        String  tByte   = ToString( aByte );
        String  tUInt16 = ToString( aUInt16 );
        String  tUInt32 = ToString( aUInt32 );
        String  tUInt64 = ToString( aUInt64 );

        TestAssert( aBool   == StringTo<Bool>   ( tBool )   );
        TestAssert( aFloat  == StringTo<Float>  ( tFloat )  );
        TestAssert( aDouble == StringTo<Double> ( tDouble ) );
        TestAssert( aChar   == StringTo<Char>   ( tChar )   );
        TestAssert( aInt16  == StringTo<Int16>  ( tInt16 )  );
        TestAssert( aInt32  == StringTo<Int32>  ( tInt32 )  );
        TestAssert( aInt64  == StringTo<Int64>  ( tInt64 )  );
        TestAssert( aByte   == StringTo<Byte>   ( tByte )   );
        TestAssert( aUInt16 == StringTo<UInt16> ( tUInt16 ) );
        TestAssert( aUInt32 == StringTo<UInt32> ( tUInt32 ) );
        TestAssert( aUInt64 == StringTo<UInt64> ( tUInt64 ) );
    }
};

IMPLEMENT_CLASS( StringConversionTest );
