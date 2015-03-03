#ifndef PSERVER_H
#define PSERVER_H

#include <stdlib.h>

#include <QObject>
#include <QAbstractSocket>
#include <QMap>
#include <QDebug>
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
#include <QHostAddress>
#include <QCoreApplication>
#include <QtNetwork>

#include <TcpDevicesDefines.h>
#include <ISolver.h>

#include "Atlant/EMS/proto/EagleMessageProto.pb.h"
#include "Flakon/Server/Structs.h"
#include "Interfaces/Tcp/ITcpListener.h"
#include "TCP/TcpDefines.h"

/*!
	Класс для отправки текста и QVector<QPointF> TCP.
*/

//typedef QSharedPointer<IMessageOld> rpc_flakon_msg;



class PServer : public QObject,  public ITcpListener
{
	Q_OBJECT
public:
	PServer(int nPort, QObject* parent = 0);
	virtual ~PServer();

	virtual void set_id(int id);
	virtual int get_id();
	virtual void set_type(int type);
	virtual int get_type();
//    virtual void send_data(QSharedPointer<IMessageOld> msg_ptr);

	virtual void onMessageReceived(const quint32 type, const QString& deviceType, const MessageSP argument);


signals:
//    void signalGetData(rpc_flakon_msg nsg);

	void finished();

public slots:
	void startServer(void);

private slots:
	void slotNewSocket();

	void slotSocketDisconnected();

	void slotReadClient();

	void _slotGetData(QByteArray& data);

private:
	QTcpServer *tcpServer;
	int server_status;
	QMap<int,QTcpSocket *> SClients;

	unsigned int blockSize;
	bool newData;

	int _id;
	int _type;
};

inline QDataStream& operator>>(QDataStream& out, UAVPositionDataEnemy& object)
{
	return out >> object.altitude >> object.altitudeStdDev >> object.speed
			   >> object.course >> object.state >> object.frequency
			   >> object.time >> object.latLonStdDev
			   >> object.latLon;
}

#endif // PSERVER_H
