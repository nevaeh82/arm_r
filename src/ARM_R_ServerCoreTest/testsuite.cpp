/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#define _CXXTEST_ABORT_TEST_ON_FAIL
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "testDBIB.h"

static testDBIB suite_testDBIB;

static CxxTest::List Tests_testDBIB = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_testDBIB( "testDBIB.h", 10, "testDBIB", suite_testDBIB, Tests_testDBIB );

static class TestDescription_testDBIB_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_testDBIB_test1() : CxxTest::RealTestDescription( Tests_testDBIB, suiteDescription_testDBIB, 13, "test1" ) {}
 void runTest() { suite_testDBIB.test1(); }
} testDescription_testDBIB_test1;

#include <cxxtest/Root.cpp>
