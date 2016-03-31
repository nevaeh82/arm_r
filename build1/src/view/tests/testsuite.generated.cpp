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
#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/view/tests/DbStationControllerTest.h"

static DBStationControllerTest suite_DBStationControllerTest;

static CxxTest::List Tests_DBStationControllerTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_DBStationControllerTest( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/view/tests/DbStationControllerTest.h", 9, "DBStationControllerTest", suite_DBStationControllerTest, Tests_DBStationControllerTest );

static class TestDescription_DBStationControllerTest_testAddStation : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testAddStation() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 46, "testAddStation" ) {}
 void runTest() { suite_DBStationControllerTest.testAddStation(); }
} testDescription_DBStationControllerTest_testAddStation;

static class TestDescription_DBStationControllerTest_testAddStationDevice : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testAddStationDevice() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 55, "testAddStationDevice" ) {}
 void runTest() { suite_DBStationControllerTest.testAddStationDevice(); }
} testDescription_DBStationControllerTest_testAddStationDevice;

static class TestDescription_DBStationControllerTest_testAddSignalType : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testAddSignalType() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 65, "testAddSignalType" ) {}
 void runTest() { suite_DBStationControllerTest.testAddSignalType(); }
} testDescription_DBStationControllerTest_testAddSignalType;

static class TestDescription_DBStationControllerTest_testAddCategory : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testAddCategory() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 74, "testAddCategory" ) {}
 void runTest() { suite_DBStationControllerTest.testAddCategory(); }
} testDescription_DBStationControllerTest_testAddCategory;

static class TestDescription_DBStationControllerTest_testAddStationData : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testAddStationData() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 83, "testAddStationData" ) {}
 void runTest() { suite_DBStationControllerTest.testAddStationData(); }
} testDescription_DBStationControllerTest_testAddStationData;

static class TestDescription_DBStationControllerTest_testGetStationInfo : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testGetStationInfo() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 105, "testGetStationInfo" ) {}
 void runTest() { suite_DBStationControllerTest.testGetStationInfo(); }
} testDescription_DBStationControllerTest_testGetStationInfo;

static class TestDescription_DBStationControllerTest_testGetFrequencyAndBandwidth : public CxxTest::RealTestDescription {
public:
 TestDescription_DBStationControllerTest_testGetFrequencyAndBandwidth() : CxxTest::RealTestDescription( Tests_DBStationControllerTest, suiteDescription_DBStationControllerTest, 147, "testGetFrequencyAndBandwidth" ) {}
 void runTest() { suite_DBStationControllerTest.testGetFrequencyAndBandwidth(); }
} testDescription_DBStationControllerTest_testGetFrequencyAndBandwidth;

#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/view/tests/SpectrumWidgetDataSourceTest.h"

static SpectrumWidgetDataSourceTest suite_SpectrumWidgetDataSourceTest;

static CxxTest::List Tests_SpectrumWidgetDataSourceTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SpectrumWidgetDataSourceTest( "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/view/tests/SpectrumWidgetDataSourceTest.h", 10, "SpectrumWidgetDataSourceTest", suite_SpectrumWidgetDataSourceTest, Tests_SpectrumWidgetDataSourceTest );

static class TestDescription_SpectrumWidgetDataSourceTest_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_SpectrumWidgetDataSourceTest_test1() : CxxTest::RealTestDescription( Tests_SpectrumWidgetDataSourceTest, suiteDescription_SpectrumWidgetDataSourceTest, 14, "test1" ) {}
 void runTest() { suite_SpectrumWidgetDataSourceTest.test1(); }
} testDescription_SpectrumWidgetDataSourceTest_test1;

static class TestDescription_SpectrumWidgetDataSourceTest_testGraphicsList : public CxxTest::RealTestDescription {
public:
 TestDescription_SpectrumWidgetDataSourceTest_testGraphicsList() : CxxTest::RealTestDescription( Tests_SpectrumWidgetDataSourceTest, suiteDescription_SpectrumWidgetDataSourceTest, 32, "testGraphicsList" ) {}
 void runTest() { suite_SpectrumWidgetDataSourceTest.testGraphicsList(); }
} testDescription_SpectrumWidgetDataSourceTest_testGraphicsList;

static class TestDescription_SpectrumWidgetDataSourceTest_testGraphicsIncomeData : public CxxTest::RealTestDescription {
public:
 TestDescription_SpectrumWidgetDataSourceTest_testGraphicsIncomeData() : CxxTest::RealTestDescription( Tests_SpectrumWidgetDataSourceTest, suiteDescription_SpectrumWidgetDataSourceTest, 47, "testGraphicsIncomeData" ) {}
 void runTest() { suite_SpectrumWidgetDataSourceTest.testGraphicsIncomeData(); }
} testDescription_SpectrumWidgetDataSourceTest_testGraphicsIncomeData;

#include <cxxtest/Root.cpp>
