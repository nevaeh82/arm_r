#include <Logger.h>

#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = FLAKON_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpFlakonController::TcpFlakonController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	bool err = init();
	connect(this, SIGNAL(createTcpFlakonCoderInternalSignal()), this, SLOT(createTcpFlakonCoderInternalSlot()));
}

TcpFlakonController::~TcpFlakonController()
{
}

void TcpFlakonController::createTcpDeviceCoder()
{
	emit createTcpFlakonCoderInternalSignal();
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
			m_deviceType = BaseSettingsType::TypeFlakon;//m_flakonSettingStruct.type;

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

void TcpFlakonController::createTcpFlakonCoderInternalSlot()
{
	debug("Creating TcpFlakonCoder...");
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
}
