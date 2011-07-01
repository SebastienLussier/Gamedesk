/**
 *  @file       TestRunner.h
 *  @brief      Classes needed to execute a Test containing TestCase and get a graphical result in a dialog.
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
#ifndef     _TESTRUNNER_H_
#define     _TESTRUNNER_H_


#include <qthread.h>

#include "Engine.h"
#include "Test/Test.h"
#include "Test/TestResult.h"

#include "TestRunnerDialogBase.h"



/**
 *  Qt dialog class used to run test cases and get their results. The user can
 *  also load specific test modules for more flexibility.
 *  @brief   Dialog class used to run tests.
 *  @author  Sébastien Lussier
 *  @date    24/10/03
 */
class TestRunner : public testrunnerDialogBase
{
    Q_OBJECT;

public:
    //! Constructor.
    TestRunner();

    //! Destructor.
    virtual ~TestRunner();

public slots:
    //@{
    //! Dialog event responses.
    void OnRunTests();
    void OnBrowseModule();
    void OnExit();
    //@}

private:
    class TestWorker*     mTestWorker;      //!<    Worker thread that will run the tests.
};


/**
 *  Qt thread used to run the test.  This class is also a TestListener so that 
 *  it can receive events from the TestResult object when a TestCase has been run.
 *  @brief  Worker thread running the tests.
 *  @author Sébastien Lussier
 *  @date   24/10/03
 */
class TestWorker : public QThread, public TestListener
{
public:
    /**
     *  Constructor.
     *  @param  pTestRunner     Qt dialog that will need to be refreshed.
     */
    TestWorker( TestRunner* pTestRunner );

    //! Virtual method called by QThread to start the thread execution, returning from this method will end execution of the thread.
    virtual void run();

    //! Update general info after a test case execution.
    void UpdateInfo( const TestResult& pResults );

    //@{
    //! Methods that must be reimplemented from TestListener.
    virtual void OnTestPassed   ( const TestResult& pResults, const String& pTestName         );
    virtual void OnTestError    ( const TestResult& pResults, const TestError& pTestError     );
    virtual void OnTestFailure  ( const TestResult& pResults, const TestFailure& pTestFailure );
    //@}

private:
    Test          mTest;            //!< Actual test that will contain all TestCase to be executed.
    TestResult    mTestResults;     //!< Tests results.
    TestRunner*   mTestRunner;      //!< The dialog associated to this thread.
};


#endif  //  _TESTRUNNER_H_
