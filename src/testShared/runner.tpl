#define CXXTEST_HAVE_EH
#define CXXTEST_ABORT_TEST_ON_FAIL
#include "../TestShared/cxxtest/VerbosePrinter.h"
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
<CxxTest world>
