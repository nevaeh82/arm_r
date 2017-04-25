#include "TcpRDSSettingsController.h"

TcpRDSSettingsController::TcpRDSSettingsController(QObject* parent) :
	TcpDeviceController(parent)
{
//    QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
//    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
}

TcpRDSSettingsController::~TcpRDSSettingsController()
{

}

void TcpRDSSettingsController::createTcpDeviceCoder()
{
	log_debug("Creating TcpRDSCoder...");
	TcpRdsSettingsCoder* coder = new TcpRdsSettingsCoder(this);
	m_tcpDeviceCoder = coder;

	m_coder = coder;
}

void TcpRDSSettingsController::createTcpClient()
{

}

QObject *TcpRDSSettingsController::asQObject()
{
	return this;
}

bool TcpRDSSettingsController::init()
{
	return false;
}

QByteArray TcpRDSSettingsController::getFullInfo()
{
	return QByteArray();
}

RpcRoutedServer::RouteId TcpRDSSettingsController::getRouteId() const
{
	return 0;//RDS_SETTINGS_ID;
}

void TcpRDSSettingsController::onMethodCalled(const QString &method, const QVariant &argument)
{
	QByteArray data = argument.toByteArray();

	if(method == RPC_METHOD_SEND_CPPACKET_PROTO) {
		sendData( MessageSP( new Message<QByteArray>( TCP_CCPACKET_SEND_PROTO, data ) ) );
	}

}

QByteArray TcpRDSSettingsController::getSettings(int id)
{
	return m_coder->encode(id);
}

