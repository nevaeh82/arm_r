#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qhostaddress.h>
#include <QString>
#include <QtNetwork/QtNetwork>
#include <QMutex>
#include <QMutexLocker>
#include <QByteArray>

#include <cstdlib>
#include <deque>
#include <iostream>

#include <QMutexLocker>

#include <PwLogger/PwLogger.h>

#include "ITCPClient.h"
#include "ITCPParser.h"
#include "../Common/IRouter.h"

#include "ParserAtlant.h"
#include "ParserFlakon.h"
#include "ParserPRM300.h"

#include "../Common/IClient.h"

#include "../Common/ISubscriber.h"

#include "../Common/CRCs.h"

#include "../Flakon/FlakonCoordinateCounter.h"

class TCPClient : public ITcpClient, public IClient
{
    Q_OBJECT
public:
    TCPClient(QString host, quint16 port, IRouter *router);
    ~TCPClient();

public:
    void set(QByteArray data);
    void distruct();

    bool get_state();

public:
    virtual void set_id(int id);
    virtual int  get_id();
    virtual void set_type(int type);
    virtual int get_type();
    virtual void send_data(QSharedPointer<IMessage> msg_ptr);



public:
    QTcpSocket *socket_;

private:
  bool _should_stop;

  QString _host;
  quint16 _port;

  bool _isRead;

  ITCPParser            *_parser;
  IRouter               *_router;

  QMutex                _mutex;
  bool                  _state;
  int                   _type;          /// type of tcp connection (influencee for coder/decoder)
  int                   _id;
  QString               _name;
//  int                   _id;

  ISubscriber*          _subscriber;
  ZaviruhaPayloadPacketHeader _header;
  int                   _number;

  bool                  _is_connected;

  FlakonCoordinateCounter*  _flakon_coordinator;

  static Pw::Logger::ILogger*      _logger;


private:
  void _read_settings();

  ///prm prepare

//  quint8 _calcCRC(QVector<quint8> aForCrcCount);
  bool _specialSymb(quint8 &aByteA, quint8 &aByteB);

  QByteArray _prm_set_freq(unsigned short freq);
  QByteArray _prm_get_freq();
  QByteArray _prm_set_att1(int value);
  QByteArray _prm_set_att2(int value);
  QByteArray _prm_set_filter(int value);




signals:
  void recv_msg(const char *);
  void err_connection();
  void finished();
  void error(QString err);
  void connected();

  void signalConnect();
  void signalReconnection();

  void signalSend(QByteArray data);

  void signalPing(bool);

  void signalPrepareToWrite(QSharedPointer<IMessage> msg_ptr);


public slots:
  void proccess();
  void stop();


  void slotDestroy();

private slots:
  void _slotWrite(QByteArray data);
  void _slotPrepareToWrite(QSharedPointer<IMessage> msg_ptr);

private slots:
     void onSocketConnected();
     void onSocketDisconnected();
     void onSocketReadyRead();
     void onSocketDisplayError(QAbstractSocket::SocketError socketError);

     void finish_connection();
     void reconnection();
     void slotState(QAbstractSocket::SocketState state);

public slots:
     void writing(QByteArray data);

private:
     QByteArray _atlant_set_freq(QByteArray* data);
     void _send_status(bool state);
};

#endif // TCPCLIENT_H
