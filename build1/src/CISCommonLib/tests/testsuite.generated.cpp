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
#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestBiMap.h"

static TestBiMap suite_TestBiMap;

static CxxTest::List Tests_TestBiMap = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestBiMap( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestBiMap.h", 7, "TestBiMap", suite_TestBiMap, Tests_TestBiMap );

static class TestDescription_TestBiMap_testInsertAndSize : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testInsertAndSize() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 13, "testInsertAndSize" ) {}
 void runTest() { suite_TestBiMap.testInsertAndSize(); }
} testDescription_TestBiMap_testInsertAndSize;

static class TestDescription_TestBiMap_testInsertFailed : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testInsertFailed() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 29, "testInsertFailed" ) {}
 void runTest() { suite_TestBiMap.testInsertFailed(); }
} testDescription_TestBiMap_testInsertFailed;

static class TestDescription_TestBiMap_testClear : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testClear() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 56, "testClear" ) {}
 void runTest() { suite_TestBiMap.testClear(); }
} testDescription_TestBiMap_testClear;

static class TestDescription_TestBiMap_testIsEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testIsEmpty() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 71, "testIsEmpty" ) {}
 void runTest() { suite_TestBiMap.testIsEmpty(); }
} testDescription_TestBiMap_testIsEmpty;

static class TestDescription_TestBiMap_testIsNotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testIsNotEmpty() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 82, "testIsNotEmpty" ) {}
 void runTest() { suite_TestBiMap.testIsNotEmpty(); }
} testDescription_TestBiMap_testIsNotEmpty;

static class TestDescription_TestBiMap_testContains : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testContains() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 97, "testContains" ) {}
 void runTest() { suite_TestBiMap.testContains(); }
} testDescription_TestBiMap_testContains;

static class TestDescription_TestBiMap_testRemoveByLeft : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testRemoveByLeft() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 120, "testRemoveByLeft" ) {}
 void runTest() { suite_TestBiMap.testRemoveByLeft(); }
} testDescription_TestBiMap_testRemoveByLeft;

static class TestDescription_TestBiMap_testRemoveByRight : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testRemoveByRight() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 136, "testRemoveByRight" ) {}
 void runTest() { suite_TestBiMap.testRemoveByRight(); }
} testDescription_TestBiMap_testRemoveByRight;

static class TestDescription_TestBiMap_testRemoveNonexistent : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testRemoveNonexistent() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 152, "testRemoveNonexistent" ) {}
 void runTest() { suite_TestBiMap.testRemoveNonexistent(); }
} testDescription_TestBiMap_testRemoveNonexistent;

static class TestDescription_TestBiMap_testLeft : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testLeft() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 171, "testLeft" ) {}
 void runTest() { suite_TestBiMap.testLeft(); }
} testDescription_TestBiMap_testLeft;

static class TestDescription_TestBiMap_testRight : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testRight() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 186, "testRight" ) {}
 void runTest() { suite_TestBiMap.testRight(); }
} testDescription_TestBiMap_testRight;

static class TestDescription_TestBiMap_testGetNotExistsValue : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testGetNotExistsValue() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 201, "testGetNotExistsValue" ) {}
 void runTest() { suite_TestBiMap.testGetNotExistsValue(); }
} testDescription_TestBiMap_testGetNotExistsValue;

static class TestDescription_TestBiMap_testKeys : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testKeys() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 213, "testKeys" ) {}
 void runTest() { suite_TestBiMap.testKeys(); }
} testDescription_TestBiMap_testKeys;

static class TestDescription_TestBiMap_testOperationEquals : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testOperationEquals() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 237, "testOperationEquals" ) {}
 void runTest() { suite_TestBiMap.testOperationEquals(); }
} testDescription_TestBiMap_testOperationEquals;

static class TestDescription_TestBiMap_testOperationCopy : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testOperationCopy() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 265, "testOperationCopy" ) {}
 void runTest() { suite_TestBiMap.testOperationCopy(); }
} testDescription_TestBiMap_testOperationCopy;

static class TestDescription_TestBiMap_testCopyConstructor : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBiMap_testCopyConstructor() : CxxTest::RealTestDescription( Tests_TestBiMap, suiteDescription_TestBiMap, 281, "testCopyConstructor" ) {}
 void runTest() { suite_TestBiMap.testCopyConstructor(); }
} testDescription_TestBiMap_testCopyConstructor;

#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestRpcRoutedClient.h"

static TestRpcRoutedClient suite_TestRpcRoutedClient;

