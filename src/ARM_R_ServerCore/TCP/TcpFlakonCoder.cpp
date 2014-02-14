#include "TcpFlakonCoder.h"

TcpFlakonCoder::TcpFlakonCoder(QObject* parent) :
	BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonCoder)), parent)
{
}

TcpFlakonCoder::~TcpFlakonCoder()
{
}

QByteArray TcpFlakonCoder::encode(const QByteArray& data)
{
	/// TODO
}

QByteArray TcpFlakonCoder::decode(const QByteArray& data)
{
	/// TODO
}

QObject* TcpFlakonCoder::asQObject()
{
	return this;
}
