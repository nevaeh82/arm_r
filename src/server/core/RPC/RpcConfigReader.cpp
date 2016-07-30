#include <QFile>

#include <Logger/Logger.h>

#include "RpcConfigReader.h"

RpcConfigReader::RpcConfigReader(QObject* parent) :
	QObject(parent)
{
	m_settingsManager = RpcSettingsManager::instance();
	connect(this, SIGNAL(readStationListInternalSignal(QString)), this, SLOT(readStationListInternalSlot(QString)));
	connect(this, SIGNAL(readDbConfigurationInternalSignal(QString)), this, SLOT(readDbConfigurationInternalSlot(QString)));
}

RpcConfigReader::~RpcConfigReader()
{
}

void RpcConfigReader::setRpcServer(IRpcControllerBase* rpcServer)
{
	m_rpcServer = rpcServer;
	m_rpcServer->registerReceiver(this);
}

void RpcConfigReader::onMethodCalled(const QString& method, const QVariant& argument)
{
	QString fileName = argument.toString();

	if (method == RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST) {
		emit readStationListInternalSignal(fileName);
	} else if (method == RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION) {
		emit readDbConfigurationInternalSignal(fileName);
	}
}

void RpcConfigReader::readStationListInternalSlot(const QString& fileName)
{
	if( 0 ) {
		QSettings stationSettings(fileName, QSettings::IniFormat, this);
		stationSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

		QList<StationConfiguration> stationList;
		QStringList childKeys = stationSettings.childGroups();

		log_debug( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" );
		log_debug( childKeys[0] );

		foreach (const QString &childKey, childKeys ) {
			StationConfiguration stationConfiguration;

			stationSettings.beginGroup(childKey);
			stationConfiguration.id				= stationSettings.value("Id").toUInt();
			stationConfiguration.name			= stationSettings.value("Name").toString();
			stationConfiguration.latitude		= stationSettings.value("Latitude").toDouble();
			stationConfiguration.longitude		= stationSettings.value("Longitude").toDouble();
			stationConfiguration.hostPrm300		= stationSettings.value("IPprm300").toString();
			stationConfiguration.hostADC		= stationSettings.value("IPADC").toString();
			stationSettings.endGroup();

			stationList.append(stationConfiguration);
		}

		QByteArray dataToSend;
		QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
		dataStream << stationList;

		m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, QVariant(dataToSend));
	} else {
		emit getStationList();
	}
}

void RpcConfigReader::readDbConfigurationInternalSlot(const QString& filename)
{
	DBConnectionStruct param;
	QSettings dbConfig(filename, QSettings::IniFormat, this);
	param.host		= dbConfig.value("DbStations_Config/host").toString();
	param.port		= dbConfig.value("DbStations_Config/port").toString().toUShort();
	param.login		= dbConfig.value("DbStations_Config/login").toString();
	param.password	= dbConfig.value("DbStations_Config/password").toString();
	param.dbName	= dbConfig.value("DbStations_Config/dbName").toString();

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << param;

	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, QVariant(dataToSend));
}

void RpcConfigReader::inStationsList(const QVariant& dataToSend)
{
	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, dataToSend);
}
