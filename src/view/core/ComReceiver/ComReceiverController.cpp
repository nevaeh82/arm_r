#include "ComReceiverController.h"

#define PD_CHANNEL 1
#define PRM_CHANNEL 2

#define TCP_RECONNECT 10000

#define USE_RAW_COM 0

#include "Logger/Logger.h"

#include "Sleeper.h"

ComReceiverController::ComReceiverController(QObject *parent)
{
	m_view = new ComReceiverView(0);
	m_connection = 0;

	connect(m_view, SIGNAL(signalComConnect(QString,bool)), this, SLOT(onComConnect(QString, bool)));
	connect(m_view, SIGNAL(signalKtrConnect(QString,bool)), this, SLOT(onKtrConnect(QString, bool)));

	connect(m_view, SIGNAL(signalSetPrmFreq(int)), this, SLOT(onSetPrmFreq(int)));
	connect(m_view, SIGNAL(signalSetPrmAtt(int)), this, SLOT(onSetPrmAtt(int)));
	connect(m_view, SIGNAL(signalSetPrmFilter(int)), this, SLOT(onSetPrmFilter(int)));

	connect(m_view, SIGNAL(signalSetPdFreq(int)), this, SLOT(onSetPdFreq(int)));
	connect(m_view, SIGNAL(signalSetPdAtt(int)), this, SLOT(onSetPdAtt(int)));

	m_serialport = new QSerialPort(this);
	connect(m_serialport, SIGNAL(readyRead()), this, SLOT(onComRead()));
	connect(m_serialport, SIGNAL(aboutToClose()), m_view, SLOT(onComDisconnect()));

	m_reconnectTimer = new QTimer(this);
	m_reconnectTimer->setInterval(TCP_RECONNECT);
	connect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(onTcpReconnect()));
}

ComReceiverController::~ComReceiverController()
{
	if(m_serialport) {
		m_serialport->close();
		delete m_serialport;
	}
}

ComReceiverView *ComReceiverController::getView()
{
	return m_view;
}

