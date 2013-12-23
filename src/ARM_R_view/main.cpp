#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QMetaType>
#include <PwLogger/PwLogger.h>


#include "arm_r_view.h"

typedef QVector<QPointF>         rpc_send_points_vector;
typedef QByteArray               rpc_send_atlant_data;

#define PWLOGGERINIT_AGENTCONFNAME ("ARM_R_view.log4qt")
int main(int argc, char *argv[])
{
	Pw::Logger::PwLoggerFactory::Instance()->Configure(PWLOGGERINIT_AGENTCONFNAME);

    QApplication a(argc, argv);
    ARM_R_view w;
    w.show();
//    w.slotShowDiagTxt();
    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

    return a.exec();
}
