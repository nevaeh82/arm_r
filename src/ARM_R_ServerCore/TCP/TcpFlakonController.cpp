#include <Logger.h>
#include <Rpc/RpcDefines.h>
#include <Info/Prm300Settings.h>

#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	TcpDeviceController(parent)
{
	m_tcpDeviceName = FLAKON_TCP_DEVICE;
	log_debug(QString("Created %1").arg(m_tcpDeviceName));

	m_coordinateCounter = 0;
}

TcpFlakonController::TcpFlakonController(const QString& tcpDeviceName, QObject* parent) :
	TcpDeviceController(tcpDeviceName, parent)
{
	init();
}

TcpFlakonController::~TcpFlakonController()
{
}

QMap<QString, BaseTcpDeviceController*>& TcpFlakonController::stations()
{
	return m_stations;
}

void TcpFlakonController::setCoordinateCounter(CoordinateCounter* obj)
{
	m_coordinateCounter = obj;
}

void TcpFlakonController::createTcpDeviceCoder()
{
	log_debug("Creating TcpFlakonCoder...");
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
}

void TcpFlakonController::createTcpClient()
{
	BaseTcpDeviceController::createTcpClient();
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(int)), this, SLOT(slotTcpConnectionStatus(int)));
}

QObject* TcpFlakonController::asQObject()
{
	return this;
}

bool TcpFlakonController::init()
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
			m_flakonSettingStruct.port = settings.value("Port", 2323).toInt();
//			m_flakonSettingStruct.type = settings.value("type", -1).toInt();
			m_flakonSettingStruct.name = settings.value("name", "").toString();

			m_host = m_flakonSettingStruct.host;
			m_port = m_flakonSettingStruct.port;
			m_deviceType = TypeFlakon;//m_flakonSettingStruct.type;

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

QByteArray TcpFlakonController::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_flakonSettingStruct;
	return ba;
}

void TcpFlakonController::requestStationCorellation(QString stationName)
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

RpcRoutedServer::RouteId TcpFlakonController::getRouteId() const
{
	return FLAKON_ROUTE_ID;
}

void TcpFlakonController::slotTcpConnectionStatus(int status)
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
}


void TcpFlakonController::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();

	if (method == RPC_METHOD_SET_MAIN_STATION_CORRELATION) {
		requestStationCorellation(argument.toString());
	}
	else if (method == RPC_METHOD_SET_BANDWIDTH) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_SET_BANDWIDTH, data ) ) );
	}
	else if (method == RPC_METHOD_SET_SHIFT) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_SET_SHIFT, data ) ) );
	}
	else if (method == RPC_METHOD_SET_CENTER) {
		//signal to save value
		bool *result;
		const double frequency = argument.toDouble(result);
		if(*result) {
			m_coordinateCounter->setCenterFrequency(frequency);
		}
	}
	else if (method == RPC_METHOD_RECOGNIZE) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_RECOGNIZE, data ) ) );
	}
	else if (method == RPC_METHOD_SS_CORRELATION) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_SS_CORRELATION, data ) ) );
	}
	else if (method == RPC_METHOD_AVARAGE_SPECTRUM) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM, data ) ) );
	}
	if (method == RPC_METHOD_FLAKON_REQUEST_STATUS) {
		bool state = isConnected();

		log_info( QString( "Connection state for %1 = %2" ).arg( m_tcpDeviceName ).arg( state ) );
	}
}
