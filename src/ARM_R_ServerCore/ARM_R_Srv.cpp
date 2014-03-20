#include "ARM_R_Srv.h"

Pw::Logger::ILogger* ARM_R_Srv::m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(ARM_R_Srv));

ARM_R_Srv::ARM_R_Srv(QObject* parent) :
	QObject(parent)
{
	qRegisterMetaType<MessageSP>("MessageSP");

	m_rpcServer = new RPCServer;
	m_rpcServer->start(24500, QHostAddress("127.0.0.1"));

	m_tcpServer = new TcpServerController(ARMR_TCP_SERVER);
	m_tcpServer->start(QHostAddress::Any, 6662);

	m_tcpManager = new TcpManager;

	QThread* tcpManagerThread = new QThread;
	connect(tcpManagerThread, SIGNAL(finished()), m_tcpManager, SLOT(deleteLater()));
	connect(tcpManagerThread, SIGNAL(finished()), tcpManagerThread, SLOT(deleteLater()));
	m_tcpManager->moveToThread(tcpManagerThread);
	tcpManagerThread->start();

	m_rpcServer->registerReceiver(m_tcpManager);
	m_tcpServer->registerReceiver(m_tcpManager);

	m_tcpManager->setRpcServer(m_rpcServer);
	m_tcpManager->setTcpServer(m_tcpServer);

	ITcpSettingsManager* settingsManager = new TcpSettingsManager;
	settingsManager->setIniFile("./TCP/coders.ini");

	QString host = settingsManager->getFlakonHost();
	quint32 port = settingsManager->getFlakonPort().toUInt();
	m_tcpManager->addTcpDevice(FLAKON_TCP_DEVICE, host, port);

	host = settingsManager->getAtlantHost();
	port = settingsManager->getAtlantPort().toUInt();
	m_tcpManager->addTcpDevice(ATLANT_TCP_DEVICE, host, port);

	delete settingsManager;
}

ARM_R_Srv::~ARM_R_Srv()
{

}

