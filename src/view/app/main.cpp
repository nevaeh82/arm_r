#include <QtGui/QApplication>
#include <QtSingleApplication>
#include <QTranslator>

#include <Logger/Logger.h>

#include "MainWindow.h"
#include "MainWindowController.h"

#include "ComReceiver/ComReceiverController.h"
#include "ComReceiver/ComReceiverView.h"


#include <Windows.h>
#include <Dbghelp.h>
#include <stdio.h>

typedef QVector<QPointF>         rpc_send_points_vector;

HWND hwnd;
static char * g_output = NULL;
static LPTOP_LEVEL_EXCEPTION_FILTER g_prev = NULL;

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS info)
{

	QProcess p;
	QString guiName = "./" + QString(GUI_NAME);
	p.startDetached(guiName);
	p.waitForStarted(10000);

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
	Logger().setupLogger("logs/ARM_R_view.log");

	QtSingleApplication a(argc, argv);

//	if(a.isRunning()) {
//		return 0;
//	}

	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);

	hwnd = ::GetDesktopWindow();
	backtrace_register();

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QCoreApplication::setOrganizationName("TRK");
	QCoreApplication::setApplicationName("Zaviruha");
	QApplication::setGraphicsSystem("opengl");

	Q_INIT_RESOURCE(translations);

	QTranslator translator;
	translator.load(":/ARM-R_ru.qm");
	a.installTranslator(&translator);

	QTranslator translatorIPP;
	bool b;
	b = translatorIPP.load(":/GraphicsIpp/GraphicsIpp_ru.qm");
	a.installTranslator(&translatorIPP);

	if(argc == 1) {
		MainWindow view;

		MainWindowController controller;
		controller.appendView(&view);

		view.show();

		return a.exec();
	} else {
		ComReceiverController controller;
		controller.getView()->show();


		return a.exec();
	}

	return a.exec();
}
