#include <Logger/Logger.h>
#include <Rpc/RpcDefines.h>
#include <Info/Prm300Settings.h>

#include "TcpRDSController.h"

TcpRDSController::TcpRDSController(QObject* parent) :
	TcpDeviceController(parent)
{
	m_tcpDeviceName = RDS_TCP_DEVICE;
	log_debug(QString("Created %1").arg(m_tcpDeviceName));

	m_coordinateCounter = 0;
}

TcpRDSController::TcpRDSController(const QString& tcpDeviceName, QObject* parent) :
	TcpDeviceController(tcpDeviceName, parent)
{
	init();
}

TcpRDSController::~TcpRDSController()
{
}

QMap<QString, BaseTcpDeviceController*>& TcpRDSController::stations()
{
	return m_stations;
}

void TcpRDSController::setCoordinateCounter(CoordinateCounter* obj)
{
	m_coordinateCounter = obj;
}

void TcpRDSController::createTcpDeviceCoder()
{
	log_debug("Creating TcpFlakonCoder...");
	m_tcpDeviceCoder = new TcpRdsCoder(this);
}

void TcpRDSController::createTcpClient()
{
	BaseTcpDeviceController::createTcpClient();
	m_tcpClient->setReconnectInterval(m_flakonSettingStruct.reconnectInterval);
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(int)), this, SLOT(slotTcpConnectionStatus(int)));
}

QObject* TcpRDSController::asQObject()
{
	return this;
}

bool TcpRDSController::init()
{
	QSettings settings("./TCP/coders.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			m_flakonSettingStruct.host = settings.value("ip", "127.0.0.1").toString();
			m_flakonSettingStruct.port = settings.value("Port", 1111).toInt();
			m_flakonSettingStruct.name = settings.value("name", "rds").toString();
			m_flakonSettingStruct.reconnectInterval = settings.value("reconnectInterval", 1000).toInt();

			m_host = m_flakonSettingStruct.host;
			m_port = m_flakonSettingStruct.port;
			m_deviceType = TypeRDS;//m_flakonSettingStruct.type;

			QByteArray baseInfo;
			QDataStream dsBaseInfo(&baseInfo, QIODevice::WriteOnly);
			dsBaseInfo << m_flakonSettingStruct;


			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}
	return false;
}

QByteArray TcpRDSController::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_flakonSettingStruct;
	return ba;
}

void TcpRDSController::requestStationCorellation(QString stationName)
{
	// request for "auto" station
	if(stationName == "Авто") {
		int id = 6;
		QByteArray data;
		QDataStream out( &data, QIODevice::WriteOnly );
		out << id;

		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION, data ) ) );
		return;
	}

	// search station by name
	BaseTcpDeviceController* station = m_stations.value(stationName, NULL);
	if(station == NULL) return;

	// load data from station
	QByteArray info = station->getFullInfo();
	QDataStream stream( &info, QIODevice::ReadOnly );

	Prm300Settings logInfo;
	stream >> logInfo;

	// prepare request
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out << logInfo.id;

	sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION, data)));
}

void TcpRDSController::requestTest()
{
	QByteArray data;

	//sendData(MessageSP(new Message<QByteArray>(TCP_RDS_GET_STATUS, data)));
//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_SET_STATUS, data)));

//	QDataStream st(&data, QIODevice::ReadWrite);
//	st << true;
//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_TURN_STATUS, data)));
}

RpcRoutedServer::RouteId TcpRDSController::getRouteId() const
{
	return RDS_ROUTE_ID;
}

void TcpRDSController::slotTcpConnectionStatus(int status)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << status;
	MessageSP message(new Message<QByteArray>(TCP_FLAKON_STATUS, byteArray));

	if (message == NULL) {
		return;
	}

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived((quint32) m_deviceType, m_tcpDeviceName, message);
	}

	requestTest();
}

void TcpRDSController::onGetStations()
{
	QByteArray data;

	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_SET_STATUS, data)));

	QDataStream st(&data, QIODevice::ReadWrite);
	st << true;
	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_TURN_STATUS, data)));

	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_GET_SYSTEM, data)));
}

void TcpRDSController::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();

	if (method == RPC_METHOD_SET_MAIN_STATION_CORRELATION) {
		requestStationCorellation(argument.toString());
	}
	else if (method == RPC_METHOD_SET_BANDWIDTH) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_SET_BANDWIDTH, data ) ) );
	} else if(method == RPC_METHOD_PRM_SET_FREQUENCY) {
		int k = 0;
		k = k+1;
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_FREQUENCY, data ) ) );
	}
	else if (method == RPC_METHOD_SET_SHIFT) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_SET_SHIFT, data ) ) );
	}
	else if (method == RPC_METHOD_SET_CENTER) {
		//signal to save value
		///WTF?? OMG.... FACEPALM.TIFF
		bool result = false;
		const double frequency = argument.toDouble(&result);
		if(result) {
			m_coordinateCounter->setCenterFrequency(frequency);
		}
	}
	else if (method == RPC_METHOD_RECOGNIZE) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_RECOGNIZE, data ) ) );
	}
	else if (method == RPC_METHOD_SS_CORRELATION) {
		QDataStream inputDataStream(&data, QIODevice::ReadOnly);

		QByteArray mess;
		QDataStream ds(&mess, QIODevice::WriteOnly);

		float frequency;
		bool enable;
		inputDataStream >> frequency;
		inputDataStream >> enable;
		ds << enable;
		log_debug(QString("frequency %1 %2").arg(QString::number(frequency)).arg(QString::number(enable)));
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_SS_CORRELATION, mess ) ) );

		MessageSP message(new Message<QByteArray>(RPC_CORRELATION_CONTROL, data));
		if (message == NULL) {
			return;
		}
		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived((quint32) m_deviceType, m_tcpDeviceName, message);
		}

	}
	else if (method == RPC_METHOD_AVARAGE_SPECTRUM) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM, data ) ) );
	}
	if (method == RPC_METHOD_FLAKON_REQUEST_STATUS) {
		bool state = isConnected();
		requestTest();
		log_info( QString( "Connection state for %1 = %2" ).arg( m_tcpDeviceName ).arg( state ) );
	}
}