void ComReceiverController::comParseData(const QByteArray& data, int device)
{
	m_comBuffer.append(data);

	if(m_comBuffer.size() < 10) {
		return;
	} else if(m_comBuffer.size() > 100) {
		m_comBuffer.clear();
		return;
	}

	QByteArray prmStart;
	QByteArray pdStart;
	prmStart.append('M');
	prmStart.append('x');
	pdStart.append(prmStart);
	prmStart.append(2);
	pdStart.append(1);

	while( m_comBuffer.contains(prmStart) || m_comBuffer.contains(pdStart) ) {
		int index1 = m_comBuffer.indexOf(prmStart);
		int index2 =  m_comBuffer.indexOf(pdStart);

		int index = -1;

		if(index1 < 0) {
			index = index2;
		} else if(index2 < 0) {
			index = index1;
		} else {
			index = qMin(index1, index2);
		}

		if(index < 0) {
			continue;
		}

		if(m_comBuffer.size() < index+5) {
			return;
		}

		int device = (uchar)m_comBuffer.at(index+2);
		int size = (uchar)m_comBuffer.at(index+4);

		if(m_comBuffer.size() < index+5+size+1) {
			return;
		}

		if(size == 0x0B) { //Work on new PRM answer
			int temp = ((int)uchar(m_comBuffer.at(index+9))<<8) + (uchar)m_comBuffer.at(index+10);
			int freq = ((int)uchar(m_comBuffer.at(index+11))<<8) + (uchar)m_comBuffer.at(index+12);
			int filter = (uchar)m_comBuffer.at(index+13);
			int att = (uchar)m_comBuffer.at(index+14);
			int inputInd = (uchar)m_comBuffer.at(index+15);
			int a1 = uchar(m_comBuffer.at(index+11))<<8;
			int a2 = (uchar)m_comBuffer.at(index+12);
			int a3 = a1+a2;

//			log_debug(QString("device: %1 att: %2  freq: %3 .... size :%4")
//					  .arg(device).arg(att).arg(freq).arg(size));
			if(device == PRM_CHANNEL) {
				m_view->setPrmDataLong( freq, att, filter, temp, inputInd );
			}
		} else { //work on old PRM answer or receiver
			//5,6 is 0xAA 0xAA
			int val1 = (uchar)m_comBuffer.at(index+7);
			int val2 = (uchar)m_comBuffer.at(index+8);

//			log_debug(QString("device: %1 data1: %2  data2: %3 .... size :%4")
//					  .arg(device).arg(val1).arg(val2).arg(size));
			if(device == PRM_CHANNEL) {
				m_view->setPrmData( (val1<<8)+val2 );
			} else if(device == PD_CHANNEL) {
				m_view->setPdData( (val1<<8)+val2 );
			}
		}

		m_comBuffer = m_comBuffer.right( m_comBuffer.length() - (index+5+size+1) );
	}


	///Work with raw com
//	QByteArray prmStart;
//		QByteArray pdStart;
//		prmStart.append(0xAA);
//		prmStart.append(0xAA);
//		pdStart.append(prmStart);

//		while( m_comBuffer.contains(prmStart) || m_comBuffer.contains(pdStart) ) {
//			int index1 = m_comBuffer.indexOf(prmStart);
//			int index2 =  m_comBuffer.indexOf(pdStart);

//			int index = -1;

//			if(index1 < 0) {
//				index = index2;
//			} else if(index2 < 0) {
//				index = index1;
//			} else {
//				index = qMin(index1, index2);
//			}

//			if(index < 0) {
//				continue;
//			}

//			if(m_comBuffer.size() < index+5) {
//				return;
//			}

//			int device = 2;
//			int size = 11;

//			if(m_comBuffer.size() < index+5+size+1) {
//				return;
//			}

//			if(size == 0x0B) { //Work on new PRM answer
//				int temp = ((int)uchar(m_comBuffer.at(index+4))<<8) + (uchar)m_comBuffer.at(index+5);
//				int freq = ((int)uchar(m_comBuffer.at(index+6))<<8) + (uchar)m_comBuffer.at(index+7);
//				int filter = (uchar)m_comBuffer.at(index+8);
//				int att = (uchar)m_comBuffer.at(index+9);

//				log_debug(QString("device: %1 att: %2  freq: %3 .... size :%4")
//						  .arg(device).arg(att).arg(freq).arg(size));

//			} else { //work on old PRM answer or receiver
//				//5,6 is 0xAA 0xAA
//				int val1 = (uchar)m_comBuffer.at(index+7);
//				int val2 = (uchar)m_comBuffer.at(index+8);

//				log_debug(QString("device: %1 data2: %2  data: %3 .... size :%4")
//						  .arg(device).arg(val1).arg(val2).arg(size));
//			}

//			m_comBuffer = m_comBuffer.right( m_comBuffer.length() - (index+5+size+1) );
//		}
}

void ComReceiverController::onComConnect(QString addr, bool on)
{
	if(m_serialport->isOpen()) {
		m_serialport->close();
	}
	if(!on) {
		m_connection = 0;
		return;
	}

	m_serialport->setPortName(addr);
	m_serialport->setBaudRate(QSerialPort::Baud115200);
	m_serialport->setDataBits(QSerialPort::Data8);
	m_serialport->setStopBits(QSerialPort::OneStop);
	m_serialport->setParity(QSerialPort::NoParity);
	m_serialport->setFlowControl(QSerialPort::NoFlowControl);

	bool b = m_serialport->open(QIODevice::ReadWrite);

	m_view->setComConnectState(b);
	m_connection = 1;
}

void ComReceiverController::onKtrConnect(QString addr, bool on)
{
	if(m_serialport->isOpen()) {
		m_serialport->close();
	}

	m_connection = 0;

	m_ktrHost = addr;

	if(!on) {
		m_reconnectTimer->stop();

		foreach (QTcpSocket* socket, m_ktrConnectionMap.values()) {
			socket->disconnectFromHost();
			socket->deleteLater();
		}

		m_view->setKtrConnectState(false);
		m_connection = 0;

		return;
	}

	QList<QString> devList;
	m_view->getBoards(devList);
	foreach (QString boardDev, devList) {
		QStringList lst = boardDev.split(",");

		if(lst.size() != 2) {
			continue;
		}

		int board = lst.at(0).toInt();
		char device = lst.at(1).toInt();

		QTcpSocket* m_connection = new QTcpSocket(this);
		m_ktrConnectionMap.insert((board<<8) + device, m_connection);
		connect(m_connection, SIGNAL(connected()), this, SLOT(onSocketConnected()));
		connect(m_connection, SIGNAL(readyRead()), this, SLOT(onTcpRead()));

		m_connection->connectToHost(m_ktrHost, 64300, QIODevice::ReadWrite);
	}
	m_reconnectTimer->start();
	m_connection = 2;
}

