#include "TcpAtlantCoder.h"

TcpAtlantCoder::TcpAtlantCoder(QObject* parent) :
	BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpAtlantCoder)), parent)
{
}


QByteArray TcpAtlantCoder::encode(const QByteArray& data)
{
	/// TODO
}

QByteArray TcpAtlantCoder::decode(const QByteArray& data)
{
	/// TODO
}

QObject* TcpAtlantCoder::asQObject()
{
	return this;
}
