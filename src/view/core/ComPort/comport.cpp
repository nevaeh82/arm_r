#include "comport.h"

ComPort::ComPort(QObject *parent) : QObject(parent)
{
	m_markText = false;
	m_validComPort = false;
	m_serialport = new QSerialPort(this);
	connect(this, SIGNAL(signalSetComPort(QString)), this, SLOT(onComConnect(QString)));
	connect(m_serialport, SIGNAL(readyRead()), this, SLOT(onComRead()));
}

ComPort::~ComPort()
{
	if(m_serialport) {
		m_serialport->close();
		delete m_serialport;
	}
}

bool ComPort::isValidComPort()
{
	return m_validComPort;
}

void ComPort::setComPort(QString addr)
{
	emit signalSetComPort(addr);
}

QList<QString> ComPort::getAllPorts()
{
	QList<QString> list;
	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	QList<QSerialPortInfo>::iterator i;
	for (i = ports.begin(); i != ports.end(); ++i)
	{
		list.append((*i).portName());
	}
	return list;
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

	m_validComPort = m_serialport->open(QIODevice::ReadWrite);
	if(m_validComPort)
	{
		init();
	}
}

void ComPort::onComRead()
{
	QByteArray data = m_serialport->readAll();
	comParseData(data);
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
	command = (tr("AT+CMGS=\"89811431955, 89816879903\" \r Hello!\x1A"));
	writeCommand(command.toAscii().data());
}

void ComPort::comParseData(const QByteArray& data)
{
	QString text(data);
	if(text.contains(">"))
	{
		m_markText = true;
	}
	else
	{
		if(m_markText && text.contains("OK"))
		{
			emit signalSent();
		}
		m_markText = false;
	}

	log_debug(text);
}
