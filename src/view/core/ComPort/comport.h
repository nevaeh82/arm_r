#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTcpSocket>
#include <QTimer>

#include <QObject>
#include "Logger/Logger.h"

class ComPort : public QObject
{
	Q_OBJECT
public:
	ComPort(QObject *parent = 0);
	~ComPort();

	QSerialPort* m_serialport;
	bool isValidComPort();

public:
	void setComPort(QString addr);
	QList<QString> getAllPorts();

private slots:
	 void onComConnect(QString addr);
	 void onComRead();

public:
	void writeCommand(const QByteArray& data);
	void writeCommand(char* data, int size);

signals:
	void signalSetComPort(QString);
	void signalSent();

private:
	void init();	
	void comParseData(const QByteArray& data);
	bool m_markText;

private:
	bool m_validComPort;
};
