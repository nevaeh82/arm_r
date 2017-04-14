#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTcpSocket>
#include <QTimer>

#include "ComReceiverView.h"

class ComReceiverController : public QObject
{
	Q_OBJECT

public:
	ComReceiverController(QObject* parent = 0);
	~ComReceiverController();

	ComReceiverView *getView();

private:
	QSerialPort* m_serialport;
	ComReceiverView* m_view;

	QByteArray m_comBuffer;

	void comParseData(const QByteArray &data, int device = 0);

	QString m_ktrHost;
	QMap<uint, QTcpSocket*> m_ktrConnectionMap;

	QTimer* m_reconnectTimer;

	int m_connection;

private slots:
	void onComConnect(QString addr, bool on);
	void onKtrConnect(QString addr, bool on);

	void onComRead();
	void onTcpRead();

	void onSetPrmFreq(int fValue);
	void onSetPrmFilter(int value);
	void onSetPrmAtt(int value);

	void onSetPdFreq(int fValue);
	void onSetPdAtt(int value);

	void onSocketConnected();

	void onTcpReconnect();

	void writeCommand(const QByteArray& data);
	void writeCommand(char* data, int size);
};

