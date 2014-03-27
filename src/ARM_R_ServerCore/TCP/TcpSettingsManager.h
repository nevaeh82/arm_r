#ifndef TCPSETTINGSMANAGER_H
#define TCPSETTINGSMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QStringList>

#include <PwLogger/PwLogger.h>

#include "Interfaces/ITcpSettingsManager.h"
#include "SingletonTemplate.h"

class TcpSettingsManager : public QObject, public ITcpSettingsManager, public SingletonTemplate<TcpSettingsManager, ITcpSettingsManager>
{
	Q_OBJECT

private:
	QString m_settingsFile;
	QMutex m_mutex;
	Pw::Logger::ILogger* m_logger;


public:
	explicit TcpSettingsManager(QObject* parent = NULL);
	virtual ~TcpSettingsManager();

	// ITcpSettingsManager interface
public:
	virtual void setIniFile(const QString& filePath);

	virtual QMap<QString, int> getAllInfo();

	virtual QString getFlakonHost();
	virtual QString getFlakonPort();
	virtual QString getAtlantHost();
	virtual QString getAtlantPort();
};

#endif // TCPSETTINGSMANAGER_H
