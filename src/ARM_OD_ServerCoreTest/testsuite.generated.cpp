/* Generated file, do not edit */

#define CXXTEST_HAVE_EH
#define CXXTEST_ABORT_TEST_ON_FAIL
#include "../TestShared/cxxtest/VerbosePrinter.h"
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
#include <stdlib.h>
#include <stdio.h>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QCoreApplication>

//#include <PwLogger/PwLogger.h>
#include "../TestShared/QApplicationRunner.h"
//#include <LoggerHelper.h>
//#define USE_QAPPLICATION
#define USE_QCOREAPPLICATION

QMutex mutex;

int main()
{
#ifdef USE_QCOREAPPLICATION
    int argc = 1;
    char *argv[] = { (char*)"app"};
    QCoreApplication testApp(argc,argv);
#endif
    //Pw::Gis::Common::LoggerHelper::instance()->configureLogger(QString(ProjectName ".log4qt"));
    int status = CxxTest::VerbosePrinter().run();
   // int status = CxxTest::ErrorPrinter().run();
#ifdef USE_QCOREAPPLICATION
    testApp.exit(0);
#endif
    return status;
}

// The CxxTest "world"
#include "test.h"

static test suite_test;

static CxxTest::List Tests_test = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_test( "test.h", 7, "test", suite_test, Tests_test );

static class TestDescription_test_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_test_test1() : CxxTest::RealTestDescription( Tests_test, suiteDescription_test, 10, "test1" ) {}
 void runTest() { suite_test.test1(); }
} testDescription_test_test1;

#include <cxxtest/Root.cpp>
