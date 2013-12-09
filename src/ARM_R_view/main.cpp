#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QMetaType>

#include "arm_r_view.h"

typedef QVector<QPointF>         rpc_send_points_vector;
typedef QByteArray               rpc_send_atlant_data;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ARM_R_view w;
    w.show();
//    w.slotShowDiagTxt();
    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

    return a.exec();
}
