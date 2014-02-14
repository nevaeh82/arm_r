#include "TcpPRM300Controller.h"

TcpPRM300Controller::TcpPRM300Controller(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Controller)), parent)
{
	m_tcpDeviceCoder = TcpPRM300Coder(this);
}

TcpPRM300Controller::~TcpPRM300Controller()
{
}

QObject* TcpPRM300Controller::asQObject()
{
	return this;
}

void TcpPRM300Controller::onDataReceived(const QVariant& argument)
{
	IMessage<QByteArray>* encodedData = m_tcpDeviceCoder->encode(argument.toByteArray());

	foreach(ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(PRM300_TCP_DEVICE, QVariant(encodedData));
	}
}
