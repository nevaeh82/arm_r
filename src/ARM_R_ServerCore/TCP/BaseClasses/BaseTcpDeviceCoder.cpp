#include "BaseTcpDeviceCoder.h"

BaseTcpDeviceCoder::BaseTcpDeviceCoder(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
}

BaseTcpDeviceCoder::~BaseTcpDeviceCoder()
{
}

IMessage<QByteArray>* BaseTcpDeviceCoder::encode(const QByteArray& data)
{
	return new Message<QByteArray>(TCP_EMPTY_MESSAGE, data);
}

QByteArray BaseTcpDeviceCoder::decode(const IMessage<QByteArray>* message)
{
	return message->data();
}

QObject* BaseTcpDeviceCoder::asQObject()
{
	return this;
}
