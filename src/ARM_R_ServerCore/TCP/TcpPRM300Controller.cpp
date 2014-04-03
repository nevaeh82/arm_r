#include <Logger.h>
#include <Rpc/RpcDefines.h>

#include "TcpPRM300Controller.h"

TcpPRM300Controller::TcpPRM300Controller(QObject* parent)
	: TcpDeviceController( parent )
	, m_routeId( 0 )
{
	m_tcpDeviceName = PRM300_TCP_DEVICE;
	init();
}

TcpPRM300Controller::TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent)
	: TcpDeviceController(tcpDeviceName, parent)
	, m_routeId( 0 )
{
	init();
}

TcpPRM300Controller::~TcpPRM300Controller()
{
}

bool TcpPRM300Controller::init()
{
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(int)), this, SLOT(slotTcpConnectionStatus(int)));

	QSettings settings("./TCP/coders.ini", QSettings::IniFormat, this);
	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys) {
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();

		if (name == m_tcpDeviceName) {
			m_prmSettings.id = settings.value("id", -1).toInt();
			m_prmSettings.host = settings.value("ip", "127.0.0.1").toString();
			m_prmSettings.port = settings.value("Port", 2323).toInt();
			m_prmSettings.name = settings.value("name", "").toString();
			m_prmSettings.latitude = settings.value("latitude", 0).toDouble();
			m_prmSettings.longitude = settings.value("longitude", 0).toDouble();
			m_prmSettings.geo_altitude = settings.value("geo_altitude", 0).toInt();
			m_prmSettings.altitude = settings.value("altitude", 0).toInt();
			m_prmSettings.trace = settings.value("trace", "0").toString();
			m_prmSettings.wire_length = settings.value("wire_length", 0).toInt();

			m_host = m_prmSettings.host;
			m_port = m_prmSettings.port;
			m_deviceType = TypePRM300;
			m_routeId = m_prmSettings.id;

			settings.endGroup();

			return true;
		}
		settings.endGroup();
	}

	return false;
}

void TcpPRM300Controller::createTcpDeviceCoder()
{
	//emit createTcpPRM300CoderInternalSignal();

	log_debug("Creating TcpPRM300Coder...");
	m_tcpDeviceCoder = new TcpPRM300Coder(this);
}

QObject* TcpPRM300Controller::asQObject()
{
	return this;
}

QByteArray TcpPRM300Controller::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_prmSettings;

	return ba;
}

void TcpPRM300Controller::slotTcpConnectionStatus(int status)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << status;
	MessageSP message(new Message<QByteArray>(TCP_PRM300_STATUS, byteArray));

	if (message == NULL) {
		return;
	}

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived((quint32) m_deviceType, m_tcpDeviceName, message);
	}
}

RpcRoutedServer::RouteId TcpPRM300Controller::getRouteId() const
{
	if (m_routeId < 0) {
		log_error( QString( "Wrong PRM300 id (name = %1, id = %2)")
				   .arg( m_tcpDeviceName ).arg( m_routeId ) );
	}

	return m_routeId + 1;
}

void TcpPRM300Controller::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();

	if (method == RPC_METHOD_PRM_SET_FREQUENCY) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_FREQUENCY, data ) ) );
		return;
	}

	if (method == RPC_METHOD_PRM_REQUEST_FREQUENCY) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_GET_FREQUENCY, data ) ) );
		return;
	}

	if (method == RPC_METHOD_PRM_SET_ATT1) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_ATTENUER_ONE, data ) ) );
		return;
	}

	if (method == RPC_METHOD_PRM_SET_ATT2) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_ATTENUER_TWO, data ) ) );
		return;
	}

	if (method == RPC_METHOD_PRM_SET_FILTER) {
		sendData( MessageSP( new Message<QByteArray>( TCP_PRM300_REQUEST_SET_FILTER, data ) ) );
		return;
	}

	if (method == RPC_METHOD_REQUEST_STATUS) {
		bool state = isConnected();

		log_info( QString( "Connection state for %1 = %2" ).arg( m_tcpDeviceName ).arg( state ) );
	}
}
