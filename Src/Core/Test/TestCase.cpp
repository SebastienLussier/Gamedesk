/**
 *  @file       TestCase.cpp
 *  @brief      Definition of the TestCase class.
 *  @author     Sébastien Lussier.
 *  @date       22/10/03.
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
#include "TestCase.h"


namespace Gamedesk {
	
	
///////////////////////////////////////////////////////////////////////////////
// TestCase
IMPLEMENT_ABSTRACT_CLASS( TestCase );

TestCase::TestCase()  {}
TestCase::TestCase( const String& pTestName )
{
    mTestName = pTestName;
}

String TestCase::GetName() const
{
    return mTestName;
}

void TestCase::SetName( const String& pTestName )
{
    mTestName = pTestName;
}

void TestCase::SetUp() {}
void TestCase::TearDown() {}


} // namespace Gamedesk
