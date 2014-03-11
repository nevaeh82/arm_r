#include "PServer.h"

PServer::PServer(int nPort, QObject* parent) : QObject(parent)
{
    _id = -1;
    _type = 1;

}

void PServer::startServer()
{
    int nPort = 10240;
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewSocket()));
    if (!tcpServer->listen(QHostAddress::Any, nPort) && server_status==0)
    {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
    } else
    {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
    }

    blockSize=0;
    newData=true;

    connect(this, SIGNAL(signalGetData(rpc_flakon_msg)), this, SLOT(_slotGetData(rpc_flakon_msg)));
}

PServer::~PServer()
{
    if(server_status==1)
    {
        foreach(int i,SClients.keys())
        {
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        server_status=0;
    }
//    QCoreApplication::processEvents();

    emit finished();
}

void PServer::set_id(int id)
{
    _id = id;
}

int PServer::get_id()
{
    return _id;
}

void PServer::set_type(int type)
{
    _type = type;
}

int PServer::get_type()
{
    return _type;
}

void PServer::send_data(QSharedPointer<IMessage> msg_ptr)
{
    emit signalGetData(msg_ptr);
}

void PServer::slotNewSocket()
{
    if(server_status==1 && (SClients.size()<5))
    {
//        qDebug() << QString::fromUtf8("У нас новое соединение!");
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;

        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
        connect(SClients[idusersocs],SIGNAL(disconnected()), this, SLOT(slotSocketDisconnected()));
    }
}

void PServer::slotSocketDisconnected()
{
    if(server_status==1)
    {
        QTcpSocket* clientSocket = (QTcpSocket*)sender();
        QMap<int,QTcpSocket *>::iterator i = SClients.constBegin();
        while (i != SClients.constEnd())
        {
            if (clientSocket==i.value())
            {
                SClients.erase(i);
                return;
            }
            ++i;
        }
    }
}

void PServer::slotReadClient()
{
}

void PServer::_slotGetData(rpc_flakon_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
    IMessage *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);

    QTime tt;
    ds >> tt;
    int state;
    ds >> state;
    QPointF std_p;
    ds >> std_p;
    QVector<QPointF> coordLatLon;
    ds >> coordLatLon;
    double speed;
    ds >> speed;
    double alt;
    ds >> alt;
    double course;
    ds >> course;

    double sko1 = qSqrt(std_p.x()*std_p.x() + std_p.y()*std_p.y());

    EMS::EagleMessage e_msg;
    QString type = "POSITION_ANSWER_MESSAGE";
    QDateTime dt(QDateTime::currentDateTime());
    qint64 d = 0; //datetime
    QString name = "ARM_R";
    QString label = "ARM_R1";
    e_msg.set_type(type.toStdString().c_str(), type.size());
    e_msg.set_sendername(name.toStdString().c_str(), name.size());
    e_msg.set_datetime(d);
    e_msg.add_label(label.toStdString().c_str(), label.size());

    Storm::PositionAnswerMessage d_msg;
    d_msg.set_requestid(50);
    d_msg.set_sourceid(50);
    d_msg.set_datetime(0);
    d_msg.set_longitude(coordLatLon.at(coordLatLon.size() - 1).y());
    d_msg.set_latitude(coordLatLon.at(coordLatLon.size() - 1).x());
    d_msg.set_quality(sko1);

//    char* d1 = new char(d_msg.ByteSize());

//    qDebug() << d_msg.SerializeToArray(d1 , d_msg.ByteSize());
    std::string inner = d_msg.SerializeAsString();

    e_msg.set_innermessage(inner);

//    char* d2 = new char(e_msg.ByteSize());
    std::string message = e_msg.SerializeAsString();

    QByteArray ret;

//    qDebug() << message.size();

//    //QByteArray bg = QByteArray::number(message.size());
    unsigned int size = message.size();
    ret.append(reinterpret_cast<char *>(&size), sizeof(size));
    ret.append(message.c_str(), message.size());


    if (!SClients.isEmpty())
    {
        //QMap<int,QTcpSocket *>::const_iterator i = SClients.constBegin();
        QMap<int,QTcpSocket *>::iterator i = SClients.constBegin();
        while (i != SClients.constEnd())
        {
            QTcpSocket* clientSocket = i.value();
//            QDataStream streamWrite(clientSocket);
//            streamWrite << ret;
            clientSocket->write(ret);

            if ((clientSocket->bytesToWrite())>(100*1024*1024))
            {
                SClients.erase(i);
            }

            ++i;
        }
    }
}