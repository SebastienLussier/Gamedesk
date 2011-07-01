/**
 *  @file       TestRunner.cpp
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
#include <qapplication.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3multilineedit.h>
#include <qprogbar.h>
#include <q3filedialog.h> 
#include <qmessagebox.h>

#include "TestRunner.h"
#include "Module/ModuleManager.h"


TestRunner::TestRunner() : testrunnerDialogBase( NULL, NULL, true ),
    mTestWorker(NULL)
{
    // Connect buttons signals to slots
    connect( mBtnRunTests,      SIGNAL(clicked()),  this,   SLOT(OnRunTests())        );
    connect( mBtnBrowseModule,  SIGNAL(clicked()),  this,   SLOT(OnBrowseModule())    );
    connect( mBtnExit,          SIGNAL(clicked()),  this,   SLOT(OnExit())            );

    mLblModuleName->setText( "UnitTests" );
    mLblOutput->setReadOnly(true);
}

TestRunner::~TestRunner()
{
    if( mTestWorker )
        GD_DELETE(mTestWorker);
}

void TestRunner::OnBrowseModule()
{
    try
    {
        ModuleManager::Instance()->LoadModule( (const Char*)mLblModuleName->text() );
        QMessageBox::information( this, "Module Registration", "Module registered successfully.", "Ok" );
    }
    catch( Exception& e )
    {
        QMessageBox::warning( this, "Module Registration", e.GetMessage().c_str(), "Ok" );
    }
}

void TestRunner::OnRunTests()
{
    if( mTestWorker )
        delete mTestWorker;

    mTestWorker = new TestWorker( this );
    mTestWorker->start();
}

void TestRunner::OnExit()
{
    accept();
}


TestWorker::TestWorker( TestRunner* pTestRunner ) :
    mTest( "TestRunner" ),
    mTestRunner( pTestRunner )
{
    mTestResults.AddTestListener( this );

    Class::Iterator itClasses( TestCase::StaticClass() );
    for( ; itClasses.IsValid(); ++itClasses )
        mTest.AddTestCase( *itClasses );

    mTestRunner->mLblNbRuns->setText( "0" );
    mTestRunner->mLblNbFailures->setText( "0" );
    mTestRunner->mLblNbErrors->setText( "0" );
    mTestRunner->mProgressBar->setTotalSteps( mTest.GetTestCaseCount() );
    mTestRunner->mProgressBar->setProgress( 0 );
}


void TestWorker::run()
{
	Core::DebugOut( "\n-------------------- Running Test --------------------\n" );
    
    mTestRunner->mProgressBar->setTotalSteps( mTest.GetTestCaseCount() );
    mTestRunner->mProgressBar->setProgress( 0 );
    
    mTestRunner->setCursor( waitCursor );
    mTestRunner->mBtnRunTests->setDisabled(true);
    mTestRunner->mBtnBrowseModule->setDisabled(true);
    mTestRunner->mBtnExit->setDisabled(true);

    mTest.Run(mTestResults);

    mTestRunner->mBtnRunTests->setDisabled(false);
    mTestRunner->mBtnBrowseModule->setDisabled(false);
    mTestRunner->mBtnExit->setDisabled(false);
    mTestRunner->setCursor( arrowCursor );

    Core::DebugOut( "\nDone!\n\n" );
}


void TestWorker::UpdateInfo( const TestResult& pResults )
{
    UInt32 nbFailures = pResults.GetFailureCount();
    UInt32 nbErrors = pResults.GetErrorCount();
    UInt32 nbRuns = nbFailures + nbErrors + pResults.GetPassedCount();

    mTestRunner->mLblNbRuns->    setText( ToString( nbRuns ).c_str()     );
    mTestRunner->mLblNbFailures->setText( ToString( nbFailures ).c_str() );
    mTestRunner->mLblNbErrors->  setText( ToString( nbErrors ).c_str()   );

    mTestRunner->mProgressBar->setProgress( nbRuns );
}

void TestWorker::OnTestPassed( const TestResult& pResults, const String& pTestName )
{
    String  outputText;

    outputText += "Test passed: ";
    outputText += pTestName;
    outputText += ".";

    mTestRunner->mLblOutput->insertLine( outputText.c_str() );
    
	Core::DebugOut( "%s\n", outputText.c_str() );
        
    UpdateInfo( pResults );
}

void TestWorker::OnTestError( const TestResult& pResults, const TestError& pTestError )
{
    String  outputText;

    outputText += pTestError.mLocation.File();
    outputText += "(";
    outputText += ToString( pTestError.mLocation.Line() );
    outputText += ") : ";
    outputText += "error in test \"";
    outputText += pTestError.mTestName;
    outputText += "\": ";
    outputText += pTestError.mErrorMsg;

    mTestRunner->mLblOutput->insertLine( outputText.c_str() );
	Core::DebugOut( "%s\n", outputText.c_str() );

	UpdateInfo( pResults );
}

void TestWorker::OnTestFailure( const TestResult& pResults, const TestFailure& pTestFailure )
{
    String  outputText;

    outputText += pTestFailure.mLocation.File();
    outputText += "(";
    outputText += ToString( pTestFailure.mLocation.Line() );
    outputText += ") : ";
    outputText += "failure in test \"";
    outputText += pTestFailure.mTestName;
    outputText += "\": ";
    outputText += pTestFailure.mCond;

    mTestRunner->mLblOutput->insertLine( outputText.c_str() );
	Core::DebugOut( "%s\n", outputText.c_str() );

	UpdateInfo( pResults );
}
