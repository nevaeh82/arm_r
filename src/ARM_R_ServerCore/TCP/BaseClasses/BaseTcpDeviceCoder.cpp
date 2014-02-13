#include "BaseTcpDeviceCoder.h"

BaseTcpDeviceCoder::BaseTcpDeviceCoder(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
}

BaseTcpDeviceCoder::~BaseTcpDeviceCoder()
{
}

QByteArray BaseTcpDeviceCoder::encode(const QByteArray& data)
{
	return data;
}

QByteArray BaseTcpDeviceCoder::decode(const QByteArray& data)
{
	return data;
}

QObject* BaseTcpDeviceCoder::asQObject()
{
	return this;
}