static CxxTest::List Tests_TestRpcRoutedClient = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestRpcRoutedClient( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestRpcRoutedClient.h", 12, "TestRpcRoutedClient", suite_TestRpcRoutedClient, Tests_TestRpcRoutedClient );

static class TestDescription_TestRpcRoutedClient_testRegisterRoute : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testRegisterRoute() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 37, "testRegisterRoute" ) {}
 void runTest() { suite_TestRpcRoutedClient.testRegisterRoute(); }
} testDescription_TestRpcRoutedClient_testRegisterRoute;

static class TestDescription_TestRpcRoutedClient_testRegisterRouteZero : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testRegisterRouteZero() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 53, "testRegisterRouteZero" ) {}
 void runTest() { suite_TestRpcRoutedClient.testRegisterRouteZero(); }
} testDescription_TestRpcRoutedClient_testRegisterRouteZero;

static class TestDescription_TestRpcRoutedClient_testRegisterRouteNotConnected : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testRegisterRouteNotConnected() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 65, "testRegisterRouteNotConnected" ) {}
 void runTest() { suite_TestRpcRoutedClient.testRegisterRouteNotConnected(); }
} testDescription_TestRpcRoutedClient_testRegisterRouteNotConnected;

static class TestDescription_TestRpcRoutedClient_testDeregisterRoute : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testDeregisterRoute() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 79, "testDeregisterRoute" ) {}
 void runTest() { suite_TestRpcRoutedClient.testDeregisterRoute(); }
} testDescription_TestRpcRoutedClient_testDeregisterRoute;

static class TestDescription_TestRpcRoutedClient_testDeregisterRouteZero : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testDeregisterRouteZero() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 104, "testDeregisterRouteZero" ) {}
 void runTest() { suite_TestRpcRoutedClient.testDeregisterRouteZero(); }
} testDescription_TestRpcRoutedClient_testDeregisterRouteZero;

static class TestDescription_TestRpcRoutedClient_testDeregisterRouteNotConnected : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testDeregisterRouteNotConnected() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 113, "testDeregisterRouteNotConnected" ) {}
 void runTest() { suite_TestRpcRoutedClient.testDeregisterRouteNotConnected(); }
} testDescription_TestRpcRoutedClient_testDeregisterRouteNotConnected;

static class TestDescription_TestRpcRoutedClient_testRegisterRouteTwoTimes : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedClient_testRegisterRouteTwoTimes() : CxxTest::RealTestDescription( Tests_TestRpcRoutedClient, suiteDescription_TestRpcRoutedClient, 129, "testRegisterRouteTwoTimes" ) {}
 void runTest() { suite_TestRpcRoutedClient.testRegisterRouteTwoTimes(); }
} testDescription_TestRpcRoutedClient_testRegisterRouteTwoTimes;

#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestRpcRoutedServer.h"

static TestRpcRoutedServer suite_TestRpcRoutedServer;

static CxxTest::List Tests_TestRpcRoutedServer = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestRpcRoutedServer( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestRpcRoutedServer.h", 12, "TestRpcRoutedServer", suite_TestRpcRoutedServer, Tests_TestRpcRoutedServer );

static class TestDescription_TestRpcRoutedServer_testRegisterReceiver : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterReceiver() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 37, "testRegisterReceiver" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterReceiver(); }
} testDescription_TestRpcRoutedServer_testRegisterReceiver;

static class TestDescription_TestRpcRoutedServer_testRegisterReceiverNULL : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterReceiverNULL() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 50, "testRegisterReceiverNULL" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterReceiverNULL(); }
} testDescription_TestRpcRoutedServer_testRegisterReceiverNULL;

static class TestDescription_TestRpcRoutedServer_testRegisterReceiverTwoDifferentOnDifferentRoutes : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterReceiverTwoDifferentOnDifferentRoutes() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 58, "testRegisterReceiverTwoDifferentOnDifferentRoutes" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterReceiverTwoDifferentOnDifferentRoutes(); }
} testDescription_TestRpcRoutedServer_testRegisterReceiverTwoDifferentOnDifferentRoutes;

static class TestDescription_TestRpcRoutedServer_testRegisterReceiverTwoDifferentOnSameRoute : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterReceiverTwoDifferentOnSameRoute() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 76, "testRegisterReceiverTwoDifferentOnSameRoute" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterReceiverTwoDifferentOnSameRoute(); }
} testDescription_TestRpcRoutedServer_testRegisterReceiverTwoDifferentOnSameRoute;

