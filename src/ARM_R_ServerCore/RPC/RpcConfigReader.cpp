#include "RpcConfigReader.h"

RpcConfigReader::RpcConfigReader(QObject* parent) :
	QObject(parent)
{
	m_settingsManager = RpcSettingsManager::instance();
	connect(this, SIGNAL(readStationListInternalSignal(QString)), this, SLOT(readStationListInternalSlot(QString)));
	connect(this, SIGNAL(readAtlantConfigurationInternalSignal(QString)), this, SLOT(readAtlantConfigurationInternalSlot(QString)));
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
	} else if (method == RPC_METHOD_CONFIG_REQUEST_GET_ATLANT_CONFIGURATION) {
		emit readAtlantConfigurationInternalSignal(fileName);
	} else if (method == RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION) {
		emit readDbConfigurationInternalSignal(fileName);
	}
}

void RpcConfigReader::readStationListInternalSlot(const QString& fileName)
{
	QSettings stationSettings(fileName, QSettings::IniFormat, this);
	stationSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	QList<StationConfiguration> stationList;

	QStringList childKeys = stationSettings.childGroups();

	foreach (const QString &childKey, childKeys ) {

		StationConfiguration stationConfiguration;

		stationSettings.beginGroup(childKey);
		stationConfiguration.id				= stationSettings.value("Id", 0).toUInt();
		stationConfiguration.name			= stationSettings.value("Name", "Unknown").toString();
		stationConfiguration.latitude		= stationSettings.value("Latitude", 0.0).toDouble();
		stationConfiguration.longitude		= stationSettings.value("Longitude", 0.0).toDouble();
		stationConfiguration.hostPrm300		= stationSettings.value("IPprm300", "127.0.0.1").toString();
		stationConfiguration.hostADC		= stationSettings.value("IPADC", "127.0.0.1").toString();
		stationConfiguration.portADC		= stationSettings.value("portADC", 1030).toUInt();
		stationSettings.endGroup();

		stationList.append(stationConfiguration);
	}

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << stationList;

	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, QVariant(dataToSend));
}

void RpcConfigReader::readAtlantConfigurationInternalSlot(const QString& filename)
{
	m_settingsManager->setIniFile(filename);
	AtlantConfiguration atlantConfiguration;

	atlantConfiguration.hostByRpc	= m_settingsManager->getRpcHost();
	atlantConfiguration.portByRpc	= m_settingsManager->getRpcPort().toUInt();

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << atlantConfiguration;

	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION, QVariant(dataToSend));
}

void RpcConfigReader::readDbConfigurationInternalSlot(const QString& filename)
{
	DBConnectionStruct param;
	QSettings dbConfig(filename, QSettings::IniFormat, this);
	param.host		= dbConfig.value("DbStations_Config/host", "127.0.0.1").toString();
	param.port		= dbConfig.value("DbStations_Config/port", "3306").toString().toUShort();
	param.login		= dbConfig.value("DbStations_Config/login", "root").toString();
	param.password	= dbConfig.value("DbStations_Config/password", "qwerty12345").toString();
	param.dbName	= dbConfig.value("DbStations_Config/dbName", "Stations").toString();

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << param;

	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, QVariant(dataToSend));
}