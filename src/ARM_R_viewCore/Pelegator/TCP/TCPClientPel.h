#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qhostaddress.h>
#include <QString>
#include <QtNetwork/QtNetwork>

#include <cstdlib>
#include <deque>
#include <iostream>

#include "../../Common/CRCs.h"

class TCPClientPel : public QObject
{
    Q_OBJECT
public:
    TCPClientPel(QString host, quint16 port);
    ~TCPClientPel();

public:
    void distruct();
    void start_recv();
//    TCPParser *getParser();


public:
    QTcpSocket *socket_;

private:
  bool _should_stop;
//  char _cur_type;
//  char *_cur_msg;

//  char _type;
//  char _num_length;
  char _blockSize;

  char  *_income_data;
  int  _pos;
//  QByteArray _header_w_data;

//  QTimer *_write_timer;

  QString _host;
  quint16 _port;
  //QueueToSend *_qs;

  QTimer *write_timer;
  QTimer *read_timer;

  bool _isRead;

//  ZaviruhaPayloadPacketHeader   payloadHeader;
//  ZaviruhaPayloadPacket         payloadPacket;
//  TCPParser                     *_parser;

//  ZaviruhaPayloadPacketHeader   _header;
  QByteArray bg;

//  static const char             _magic[];

  QMutex                        _mutex;

private:
      void _complete_msg(QDataStream &stream);

signals:
  void recv_msg(const char *);
  void err_connection();
  void finished();
  void error(QString err);
  void connected();

  void ping(bool);

  void signal_finish_parsing();

  void signalParsing(QByteArray data);

  void signalFFT(const QVector<QPointF> vecFFT, const bool isComplex);
  void signalFFT2(const QVector<QPointF> vecFFT, const bool isComplex);
  void signalFFT3(const QVector<QPointF> vecFFT, const bool isComplex);
  void signalCoord(int id_BE, QPointF point);
  void signalConnect();
  void signalReconnection();

public slots:
  void proccess();
  void stop();
  void onWrite(QByteArray _data, int size);
  void slotRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);

  void slotDestroy();

private slots:
     void onSocketConnected();
     void onSocketDisconnected();
     void onSocketReadyRead();
     void onSocketDisplayError(QAbstractSocket::SocketError socketError);


     void read_by_timer();
     void finish_connection();
     void reconnection();

public slots:
     void writing(QByteArray data);
};

#endif // TCPCLIENT_H
