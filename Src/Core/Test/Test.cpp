/**
 *  @file       Test.cpp
 *  @brief      Definition of classes needed for Test.
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
#include "Test.h"


namespace Gamedesk {
	
	
///////////////////////////////////////////////////////////////////////////////
// Test
Test::Test( const String& pTestName ) :
    mTestName( pTestName )
{
}

void Test::AddTestCase( Class* pTestCaseClass )
{
    mTestCases.push_back(pTestCaseClass);
}

UInt32 Test::GetTestCaseCount() const
{
    return mTestCases.size();
}

void Test::Run( TestResult& pResults )
{
    Vector<Class*>::iterator    itClasses;

    for( itClasses = mTestCases.begin(); itClasses != mTestCases.end(); ++itClasses )
    {
        TestCase*   testCase = NULL;

        try
        {
            testCase = Cast<TestCase>((*itClasses)->AllocateNew());

            testCase->SetUp();
            testCase->Run();
            testCase->TearDown();

            pResults.AddPassed( testCase->GetClass()->GetName() );

            GD_DELETE(testCase);
        }
        catch( TestFailureException& e )
        {
            TestFailure failure( testCase->GetClass()->GetName(), e.GetLocation(), e.GetMessage() );
            pResults.AddFailure( failure );

            if( testCase )
                GD_DELETE(testCase);
        }
        catch( Exception& e )
        {
            TestError   error( testCase->GetClass()->GetName(), e.GetLocation(), e.GetMessage() );
            pResults.AddError( error );

            if( testCase )
                GD_DELETE(testCase);
        }
    }
}


} // namespace Gamedesk
