#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QMetaType>
#include <PwLogger/PwLogger.h>


#include "MainWindow.h"
#include "MainWindowController.h"

typedef QVector<QPointF>         rpc_send_points_vector;
typedef QByteArray               rpc_send_atlant_data;

#define PWLOGGERINIT_AGENTCONFNAME ("ARM_R_view.log4qt")
int main(int argc, char *argv[])
{
	Pw::Logger::PwLoggerFactory::Instance()->Configure(PWLOGGERINIT_AGENTCONFNAME);

    QApplication a(argc, argv);

	//QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	//QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QCoreApplication::setOrganizationName("TRK");
	QCoreApplication::setApplicationName("Zaviruha");
	QApplication::setGraphicsSystem("opengl");

//	Q_INIT_RESOURCE(translations);

//	QTranslator translator;
//	translator.load(":/SkyHobbitLandClient_ru.qm");
//	a.installTranslator(&translator);

	MainWindow view;

	MainWindowController controller;
	controller.appendView(&view);

	view.show();

	controller.startServer();


    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

    return a.exec();
}
