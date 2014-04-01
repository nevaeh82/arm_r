#include "TcpPRM300Controller.h"

TcpPRM300Controller::TcpPRM300Controller(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = PRM300_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpPRM300Controller::TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	init();
	connect(this, SIGNAL(createTcpPRM300CoderInternalSignal()), this, SLOT(createTcpPRM300CoderInternalSlot()));
	connect(this, SIGNAL(signalTcpDeviceConnectedToHost(int)), this, SLOT(slotTcpConnectionStatus(int)));
}

TcpPRM300Controller::~TcpPRM300Controller()
{
}

bool TcpPRM300Controller::init()
{
	// TODO read settings
	QSettings settings("./TCP/coders.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			prmSettings.id = settings.value("id", -1).toInt();
			prmSettings.host = settings.value("ip", "127.0.0.1").toString();
			prmSettings.port = settings.value("Port", 2323).toInt();
			prmSettings.name = settings.value("name", "").toString();
			prmSettings.latitude = settings.value("latitude", 0).toDouble();
			prmSettings.longitude = settings.value("longitude", 0).toDouble();
			prmSettings.geo_altitude = settings.value("geo_altitude", 0).toInt();
			prmSettings.altitude = settings.value("altitude", 0).toInt();
			prmSettings.trace = settings.value("trace", "0").toString();
			prmSettings.wire_length = settings.value("wire_length", 0).toInt();

			m_host = prmSettings.host;
			m_port = prmSettings.port;
			m_deviceType = BaseSettingsType::TypePRM300;
			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}

	return false;
}

void TcpPRM300Controller::createTcpDeviceCoder()
{
	emit createTcpPRM300CoderInternalSignal();
}

QObject* TcpPRM300Controller::asQObject()
{
	return this;
}

QByteArray TcpPRM300Controller::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << prmSettings;

	return ba;
}

void TcpPRM300Controller::createTcpPRM300CoderInternalSlot()
{
	debug("Creating TcpPRM300Coder...");
	m_tcpDeviceCoder = new TcpPRM300Coder(this);
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
		receiver->onMessageReceived(m_deviceType, m_tcpDeviceName, message);
	}
}