static class TestDescription_TestRpcRoutedServer_testRegisterReceiverTwoTimes : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterReceiverTwoTimes() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 94, "testRegisterReceiverTwoTimes" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterReceiverTwoTimes(); }
} testDescription_TestRpcRoutedServer_testRegisterReceiverTwoTimes;

static class TestDescription_TestRpcRoutedServer_testRegisterClient : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterClient() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 110, "testRegisterClient" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterClient(); }
} testDescription_TestRpcRoutedServer_testRegisterClient;

static class TestDescription_TestRpcRoutedServer_testRegisterCleintZero : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterCleintZero() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 118, "testRegisterCleintZero" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterCleintZero(); }
} testDescription_TestRpcRoutedServer_testRegisterCleintZero;

static class TestDescription_TestRpcRoutedServer_testRegisterClientTwoDifferentOnDifferentRoutes : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterClientTwoDifferentOnDifferentRoutes() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 126, "testRegisterClientTwoDifferentOnDifferentRoutes" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterClientTwoDifferentOnDifferentRoutes(); }
} testDescription_TestRpcRoutedServer_testRegisterClientTwoDifferentOnDifferentRoutes;

static class TestDescription_TestRpcRoutedServer_testRegisterClientTwoDifferentOnSameRoute : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterClientTwoDifferentOnSameRoute() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 139, "testRegisterClientTwoDifferentOnSameRoute" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterClientTwoDifferentOnSameRoute(); }
} testDescription_TestRpcRoutedServer_testRegisterClientTwoDifferentOnSameRoute;

static class TestDescription_TestRpcRoutedServer_testRegisterClientTwoTimes : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterClientTwoTimes() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 152, "testRegisterClientTwoTimes" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterClientTwoTimes(); }
} testDescription_TestRpcRoutedServer_testRegisterClientTwoTimes;

static class TestDescription_TestRpcRoutedServer_testDispatch : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDispatch() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 165, "testDispatch" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDispatch(); }
} testDescription_TestRpcRoutedServer_testDispatch;

static class TestDescription_TestRpcRoutedServer_testDispatchToNotRegisteredRoute : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDispatchToNotRegisteredRoute() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 189, "testDispatchToNotRegisteredRoute" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDispatchToNotRegisteredRoute(); }
} testDescription_TestRpcRoutedServer_testDispatchToNotRegisteredRoute;

static class TestDescription_TestRpcRoutedServer_testDispatchWithoutClientId : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDispatchWithoutClientId() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 201, "testDispatchWithoutClientId" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDispatchWithoutClientId(); }
} testDescription_TestRpcRoutedServer_testDispatchWithoutClientId;

static class TestDescription_TestRpcRoutedServer_testDeregisterReceiver : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDeregisterReceiver() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 224, "testDeregisterReceiver" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDeregisterReceiver(); }
} testDescription_TestRpcRoutedServer_testDeregisterReceiver;

static class TestDescription_TestRpcRoutedServer_testDeregisterReceiverAndRegisterClientAfter : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDeregisterReceiverAndRegisterClientAfter() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 248, "testDeregisterReceiverAndRegisterClientAfter" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDeregisterReceiverAndRegisterClientAfter(); }
} testDescription_TestRpcRoutedServer_testDeregisterReceiverAndRegisterClientAfter;

static class TestDescription_TestRpcRoutedServer_testDeregisterReceiverNULL : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDeregisterReceiverNULL() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 266, "testDeregisterReceiverNULL" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDeregisterReceiverNULL(); }
} testDescription_TestRpcRoutedServer_testDeregisterReceiverNULL;

static class TestDescription_TestRpcRoutedServer_testCall : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testCall() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 275, "testCall" ) {}
 void runTest() { suite_TestRpcRoutedServer.testCall(); }
} testDescription_TestRpcRoutedServer_testCall;

static class TestDescription_TestRpcRoutedServer_testCallToNotRegisteredRoute : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testCallToNotRegisteredRoute() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 299, "testCallToNotRegisteredRoute" ) {}
 void runTest() { suite_TestRpcRoutedServer.testCallToNotRegisteredRoute(); }
} testDescription_TestRpcRoutedServer_testCallToNotRegisteredRoute;

static class TestDescription_TestRpcRoutedServer_testCallWithoutSender : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testCallWithoutSender() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 313, "testCallWithoutSender" ) {}
 void runTest() { suite_TestRpcRoutedServer.testCallWithoutSender(); }
} testDescription_TestRpcRoutedServer_testCallWithoutSender;

static class TestDescription_TestRpcRoutedServer_testDeregisterClient : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDeregisterClient() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 335, "testDeregisterClient" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDeregisterClient(); }
} testDescription_TestRpcRoutedServer_testDeregisterClient;

