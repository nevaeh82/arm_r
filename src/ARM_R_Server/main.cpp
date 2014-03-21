#include <QtCore/QCoreApplication>
#include <QObject>
#include <QMetaType>
#include <PwLogger/PwLogger.h>


#include "ARM_R_Application.h"

#include "ARM_R_Srv.h"

#define PWLOGGERINIT_AGENTCONFNAME ("ARM_R_server.log4qt")

int main(int argc, char *argv[])
{
    Pw::Logger::PwLoggerFactory::Instance()->Configure(PWLOGGERINIT_AGENTCONFNAME);



    ARM_R_Application a(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));



//    qRegisterMetaType<rpc_flakon_msg> ("rpc_flakon_msg");
//    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
//    qRegisterMetaType<quint32>("quint32");
//    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

    ARM_R_Srv arm;
//    arm.start();

	qDebug() << "тест";

    return a.exec();
}
