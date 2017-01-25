#include "SolverClient1.h"

#include "RPC/RpcDefines.h"

SolverClient1::SolverClient1(QString deviceName, QObject* parent) :
	TcpDeviceController(deviceName, parent)
{
	m_tcpDeviceName = deviceName;

	init();
}

SolverClient1::~SolverClient1()
{
	disconnectFromHost();
}

void SolverClient1::onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument)
{
	Q_UNUSED(device);

	QString messageType = argument->type();
	QVariant data = QVariant( argument->data() );

	switch (deviceType) {
	case CLIENT_TCP_SERVER:
	{
		if(messageType == CLIENT_TCP_SERVER_SOLVER_DATA_1) {
			//QByteArray dataToSend = m_encoder1->decode(argument);
			//log_debug("Send to solver >>> ");

			sendData( argument );
			break;
		}
	}
	default:
	break;
	}
}

void SolverClient1::onMethodCalled(const QString &method, const QVariant &argument)
{
	if(method == RPC_METHOD_SOLVER_SETTINGS_SETUP) {
		QByteArray data = argument.toByteArray();
		if(data.isEmpty()) {
			return;
		}

		m_tcpClient->write( argument.toByteArray() );
	}
}

SolverEncoder1* SolverClient1::getSolverEncoder()
{
	return m_encoder1;
}

void SolverClient1::createTcpDeviceCoder()
{
	log_debug("Creating TcpFlakonCoder...");

	SolverEncoder1* encoder = new SolverEncoder1(this);
	m_tcpDeviceCoder = encoder;
	m_encoder1 = encoder;
}

void SolverClient1::createTcpClient()
{
	BaseTcpDeviceController::createTcpClient();
	m_tcpClient->setReconnectInterval(5000);
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(int)), this, SLOT(slotTcpConnectionStatus(int)));
}

QObject* SolverClient1::asQObject()
{
	return this;
}

bool SolverClient1::init()
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
			m_flakonSettingStruct.port = settings.value("Port", 1000).toInt();
			m_flakonSettingStruct.name = settings.value("name", "solverClient").toString();
			m_flakonSettingStruct.reconnectInterval = settings.value("reconnectInterval", 1000).toInt();

			m_host = m_flakonSettingStruct.host;
			m_port = m_flakonSettingStruct.port;
			m_deviceType = TypeSolverClient;//m_flakonSettingStruct.type;

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

QByteArray SolverClient1::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_flakonSettingStruct;
	return ba;
}

RpcRoutedServer::RouteId SolverClient1::getRouteId() const
{
	return SOLVER_ROUTE_ID;
}

