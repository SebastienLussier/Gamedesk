/**
 *  @file       TestResult.cpp
 *  @brief      Definition for the TestResult class.
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
#include "TestResult.h"


namespace Gamedesk {
	
	
///////////////////////////////////////////////////////////////////////////////
// TestFailure
TestFailure::TestFailure( const String& pTestName, CodeLocation pLocation, const String& pCond ) :
    mTestName(pTestName), mLocation(pLocation), mCond(pCond)
{
}

TestFailure::TestFailure( const TestFailure& pOther ) :
    mTestName(pOther.mTestName), mLocation(pOther.mLocation), mCond(pOther.mCond)
{
}
    

///////////////////////////////////////////////////////////////////////////////
// TestError
TestError::TestError( const String& pTestName, CodeLocation pLocation, const String& pErrorMsg ) :
    mTestName(pTestName), mLocation(pLocation), mErrorMsg(pErrorMsg)
{
}

TestError::TestError( const TestError& pOther ) :
    mTestName(pOther.mTestName), mLocation(pOther.mLocation), mErrorMsg(pOther.mErrorMsg)
{
}
   

///////////////////////////////////////////////////////////////////////////////
// TestResult
TestResult::TestResult( const String& pTestName )
{
    mTestName = pTestName;
}

void TestResult::AddPassed( const String& pTestName )
{
    mPassed.push_back( pTestName );
    FireOnTestPassed( pTestName );
}

void TestResult::AddError( const TestError& pError )
{
    mErrors.push_back( pError );
    FireOnTestError( pError );
}

void TestResult::AddFailure( const TestFailure& pFailure )
{
    mFailures.push_back( pFailure );
    FireOnTestFailure( pFailure );
}

UInt32 TestResult::GetPassedCount() const
{
    return mPassed.size();
}

Vector<String> TestResult::GetPassed() const
{
    return mPassed;
}

UInt32 TestResult::GetErrorCount() const
{
    return mErrors.size();
}

Vector<TestError> TestResult::GetErrors() const
{
    return mErrors;
}

UInt32 TestResult::GetFailureCount() const
{
    return mFailures.size();
}

Vector<TestFailure> TestResult::GetFailures() const
{
    return mFailures;
}

Bool TestResult::WasSuccessful() const
{
    return GetErrorCount() == 0 && GetFailureCount() == 0;
}

void TestResult::AddTestListener( TestListener* pTestListener )
{
    mTestListeners.push_back( pTestListener );
}

void TestResult::FireOnTestPassed( const String& pText ) const
{
    Vector<TestListener*>::const_iterator   itListeners;
    TestListener*                           listener = NULL;
    
    for( itListeners = mTestListeners.begin(); itListeners != mTestListeners.end(); ++itListeners )
    {
        listener = (*itListeners);
        listener->OnTestPassed( *this, pText );
    }
}

void TestResult::FireOnTestError( const TestError& pTestError ) const
{
    Vector<TestListener*>::const_iterator   itListeners;
    TestListener*                           listener = NULL;
    
    for( itListeners = mTestListeners.begin(); itListeners != mTestListeners.end(); ++itListeners )
    {
        listener = (*itListeners);
        listener->OnTestError( *this, pTestError );
    }
}

void TestResult::FireOnTestFailure( const TestFailure& pTestFailure ) const
{
    Vector<TestListener*>::const_iterator   itListeners;
    TestListener*                           listener = NULL;
    
    for( itListeners = mTestListeners.begin(); itListeners != mTestListeners.end(); ++itListeners )
    {
        listener = (*itListeners);
        listener->OnTestFailure( *this, pTestFailure );
    }
}


} // namespace Gamedesk
