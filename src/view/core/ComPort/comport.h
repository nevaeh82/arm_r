#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTcpSocket>
#include <QTimer>

#include <QObject>

class ComPort : public QObject
{
	Q_OBJECT
public:
	ComPort(QObject *parent = 0);
	~ComPort();

	QSerialPort* m_serialport;

public:
	void setComPort(QString addr);

private slots:
	 void onComConnect(QString addr);

	void writeCommand(const QByteArray& data);
	void writeCommand(char* data, int size);

signals:
	void signalSetComPort(QString);

private:
	void init();
};
