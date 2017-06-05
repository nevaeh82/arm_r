#include <Logger/Logger.h>
#include <Rpc/RpcDefines.h>
#include <Info/Prm300Settings.h>

#include "TcpRDSController.h"

TcpRDSController::TcpRDSController(int serverId, QObject* parent) :
	TcpDeviceController(parent),
	m_stationShift(0),
	m_serverId(serverId),
	m_tcpRdsSettingsController(NULL)
{
	m_tcpDeviceName = RDS_TCP_DEVICE;
	log_debug(QString("Created %1").arg(m_tcpDeviceName));

	m_coordinateCounter = 0;

	init();

	m_locationTimer = new QTimer(this);
	connect(m_locationTimer, SIGNAL(timeout()), this, SLOT(sendLocation()));
	m_locationTimer->setInterval(50);

	m_tcpSendTimer = new QTimer(this);

	connect(this, SIGNAL(signalSendDataInternal(QByteArray)), this, SLOT(onSendDataInternal(QByteArray)));
}

TcpRDSController::TcpRDSController(int serverId, const QString& tcpDeviceName, QObject* parent) :
	TcpDeviceController(tcpDeviceName, parent),
	m_stationShift(0),
	m_serverId(serverId),
	m_tcpRdsSettingsController(NULL)
{
	m_coordinateCounter = 0;
	init();

	m_locationTimer = new QTimer(this);
	connect(m_locationTimer, SIGNAL(timeout()), this, SLOT(sendLocation()));
	m_locationTimer->setInterval(50);


	m_tcpSendTimer = new QTimer(this);

	connect(this, SIGNAL(signalSendDataInternal(QByteArray)), this, SLOT(onSendDataInternal(QByteArray)));
}

TcpRDSController::~TcpRDSController()
{
	if(m_tcpRdsSettingsController)
	{
		delete m_tcpRdsSettingsController;
	}
}

QMap<QString, BaseTcpDeviceController*>& TcpRDSController::stations()
{
	return m_stations;
}

void TcpRDSController::setCoordinateCounter(CoordinateCounter* obj)
{
	m_coordinateCounter = obj;
	m_coordinateCounter->setStationsShift(m_stationShift);
}

void TcpRDSController::setTcpRdsSettingscontroller(TcpRDSSettingsController *controller)
{
	m_tcpRdsSettingsController = controller;
}

void TcpRDSController::createTcpDeviceCoder()
{
	log_debug("Creating TcpRDSCoder...");
	TcpRdsCoder* coder = new TcpRdsCoder(m_flakonSettingStruct.zone,  m_flakonSettingStruct.typeRds, this);
	m_tcpDeviceCoder = coder;

	m_coder = coder;

	coder->setCoordinatesCounter(m_coordinateCounter);

	m_tcpRdsSettingsController->createTcpDeviceCoder();
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
		QString n = m_tcpDeviceName.toUpper() + "-" + QString::number(m_serverId);
		if(childKey == m_tcpDeviceName.toUpper() + "-" + QString::number(m_serverId))
		{
			settings.beginGroup(childKey);
			QString name = settings.value("name", "Unknown").toString();
			//  int serverId = settings.value("id", 0).toInt();
			//  QString n = m_tcpDeviceName + "-" + QString::number(m_serverId);
			//if(name == m_tcpDeviceName + "-" + QString::number(m_serverId))

			m_flakonSettingStruct.zone = settings.value("zone", 0).toInt();
			m_flakonSettingStruct.typeRds = settings.value("typeRds", 0).toInt();
			m_flakonSettingStruct.host = settings.value("ip", "127.0.0.1").toString();
			m_flakonSettingStruct.port = settings.value("Port", 1111).toInt();
			m_flakonSettingStruct.name = settings.value("name", "RDS").toString();
			m_flakonSettingStruct.reconnectInterval = settings.value("reconnectInterval", 1000).toInt();

			m_host = m_flakonSettingStruct.host;
			m_port = m_flakonSettingStruct.port;
			m_deviceType = TypeRDS;//m_flakonSettingStruct.type;

			m_stationShift = settings.value( "shift", 0 ).toInt();

			QByteArray baseInfo;
			QDataStream dsBaseInfo(&baseInfo, QIODevice::WriteOnly);
			dsBaseInfo << m_flakonSettingStruct;


			settings.endGroup();
			return true;
		}
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

bool TcpRDSController::writeToRds(const QByteArray &data)
{
	bool retVal = false;
	//m_tcpClient->write(data);

	if( m_coder->isSend() ) {

		QByteArray dataToSend;
		if(!m_messageBuffer.isEmpty()) {
			dataToSend = m_messageBuffer.takeFirst();
			if(!m_messageBuffer.contains(data)) {
				m_messageBuffer.append(data);
			}
		}

		m_tcpClient->write(data);
		retVal = true;
		m_coder->onSetFlag();
	} else {
		if(!m_messageBuffer.contains(data)) {
			m_messageBuffer.append(data);
		}
	}

	if(!m_messageBuffer.isEmpty()) {
		m_tcpSendTimer->singleShot(10, this, SLOT(onForceWriteToRds()));
	}

	return retVal;
}

void TcpRDSController::onForceWriteToRds()
{
	if( m_coder->isSend() ) {
		if(!m_messageBuffer.isEmpty()) {
			m_tcpClient->write(m_messageBuffer.takeFirst());
			m_coder->onSetFlag();
		}
	}

	if(!m_messageBuffer.isEmpty()) {
		m_tcpSendTimer->singleShot(10, this, SLOT(onForceWriteToRds()));
	}
}

void TcpRDSController::slotTcpConnectionStatus(int status)
{
	if(status == (int)TCP::Connected) {
		//ask all RDS params
		RdsProtobuf::Packet pkt;
		createGetSystemSystemOptions( pkt );
		m_tcpClient->write( pack(pkt) );
	}
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << status;


	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(m_deviceType, m_tcpDeviceName, MessageSP(new Message<QByteArray>(TCP_RDS_WORK_MODE, data)));
	}
}

