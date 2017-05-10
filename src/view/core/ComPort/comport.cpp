#include "comport.h"

ComPort::ComPort(QObject *parent) : QObject(parent)
{
	m_serialport = new QSerialPort(this);
	connect(this, SIGNAL(signalSetComPort(QString)), this, SLOT(onComConnect(QString)));
}

ComPort::~ComPort()
{
	if(m_serialport) {
		m_serialport->close();
		delete m_serialport;
	}
}

void ComPort::setComPort(QString addr)
{
	emit signalSetComPort(addr);
}


void ComPort::onComConnect(QString addr)
{
	if(m_serialport->isOpen()) {
		m_serialport->close();
	}

	m_serialport->setPortName(addr);
	m_serialport->setBaudRate(QSerialPort::Baud9600);
	m_serialport->setDataBits(QSerialPort::Data8);
	m_serialport->setStopBits(QSerialPort::OneStop);
	m_serialport->setParity(QSerialPort::NoParity);
	m_serialport->setFlowControl(QSerialPort::NoFlowControl);

	bool b = m_serialport->open(QIODevice::ReadWrite);
	if(b)
	{
		init();
	}
}

void ComPort::writeCommand(const QByteArray &data)
{
	m_serialport->write(data);

}

void ComPort::writeCommand(char *data, int size)
{
	m_serialport->write(data, size);
}

void ComPort::init()
{
	QString command = (tr("AT+CMGF=1 \r"));
	writeCommand(command.toAscii().data());
	command = (tr("AT+CMGS=\"89811431955\" \r GGG T\x1A"));
	writeCommand(command.toAscii().data());
}
