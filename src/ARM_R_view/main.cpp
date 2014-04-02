#include <QtGui/QApplication>
#include <QTranslator>

#include <Logger.h>

#include "MainWindow.h"
#include "MainWindowController.h"

int main(int argc, char *argv[])
{
	Logger().setupLogger("logs/ARM_R_view.log");

	QApplication a(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QCoreApplication::setOrganizationName("TRK");
	QCoreApplication::setApplicationName("Zaviruha");
	QApplication::setGraphicsSystem("opengl");

	Q_INIT_RESOURCE(translations);

	QTranslator translator;
	translator.load(":/ARM-R_ru.qm");
	a.installTranslator(&translator);

	MainWindow view;

	MainWindowController controller;
	controller.appendView(&view);

	view.show();

	controller.startServer();

	return a.exec();
}
