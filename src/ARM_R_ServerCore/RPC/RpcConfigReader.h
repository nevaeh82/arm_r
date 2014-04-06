#ifndef RPCCONFIGREADER_H
#define RPCCONFIGREADER_H

#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QList>
#include <QTextCodec>
#include <QStringList>

#include <Interfaces/IRpcListener.h>
#include <Interfaces/IRpcControllerBase.h>

#include "Info/StationConfiguration.h"
#include "Info/AtlantConfiguraton.h"
#include "Info/DBConnectionParameters.h"
#include "Rpc/RpcSettingsManager.h"
#include "Rpc/RpcDefines.h"

#include "Interfaces/IRpcConfigReader.h"

class RpcConfigReader : public QObject, public IRpcConfigReader, public IRpcListener
{
	Q_OBJECT
private:
	IRpcControllerBase* m_rpcServer;
	IRpcSettingsManager* m_settingsManager;

public:
	explicit RpcConfigReader(QObject* parent = NULL);
	virtual ~RpcConfigReader();

	// IRpcConfigReader interface
public:
	virtual void setRpcServer(IRpcControllerBase* rpcServer);

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

signals:
	void readStationListInternalSignal(const QString& fileName);
	void readAtlantConfigurationInternalSignal(const QString& filename);
	void readDbConfigurationInternalSignal(const QString& filename);

private slots:
	void readStationListInternalSlot(const QString& fileName);
	void readAtlantConfigurationInternalSlot(const QString& filename);
	void readDbConfigurationInternalSlot(const QString& filename);
};

#endif // RPCCONFIGREADER_H
