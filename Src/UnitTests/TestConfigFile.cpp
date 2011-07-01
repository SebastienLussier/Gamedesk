/**
 *  @file       TestConfigFile.cpp
 *  @brief      Tests for the ConfigFile class.
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
#include "Config/ConfigFile.h"
#include "FileManager/FileManager.h"


class UNITTESTS_API ConfigFileTest : public TestCase
{
    DECLARE_CLASS( ConfigFileTest, TestCase );

public:
    ConfigFileTest() :
        mBool(true),
        mInt32(101),
        mFloat(99.856f),
        mSimpleString("String"),
        mComplexString("A complex String.")
    {
    }

    virtual void SetUp()
    {
        ConfigFile configSave( "TestConfigFile.ini" );
        
        configSave["Section 1"]["Boolean"      ] = mBool;
        configSave["Section 1"]["Integer"      ] = mInt32;
        configSave["Section 1"]["Float"        ] = mFloat;
        configSave["Section 1"]["SimpleString" ] = mSimpleString;
        configSave["Section 1"]["ComplexString"] = mComplexString;
        configSave["Section 2"]["Boolean"      ] = mBool;
        configSave["Section 2"]["SimpleString" ] = mSimpleString;

        configSave.Save();
    }

    virtual void Run()
    {
        ConfigFile configLoad( "TestConfigFile.ini" );

        configLoad.Load();

        TestAssert( ((Bool)  configLoad["Section 1"]["Boolean"      ]) == mBool         );
        TestAssert( ((Int32) configLoad["Section 1"]["Integer"      ]) == mInt32         );
        TestAssert( ((Float) configLoad["Section 1"]["Float"        ]) == mFloat         );
        TestAssert( ((String)configLoad["Section 1"]["SimpleString" ]) == mSimpleString  );
        TestAssert( ((String)configLoad["Section 1"]["ComplexString"]) == mComplexString );
        TestAssert( ((Bool)  configLoad["Section 2"]["Boolean"      ]) == mBool          );
        TestAssert( ((String)configLoad["Section 2"]["SimpleString" ]) == mSimpleString  );
    }

    virtual void TearDown()
    {
        FileManager::DeleteFile( "config/TestConfigFile.ini" );
    }

private:
    const Bool    mBool;
    const Int32   mInt32;
    const Float   mFloat;
    const String  mSimpleString;
    const String  mComplexString;
};

IMPLEMENT_CLASS( ConfigFileTest );
