#ifndef PSERVER_H
#define PSERVER_H

#include <QObject>
#include <QAbstractSocket>
#include <QMap>
#include <QDebug>

#include <QtNetwork>
//#include <QMessageBox>
#include <QTcpSocket>
#include <QTextCodec>
#include <QAbstractSocket>
#include <QDataStream>
#include <QIODevice>
#include <QByteArray>
#include <QtGlobal>
#include <QString>
#include <QTime>
#include <QtNetwork>
#include <QHostAddress>
#include <QCoreApplication>

#include <stdlib.h>

#include "../../Common/IClient.h"

#include "../../Atlant/EMS/StormEmsClient.h"

#include "../../TCP/ParserAtlant.h"

/*!
    Класс для отправки текста и QVector<QPointF> TCP.
*/

typedef QSharedPointer<IMessage> rpc_flakon_msg;

class PServer : public QObject, public IClient
{
    Q_OBJECT
public:
    PServer(int nPort, QObject* parent = 0);
    ~PServer();

    virtual void set_id(int id);
    virtual int get_id();
    virtual void set_type(int type);
    virtual int get_type();
    virtual void send_data(QSharedPointer<IMessage> msg_ptr);

signals:
    void signalGetData(rpc_flakon_msg nsg);

    void finished();
    
public slots:
    void startServer(void);

private slots:
    void slotNewSocket();

    void slotSocketDisconnected();

    void slotReadClient();

    void _slotGetData(rpc_flakon_msg msg_ptr);

private:
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;

    unsigned int blockSize;
    bool newData;

    int _id;
    int _type;
};

#endif // PSERVER_H
