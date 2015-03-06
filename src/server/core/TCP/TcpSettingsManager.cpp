#include <Logger/Logger.h>

#include "TcpSettingsManager.h"

TcpSettingsManager::TcpSettingsManager(QObject* parent) :
	QObject(parent)
{
}

TcpSettingsManager::~TcpSettingsManager()
{
}

void TcpSettingsManager::setIniFile(const QString& filePath)
{
	m_settingsFile = filePath;
}

QMap<QString, int> TcpSettingsManager::getAllInfo()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings m_settings(m_settingsFile, QSettings::IniFormat);
	m_settings.setIniCodec("UTF-8");

	QMap<QString, int> mapInfo;
	QStringList childKeys = m_settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
		m_settings.beginGroup(childKey);

		QString name = m_settings.value("name", "").toString();
		int type = m_settings.value("type", 0).toInt();

		if(type > 0)
		{
			log_info(QString("Name = %1, type =%2, %3").arg(QString(name)).arg(type).arg(childKey));
			mapInfo.insert(name, type);
		}
		m_settings.endGroup();
	}

	return mapInfo;
}

QString TcpSettingsManager::getFlakonHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("Flakon/ip", "127.0.0.1").toString();
}

QString TcpSettingsManager::getFlakonPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("Flakon/port", "0").toString();
}

QString TcpSettingsManager::getAtlantHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("ATLANT/ip", "127.0.0.1").toString();
}

QString TcpSettingsManager::getAtlantPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("ATLANT/port", "127.0.0.1").toString();
}