RpcRoutedServer::RouteId TcpRDSController::getRouteId() const
{
	return RDS_ROUTE_ID;
}

void TcpRDSController::onGetStations()
{
	QByteArray data;

	//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_SET_STATUS, data)));

	//	QDataStream st(&data, QIODevice::ReadWrite);
	//	st << true;
	//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_TURN_STATUS, data)));

	//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_GET_SYSTEM, data)));
	//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_WORK_MODE, data)));
	//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_GET_STATUS, data)));
	//	sendData(MessageSP(new Message<QByteArray>(TCP_RDS_GET_STATUS1, data)));
	//	sendData( MessageSP( new Message<QByteArray>( TCP_RDS_GET_LOC_STATUS, data ) ) );
	//	sendData( MessageSP( new Message<QByteArray>( TCP_RDS_GET_ANALYSIS_STATUS, data ) ) );
}

void TcpRDSController::sendLocation()
{
	if(m_lastLocationMessageTime.msecsTo(QTime::currentTime()) > 2000) {
		return;
	}

	if(m_coder->isSend()) {
		if( writeToRds(m_lastLocationMessage) ) {
			m_coder->setReadyToPushSpectrums(false);
		}
	}

}

void TcpRDSController::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();

	if (method == RPC_METHOD_SET_MAIN_STATION_CORRELATION) {
		//requestStationCorellation(argument.toString());
		sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION, data)));
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
		//		if(result) {
		//			m_coordinateCounter->setCenterFrequency(frequency);
		//		}

		//m_coordinateCounter->setCenterFrequency(frequency);
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
	else if(method == RPC_METHOD_ENABLE_RECEIVER) {
		sendData( MessageSP( new Message<QByteArray>( TCP_FLAKON_REQUEST_ENABLE_RECEIVER, data ) ) );
	}
	else if(method == RPC_METHOD_WORK_MODE_M) {
		sendData(MessageSP(new Message<QByteArray>(TCP_RDS_SET_STATUS, data)));
	}
	else if(method == RPC_METHOD_WORK_MODE_ON) {
		sendData(MessageSP(new Message<QByteArray>(TCP_RDS_TURN_STATUS, data)));
	}

	else if (method == RPC_METHOD_PRM_SET_ATT1) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_ATTENUER_ONE, data ) ) );
		return;
	}
	else if (method == RPC_METHOD_PRM_SET_ATT2) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_ATTENUER_TWO, data ) ) );
		return;
	}
	else if (method == RPC_METHOD_PRM_SET_FILTER) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_FILTER, data ) ) );
		return;
	}
	else if (method == RPC_METHOD_FLAKON_REQUEST_STATUS) {
		//		bool state = isConnected();
		//		requestTest();
		//		log_info( QString( "Connection state for %1 = %2" ).arg( m_tcpDeviceName ).arg( state ) );
		//		sendData( MessageSP( new Message<QByteArray>( TCP_RDS_GET_LOC_STATUS, data ) ) );
		sendData( MessageSP( new Message<QByteArray>( TCP_RDS_GET_PRM_STATUS, data ) ) );
	}
	else if(method == RPC_METHOD_SEND_RDS_PROTO) {
		sendData( MessageSP( new Message<QByteArray>( TCP_RDS_SEND_PROTO, data ) ) );
	}

}


void TcpRDSController::sendData(const MessageSP message)
{
	if (NULL == m_tcpDeviceCoder) {
		log_warning(QString("m_tcpDeviceCoder is NULL"));
		return;
	}

	//QByteArray decodedData = m_tcpDeviceCoder->decode(message);
	bool isLocation;
	QByteArray decodedData = m_coder->decodeWithCheckLocation(message, isLocation);

	if (decodedData.size() <= 0) {
		log_debug(QString("decodedData.size() <= 0 for %1").arg(m_tcpDeviceName));
		return;
	}

	if (NULL == m_tcpClient) {
		log_warning(QString("m_tcpClient is NULL"));
		return;
	}

	if(isLocation) {
		m_coder->setReadyToPushSpectrums();
		emit signalSendDataInternal(decodedData);
	}

	writeToRds(decodedData);
}

void TcpRDSController::onSendDataInternal(QByteArray decodedData)
{
	m_lastLocationMessage = decodedData;
	m_lastLocationMessageTime = QTime::currentTime();

	m_locationTimer->start();
}
