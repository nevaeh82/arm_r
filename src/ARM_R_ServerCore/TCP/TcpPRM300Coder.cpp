#include "TcpPRM300Coder.h"

TcpPRM300Coder::TcpPRM300Coder(QObject* parent) :
	BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Coder)), parent)
{
}

TcpPRM300Coder::~TcpPRM300Coder()
{
}

IMessage<QByteArray>* TcpPRM300Coder::encode(const QByteArray& data)
{
	/// TODO
}

QByteArray TcpPRM300Coder::decode(const IMessage<QByteArray>* message)
{
	/// TODO
}

QObject* TcpPRM300Coder::asQObject()
{
	return this;
}
