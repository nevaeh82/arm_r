#include "RpcSettingsManager.h"

RpcSettingsManager::RpcSettingsManager(QObject *parent) :
	QObject(parent)
{
	m_settingsFile = "";
}

RpcSettingsManager::~RpcSettingsManager()
{
}

void RpcSettingsManager::setIniFile(const QString& filePath)
{
	m_settingsFile = filePath;
}

QString RpcSettingsManager::getRpcHost()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("RPC_UI/IP", "127.0.0.1").toString();
}

QString RpcSettingsManager::getRpcPort()
{
	QMutexLocker mutexLocker(&m_mutex);
	QSettings settings(m_settingsFile, QSettings::IniFormat, this);
	return settings.value("RPC_UI/Port", "24500").toString();
}