void ComReceiverController::onComRead()
{
	QByteArray data = m_serialport->readAll();
	comParseData(data);
}

void ComReceiverController::onTcpRead()
{
	QObject* obj = sender();
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(obj);

	if(socket && m_ktrConnectionMap.values().contains(socket)) {
		QByteArray data = socket->readAll();
		int device = m_ktrConnectionMap.key(socket);
		comParseData(data, device);
	}
}

void ComReceiverController::onSetPrmFreq(int fValue)
{
	if(m_view->isNewPrm()) {
			//******Raw Command *********
			QByteArray rawCommand;
			rawCommand.append(0xAA);
			rawCommand.append(0x0B);
			rawCommand.append((char)(fValue & 255));
			rawCommand.append(0x01);

			rawCommand.append(0xAA);
			rawCommand.append(0x0C);
			rawCommand.append((char)((fValue >> 8) & 255));
			rawCommand.append(0x01);

			rawCommand.append(0xAA);
			rawCommand.append(0x0D);
			rawCommand.append(char(0));
			rawCommand.append(0x01);
			//******Raw Command *********

		int length = 6 +rawCommand.size();
		QByteArray command;
		int xorVal = 0x51;
		command.append('M');
		command.append('x');
		command.append(PRM_CHANNEL);
		command.append(char(0));
		command.append(rawCommand.size());
		command.append(rawCommand);
		for (int i = 2; i < length-1; i++) {
			xorVal ^= (int)command[i] & 255;
		}
		command[length-1] = (char)xorVal;

		if(USE_RAW_COM) {
			writeCommand(rawCommand);
		} else {
			writeCommand(command);
		}

	} else {
		int length = 6 + 2 + 2 + 2;
		char* command = new char[length];
		int xorVal = 0x51;
		command[0] = 'M';
		command[1] = 'x';
		command[2] = (char)PRM_CHANNEL;
		command[3] = 0;
		command[4] = 6;
		command[5] = 0x0b;
		command[6] = (char)(fValue & 255);
		command[7] = 0x0c;
		command[8] = (char)((fValue >> 8) & 255);
		command[9] = 0x0d;
		command[10] = 0;
		for (int i = 2; i < length-1; i++) {
			xorVal ^= (int)command[i] & 255;
		}
		command[length-1] = (char)xorVal;

		writeCommand(command, length);

		delete []command;
	}
}

void ComReceiverController::onSetPrmAtt(int value)
{
	if(m_view->isNewPrm()) {

		//******Raw Command *********
		QByteArray rawCommand;
		rawCommand.append(0xAA);
		rawCommand.append(0x03);
		rawCommand.append((char)(value & 255));
		rawCommand.append(0x01);
		//******Raw Command *********

		int length = 6 + rawCommand.size();
		QByteArray command;
		int xorVal = 0x51;
		command.append('M');
		command.append('x');
		command.append(PRM_CHANNEL);
		command.append(char(0));
		command.append(rawCommand.size());
		command.append(rawCommand);
		for (int i = 2; i < length-1; i++) {
			xorVal ^= (int)command[i] & 255;
		}
		command[length-1] = (char)xorVal;

		if(USE_RAW_COM) {
			writeCommand(rawCommand);
		} else {
			writeCommand(command);
		}
	} else {
		int length = 6 + 2;
		char* command = new char[length];
		int xorVal = 0x51;
		command[0] = 'M';
		command[1] = 'x';
		command[2] = (char)PRM_CHANNEL;
		command[3] = 0;
		command[4] = 2;
		command[5] = 0x03;
		command[6] = (char)(value & 255);
		for (int i = 2; i < length-1; i++) {
			xorVal ^= (int)command[i] & 255;
		}
		command[length-1] = (char)xorVal;

		writeCommand(command, length);

		delete []command;
	}
}

