/**
 *  @file       TestCase.h
 *  @brief      Declaration of the TestCase class.
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
#ifndef     _TESTCASE_H_
#define     _TESTCASE_H_


namespace Gamedesk {


/**
 *  A test case defines the fixture to run multiple tests. To define a test case
 *  1) Implement a subclass of TestCase
 *  2) Define instance variables that store the state of the fixture
 *  3) Initialize the fixture state by overriding SetUp
 *  4) Clean-up after a test by overriding TearDown.
 *  Each test runs in its own fixture so there can be no side effects among test runs.
 *  @brief A test case used for automatic testing of the Gamedesk SDK.
 */
class CORE_API TestCase : public Object
{
    DECLARE_ABSTRACT_CLASS( TestCase, Object );

public:
    TestCase();
    TestCase( const String& pTestName );
    
    //! Gets the name of a TestCase.
    String GetName() const;

    //! Sets the name of a TestCase.
    void SetName( const String& pTestName );
    
    //! Sets up the fixture, for example, open a network connection.
    virtual void SetUp();
        
    //! Runs the test case.
    virtual void Run() = 0;    

    //! Tears down the fixture, for example, close a network connection.
    virtual void TearDown();

private:
    String  mTestName;      //!< Name of the test.
};


/**
 *  Exception throwed when an assertion fail in a test case. It's message 
 *  contain the condition that failed, as a string.
 *  @brief  Exception used when an assertion fail in a test case.
 *  @author Sébastien Lussier
 */
class CORE_API TestFailureException : public Exception
{
    DECLARE_EXCEPTION(TestFailureException);

public:
    /**
     *  Constructor.
     *  @param  pFailure    Condition that failed (as a string).
     *  @param  pLoc        Location where the exception was thrown.
     */
    TestFailureException( const Char* pFailure, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = pFailure;
        DebugOut();
    }
};


//! Macro to be used to verify results of a test assertion.
#define TestAssert( cond ) if( !(cond) ) throw TestFailureException( #cond, Here );


} // namespace Gamedesk


#endif  //  _TESTCASE_H_
