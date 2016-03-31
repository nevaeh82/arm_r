#include <QtCore/QCoreApplication>
#include <QObject>
#include <QMetaType>

#include <Logger/Logger.h>

#include "ARM_R_Application.h"
#include "ARM_R_Srv.h"

int main(int argc, char *argv[])
{
	Logger().setupLogger("logs/ARM_R_server.log");

	ARM_R_Application a(argc, argv);
	if( a.isRunning() ) {
		return 0;
	}

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	ARM_R_Srv arm;
	//    arm.start();
	return a.exec();
}
