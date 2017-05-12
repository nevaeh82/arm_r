#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
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
	bool isValidComPort();

public:
	void setComPort(QString addr);
	QList<QString> getAllPorts();

private slots:
	 void onComConnect(QString addr);

public:
	void writeCommand(const QByteArray& data);
	void writeCommand(char* data, int size);

signals:
	void signalSetComPort(QString);

private:
	void init();

private:
	bool m_validComPort;
};
