/**
 *  @file       Test.h
 *  @brief      Declaration of classes needed for Test.
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
#ifndef     _TEST_H_
#define     _TEST_H_


#include "TestCase.h"
#include "TestResult.h"


namespace Gamedesk {


/**
 *  A Test can be run and its results collected through a TestResult object.
 *  @brief Used to execute a number of test cases.
 *  @author Sébastien Lussier
 */
class CORE_API Test
{
public:
    /**
     *  Constructor.
     *  @param  pTestName   Name of the test.
     */
    Test( const String& pTestName = "" );

    /**
     *  Add a test case 
     *  @param  pTestCaseClass  A class object derived from TestCase.
     */
    void AddTestCase( Class* pTestCaseClass );

    /**
     *  Counts the number of test cases that will be run by this test. 
     *  @return Number of tests cases.
     */
    UInt32 GetTestCaseCount() const;

    /**
     *  Run through all test cases and collect the results.
     *  @param  pTestResult     Will contain the result of the test execution.
     */
    void Run( TestResult& pTestResult );

private:
    String          mTestName;          //!<    Name of the test.
    Vector<Class*>  mTestCases;         //!<    List of class derived from TestCase.
};


} // namespace Gamedesk


#endif  //  _TEST_H_
