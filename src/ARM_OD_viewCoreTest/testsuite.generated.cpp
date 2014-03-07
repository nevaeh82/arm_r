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
#include "testdb.h"

static testPhoto suite_testPhoto;

static CxxTest::List Tests_testPhoto = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_testPhoto( "testdb.h", 14, "testPhoto", suite_testPhoto, Tests_testPhoto );

static class TestDescription_testPhoto_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_testPhoto_test1() : CxxTest::RealTestDescription( Tests_testPhoto, suiteDescription_testPhoto, 18, "test1" ) {}
 void runTest() { suite_testPhoto.test1(); }
} testDescription_testPhoto_test1;

#include <cxxtest/Root.cpp>
