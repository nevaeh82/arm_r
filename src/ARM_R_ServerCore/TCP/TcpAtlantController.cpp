#include "TcpAtlantController.h"

TcpAtlantController::TcpAtlantController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = ATLANT_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpAtlantController::TcpAtlantController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	bool err = init();
	connect(this, SIGNAL(createTcpAtlantCoderInternalSignal()), this, SLOT(createTcpAtlantCoderInternalSlot()));
}

TcpAtlantController::~TcpAtlantController()
{
}

void TcpAtlantController::createTcpDeviceCoder()
{
	emit createTcpAtlantCoderInternalSignal();
}

QObject* TcpAtlantController::asQObject()
{
	return this;
}

bool TcpAtlantController::init()
{
	QSettings settings("./TCP/coders.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			m_atlantSettingStruct.host = settings.value("ip", "127.0.0.1").toString();
			m_atlantSettingStruct.port = settings.value("Port", 2323).toInt();
//			m_atlantSettingStruct.type = settings.value("type", -1).toInt();
			m_atlantSettingStruct.name = settings.value("name", "").toString();

			m_host = m_atlantSettingStruct.host;
			m_port = m_atlantSettingStruct.port;
			m_deviceType = BaseSettingsType::TypeAtlant;//m_atlantSettingStruct.type;

			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}

	return false;
}

QByteArray TcpAtlantController::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_atlantSettingStruct;
	return ba;
}

void TcpAtlantController::createTcpAtlantCoderInternalSlot()
{
	debug("Creating TcpAtlantCoder...");
	m_tcpDeviceCoder = new TcpAtlantCoder(this);
}
