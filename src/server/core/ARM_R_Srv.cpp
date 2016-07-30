#include <Logger/Logger.h>

#include "ARM_R_Srv.h"


ARM_R_Srv::ARM_R_Srv(int serverId, QObject* parent) :
    QObject(parent),
    m_serverId(serverId)
{
	qRegisterMetaType<MessageSP>("MessageSP");
	qRegisterMetaType<DataFromFlacon> ("DataFromFlacon");
	qRegisterMetaType<DataFromRadioLocation> ("DataFromRadioLocation");
	qRegisterMetaType<OneDataFromRadioLocation> ("OneDataFromRadioLocation");

	m_rpcServer = new RpcServer;

	IRpcSettingsManager* rpcSettingsManager = RpcSettingsManager::instance();
	rpcSettingsManager->setIniFile("./Rpc/RpcServer.ini");

	QString host = rpcSettingsManager->getRpcHost();
    quint16 port = rpcSettingsManager->getRpcPort().toUShort() + m_serverId-1;
	m_rpcServer->start(port, QHostAddress(host));

	m_tcpServer = new TcpServerController(this);
	m_tcpServer->createTcpServer();
	m_tcpServer->createTcpServerCoder();
	m_tcpServer->start(QHostAddress::Any, 6662);

    m_tcpManager = new TcpManager(m_serverId);

	QThread* tcpManagerThread = new QThread;
	connect(tcpManagerThread, SIGNAL(finished()), m_tcpManager, SLOT(deleteLater()));
	connect(tcpManagerThread, SIGNAL(finished()), tcpManagerThread, SLOT(deleteLater()));
	m_tcpManager->moveToThread(tcpManagerThread);
	tcpManagerThread->start();

	m_rpcServer->registerReceiver(m_tcpManager, 701);
//	m_rpcServer->registerReceiver(m_tcpManager, 201);

	m_tcpServer->registerReceiver(m_tcpManager);

	m_tcpManager->setRpcServer(m_rpcServer);
	m_tcpManager->setTcpServer(m_tcpServer);

	ITcpSettingsManager* settingsManager = TcpSettingsManager::instance();
    settingsManager->setIniFile("./TCP/coders.ini");
	QMap<QString, int> mapInfo = settingsManager->getAllInfo();

	foreach(QString key, mapInfo.keys())
	{
		log_debug(QString(key));
		m_tcpManager->addTcpDevice(key, mapInfo.value(key));
	}

	RpcConfigReader* rpcConfigReader = new RpcConfigReader(this);
	rpcConfigReader->setRpcServer(m_rpcServer);

	m_tcpManager->setConfigReader(rpcConfigReader);
}

ARM_R_Srv::~ARM_R_Srv()
{

}
