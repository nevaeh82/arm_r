#include <QtCore/QCoreApplication>
#include <QObject>
#include <QMetaType>

#include <Logger/Logger.h>

#include "ARM_R_Application.h"
#include "ARM_R_Srv.h"


#include <Windows.h>
#include <Dbghelp.h>
#include <stdio.h>

#include <Logger/Logger.h>

#define SERVER_NAME "ZaviruhaRServer"

typedef QVector<QPointF>         rpc_send_points_vector;

HWND hwnd;
static char * g_output = NULL;
static LPTOP_LEVEL_EXCEPTION_FILTER g_prev = NULL;

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS info)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

static void
backtrace_register(void)
{
	LoadLibraryA("msvcrt.dll");

	DWORD dwMode = SetErrorMode(SEM_NOGPFAULTERRORBOX);
	SetErrorMode(dwMode | SEM_NOGPFAULTERRORBOX);

	if (g_output == NULL) {
		g_output = (char*)malloc(sizeof(long));
		g_prev = SetUnhandledExceptionFilter(exception_filter);
	}
}


int main(int argc, char *argv[])
{
	Logger().setupLogger("logs/ARM_R_server.log");

	if(argc < 2) {
        log_debug(QString("You are not set id of server.\n"));
        exit(1);
      }
    int id = QString(*argv[1]).toInt();


	hwnd = ::GetDesktopWindow();
	backtrace_register();

	ARM_R_Application a(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    ARM_R_Srv arm(id);
	//    arm.start();
	return a.exec();
}