void ComReceiverController::onSetPrmFilter(int value)
{
	if(m_view->isNewPrm()) {

		//******Raw Command *********
		QByteArray rawCommand;
		rawCommand.append(0xAA);
		rawCommand.append(0x45);
		rawCommand.append((char)(value & 255));
		rawCommand.append(0x01);
		//******Raw Command *********

		int length = 6 + rawCommand.size();
		QByteArray command;
		int xorVal = 0x51;
		command.append('M');
		command.append('x');
		command.append(PRM_CHANNEL);
		command.append(char(0));
		command.append(rawCommand.size());
		command.append(rawCommand);
		for (int i = 2; i < length-1; i++) {
			xorVal ^= (int)command[i] & 255;
		}
		command[length-1] = (char)xorVal;

		if(USE_RAW_COM) {
			writeCommand(rawCommand);
		} else {
			writeCommand(command);
		}
	} else {
		int length = 6 + 2;
		char* command = new char[length];
		int xorVal = 0x51;
		command[0] = 'M';
		command[1] = 'x';
		command[2] = (char)PRM_CHANNEL;
		command[3] = 0;
		command[4] = 2;
		command[5] = 0x11;
		command[6] = (char)(value & 255);
		for (int i = 2; i < length-1; i++) {
			xorVal ^= (int)command[i] & 255;
		}
		command[length-1] = (char)xorVal;

		writeCommand(command, length);

		delete []command;
	}
}

void ComReceiverController::onSetPdFreq(int fValue)
{
	int length = 6 + 2 + 2 + 2;
	fValue -= 910;
	char* command = new char[length];
	int xorVal = 0x51;
	command[0] = 'M';
	command[1] = 'x';
	command[2] = (char)PD_CHANNEL;
	command[3] = 0;
	command[4] = 6;
	command[5] = 0x0b;
	command[6] = (char)(fValue & 255);
	command[7] = 0x0c;
	command[8] = (char)((fValue >> 8) & 255);
	command[9] = 0x0d;
	command[10] = 0;
	for (int i = 2; i < length-1; i++) {
		xorVal ^= (int)command[i] & 255;
	}
	command[length-1] = (char)xorVal;

	writeCommand(command, length);

	delete []command;
}

void ComReceiverController::onSetPdAtt(int value)
{
	int length = 6 + 2;
	char* command = new char[length];
	int xorVal = 0x51;
	command[0] = 'M';
	command[1] = 'x';
	command[2] = (char)PD_CHANNEL;
	command[3] = 0;
	command[4] = 2;
	command[5] = 0x41;
	command[6] = (char)(value & 255);
	for (int i = 2; i < length-1; i++) {
		xorVal ^= (int)command[i] & 255;
	}
	command[length-1] = (char)xorVal;

	writeCommand(command, length);

	delete []command;
}

void ComReceiverController::onSocketConnected()
{
	QObject* obj = sender();
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(obj);

	if(socket && m_ktrConnectionMap.values().contains(socket)) {

		int device = m_ktrConnectionMap.key(socket);

		QString command = QString("user link command b%1d%2").arg( (device>>8) ).arg( (device & 255) );

		socket->write(command.toLatin1());

	}

	m_view->setKtrConnectState(true);
}

void ComReceiverController::onTcpReconnect()
{
	foreach (QTcpSocket* socket, m_ktrConnectionMap.values()) {
		if(socket->state()!=QAbstractSocket::ConnectedState) {
			socket->connectToHost(m_ktrHost, 64300, QIODevice::ReadWrite);
		}
	}
}

void ComReceiverController::writeCommand(const QByteArray &data)
{
	if(m_connection == 0) {
		return;
	}

	if(m_connection == 1) {
		m_serialport->write(data);
	} else if(m_connection == 2) {
		foreach (QTcpSocket* socket, m_ktrConnectionMap.values()) {

				socket->write(data);

		}
	}
}

void ComReceiverController::writeCommand(char *data, int size)
{
	if(m_connection == 0) {
		return;
	}

	if(m_connection == 1) {
		m_serialport->write(data, size);
	} else if(m_connection == 2) {
		foreach (QTcpSocket* socket, m_ktrConnectionMap.values()) {

				socket->write(data, size);

		}
	}
}
