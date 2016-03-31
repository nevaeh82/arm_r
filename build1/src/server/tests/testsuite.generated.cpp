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
#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/server/tests/ClientServerTest.h"

static ClientServerTest suite_ClientServerTest;

static CxxTest::List Tests_ClientServerTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ClientServerTest( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/server/tests/ClientServerTest.h", 17, "ClientServerTest", suite_ClientServerTest, Tests_ClientServerTest );

static class TestDescription_ClientServerTest_testServerSendAnyDataWhenNoClients : public CxxTest::RealTestDescription {
public:
 TestDescription_ClientServerTest_testServerSendAnyDataWhenNoClients() : CxxTest::RealTestDescription( Tests_ClientServerTest, suiteDescription_ClientServerTest, 65, "testServerSendAnyDataWhenNoClients" ) {}
 void runTest() { suite_ClientServerTest.testServerSendAnyDataWhenNoClients(); }
} testDescription_ClientServerTest_testServerSendAnyDataWhenNoClients;

static class TestDescription_ClientServerTest_testConnectToClientServerAndSendAnyData : public CxxTest::RealTestDescription {
public:
 TestDescription_ClientServerTest_testConnectToClientServerAndSendAnyData() : CxxTest::RealTestDescription( Tests_ClientServerTest, suiteDescription_ClientServerTest, 74, "testConnectToClientServerAndSendAnyData" ) {}
 void runTest() { suite_ClientServerTest.testConnectToClientServerAndSendAnyData(); }
} testDescription_ClientServerTest_testConnectToClientServerAndSendAnyData;

static class TestDescription_ClientServerTest_testClientServerOnMessageReceive : public CxxTest::RealTestDescription {
public:
 TestDescription_ClientServerTest_testClientServerOnMessageReceive() : CxxTest::RealTestDescription( Tests_ClientServerTest, suiteDescription_ClientServerTest, 90, "testClientServerOnMessageReceive" ) {}
 void runTest() { suite_ClientServerTest.testClientServerOnMessageReceive(); }
} testDescription_ClientServerTest_testClientServerOnMessageReceive;

#include <cxxtest/Root.cpp>
