#include <QtCore/QCoreApplication>
#include <QObject>
#include <QMetaType>

#include "ARM_R_Application.h"

#include "ARM_R_Srv.h"

int main(int argc, char *argv[])
{

    ARM_R_Application a(argc, argv);
    qRegisterMetaType<rpc_flakon_msg> ("rpc_flakon_msg");
    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

    ARM_R_Srv arm;
//    arm.start();


    return a.exec();
}