static class TestDescription_TestRpcRoutedServer_testDeregisterClientAndRegisterReceiverAfter : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDeregisterClientAndRegisterReceiverAfter() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 357, "testDeregisterClientAndRegisterReceiverAfter" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDeregisterClientAndRegisterReceiverAfter(); }
} testDescription_TestRpcRoutedServer_testDeregisterClientAndRegisterReceiverAfter;

static class TestDescription_TestRpcRoutedServer_testDeregisterClientNULL : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testDeregisterClientNULL() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 375, "testDeregisterClientNULL" ) {}
 void runTest() { suite_TestRpcRoutedServer.testDeregisterClientNULL(); }
} testDescription_TestRpcRoutedServer_testDeregisterClientNULL;

static class TestDescription_TestRpcRoutedServer_testRegisterReceiverWithoutId : public CxxTest::RealTestDescription {
public:
 TestDescription_TestRpcRoutedServer_testRegisterReceiverWithoutId() : CxxTest::RealTestDescription( Tests_TestRpcRoutedServer, suiteDescription_TestRpcRoutedServer, 382, "testRegisterReceiverWithoutId" ) {}
 void runTest() { suite_TestRpcRoutedServer.testRegisterReceiverWithoutId(); }
} testDescription_TestRpcRoutedServer_testRegisterReceiverWithoutId;

#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestStreaming.h"

static TestStreaming suite_TestStreaming;

static CxxTest::List Tests_TestStreaming = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestStreaming( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestStreaming.h", 7, "TestStreaming", suite_TestStreaming, Tests_TestStreaming );

static class TestDescription_TestStreaming_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_TestStreaming_test1() : CxxTest::RealTestDescription( Tests_TestStreaming, suiteDescription_TestStreaming, 11, "test1" ) {}
 void runTest() { suite_TestStreaming.test1(); }
} testDescription_TestStreaming_test1;

#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestTcp.h"

static TestTcp suite_TestTcp;

static CxxTest::List Tests_TestTcp = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestTcp( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/tests/TestTcp.h", 65, "TestTcp", suite_TestTcp, Tests_TestTcp );

static class TestDescription_TestTcp_testConnectAndSendConnected : public CxxTest::RealTestDescription {
public:
 TestDescription_TestTcp_testConnectAndSendConnected() : CxxTest::RealTestDescription( Tests_TestTcp, suiteDescription_TestTcp, 70, "testConnectAndSendConnected" ) {}
 void runTest() { suite_TestTcp.testConnectAndSendConnected(); }
} testDescription_TestTcp_testConnectAndSendConnected;

static class TestDescription_TestTcp_testConnectAndSendFailed : public CxxTest::RealTestDescription {
public:
 TestDescription_TestTcp_testConnectAndSendFailed() : CxxTest::RealTestDescription( Tests_TestTcp, suiteDescription_TestTcp, 106, "testConnectAndSendFailed" ) {}
 void runTest() { suite_TestTcp.testConnectAndSendFailed(); }
} testDescription_TestTcp_testConnectAndSendFailed;

static class TestDescription_TestTcp_testConnectAndReconnect : public CxxTest::RealTestDescription {
public:
 TestDescription_TestTcp_testConnectAndReconnect() : CxxTest::RealTestDescription( Tests_TestTcp, suiteDescription_TestTcp, 139, "testConnectAndReconnect" ) {}
 void runTest() { suite_TestTcp.testConnectAndReconnect(); }
} testDescription_TestTcp_testConnectAndReconnect;

static class TestDescription_TestTcp_testConnectFailedAndReconnect : public CxxTest::RealTestDescription {
public:
 TestDescription_TestTcp_testConnectFailedAndReconnect() : CxxTest::RealTestDescription( Tests_TestTcp, suiteDescription_TestTcp, 187, "testConnectFailedAndReconnect" ) {}
 void runTest() { suite_TestTcp.testConnectFailedAndReconnect(); }
} testDescription_TestTcp_testConnectFailedAndReconnect;

static class TestDescription_TestTcp_testChangeReconnectInterval : public CxxTest::RealTestDescription {
public:
 TestDescription_TestTcp_testChangeReconnectInterval() : CxxTest::RealTestDescription( Tests_TestTcp, suiteDescription_TestTcp, 232, "testChangeReconnectInterval" ) {}
 void runTest() { suite_TestTcp.testChangeReconnectInterval(); }
} testDescription_TestTcp_testChangeReconnectInterval;

#include <cxxtest/Root.cpp>
