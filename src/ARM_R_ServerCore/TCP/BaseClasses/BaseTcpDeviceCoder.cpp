#include "BaseTcpDeviceCoder.h"

BaseTcpDeviceCoderOLD::BaseTcpDeviceCoderOLD(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
}

BaseTcpDeviceCoderOLD::~BaseTcpDeviceCoderOLD()
{
}

MessageSP BaseTcpDeviceCoderOLD::encode(const QByteArray& data)
{
	return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, data));
}

QByteArray BaseTcpDeviceCoderOLD::decode(const MessageSP message)
{
	return message->data();
}

QObject* BaseTcpDeviceCoderOLD::asQObject()
{
	return this;
}
