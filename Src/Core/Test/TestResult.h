/**
 *  @file       TestResult.h
 *  @brief      Declarations for the TestResult class.
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
#ifndef     _TESTRESULT_H_
#define     _TESTRESULT_H_


namespace Gamedesk {


//!  Result of a test failure.
class CORE_API TestFailure
{
public:
    TestFailure( const String& pTestName, CodeLocation pLocation, const String& pCond );   
    TestFailure( const TestFailure& pOther );
   
    String          mTestName;
    CodeLocation    mLocation;
    String          mCond;
};


//!  Result of a test error.
class CORE_API TestError
{
public:
    TestError( const String& pTestName, CodeLocation pLocation, const String& pErrorMsg );
    TestError( const TestError& pOther );

    String          mTestName;
    CodeLocation    mLocation;
    String          mErrorMsg;
};


class TestResult;


//!  Result of a test failure.
class CORE_API TestListener
{
public:
    //! Method called when a test is passed.
    virtual void OnTestPassed   ( const TestResult& pResults, const String& pTestName         ) = 0;

    //! Method called when an error (unexpected result).
    virtual void OnTestError    ( const TestResult& pResults, const TestError& pTestError     ) = 0;

    //! Method called when a test assertion failed.
    virtual void OnTestFailure  ( const TestResult& pResults, const TestFailure& pTestFailure ) = 0;
};


/**
 * A TestResult collects the results of executing a test case. The test 
 * framework distinguishes between failures and errors. A failure is 
 * anticipated and checked for with assertions. Errors are unanticipated 
 * problems like an Exception. 
 */
class CORE_API TestResult
{
public:
    /**
     *  Constructor.
     *  @param  pTestName   Name of this test result.
     */
    TestResult( const String& pTestName = "" );

    /**
     *  Add a listener to this test result.
     *  @param pTestListener    Listener object that will receive notifications.
     */
    void AddTestListener( TestListener* pTestListener );

    /**
     *  Add a successful test to this test result.
     *  @param  pTestName   Name of the test class successfully executed.
     */
    void AddPassed( const String& pTestName );    

    /**
     *  Add an erroneous test to this result.
     *  @param  pError      TestError object.
     */
    void AddError( const TestError& pError );

    /**
     *  Add a failure test to this result.
     *  @param  pFailure    TestFailure object.
     */
    void AddFailure( const TestFailure& pFailure );

    //! Return the number of passed tests.
    UInt32 GetPassedCount() const;

    //! Return the list of passed tests classes names.
    Vector<String> GetPassed() const;

    //! Return the number of erroneous tests.
    UInt32 GetErrorCount() const;

    //! Return the list of error objects.
    Vector<TestError> GetErrors() const;

    //! Return the number of failed tests.
    UInt32 GetFailureCount() const;

    //! Return the list of failure objects.
    Vector<TestFailure> GetFailures() const;
    
    /**
     *  If no error or failure occured, then the test result is successful.
     *  @return true if the test was successful, false otherwise.
     */
    Bool WasSuccessful() const;

private:
    //@{
    //! Fire events to notify listeners of changes.
    void FireOnTestPassed( const String& pTestName ) const;
    void FireOnTestError( const TestError& pError ) const;
    void FireOnTestFailure( const TestFailure& pFailure ) const;
    //@}
    
private:
    String                  mTestName;          //!< Name of the test.

    Vector<String>          mPassed;            //!< List of passed tests classes names.
    Vector<TestError>       mErrors;            //!< List of error objects.
    Vector<TestFailure>     mFailures;          //!< List of failure objects.
    
    Vector<TestListener*>   mTestListeners;     //!< Listeners that will be notified of changes to this test result object.
};


} // namespace Gamedesk


#endif  //  _TESTRESULT_H_
