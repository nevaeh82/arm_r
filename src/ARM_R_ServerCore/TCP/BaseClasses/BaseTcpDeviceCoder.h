#ifndef BASETCPDEVICECODEROLD_H
#define BASETCPDEVICECODEROLD_H

#include <QObject>
#include <PwLogger/PwLogger.h>

#include "../Interfaces/ITcpDeviceCoder.h"
#include "../TcpDefines.h"

class BaseTcpDeviceCoderOLD : public QObject, public ITcpDeviceCoder
{
	Q_OBJECT

protected:
	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpDeviceCoderOLD(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceCoderOLD();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();
};

#endif // BASETCPDEVICECODEROLD_H
