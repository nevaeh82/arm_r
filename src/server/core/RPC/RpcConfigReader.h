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
#include <SettingsManager/RpcSettingsManager.h>

#include "Info/StationConfiguration.h"
#include "Info/DBConnectionParameters.h"
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
	void readDbConfigurationInternalSignal(const QString& filename);

	void getStationList();

private slots:
	void readStationListInternalSlot(const QString& fileName);
	void readDbConfigurationInternalSlot(const QString& filename);

public slots:
	void inStationsList(const QVariant& dataToSend);
};

#endif // RPCCONFIGREADER_H
