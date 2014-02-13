#ifndef BASETCPDEVICECODER_H
#define BASETCPDEVICECODER_H

#include <QObject>
#include <PwLogger/PwLogger.h>

#include "../Interfaces/ITcpDeviceCoder.h"

class BaseTcpDeviceCoder : public QObject, public ITcpDeviceCoder
{
	Q_OBJECT

protected:
	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpDeviceCoder(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceCoder();

	// ITcpDeviceCoder interface
public:
	virtual QByteArray encode(const QByteArray& data);
	virtual QByteArray decode(const QByteArray& data);
	virtual QObject* asQObject();
};

#endif // BASETCPDEVICECODER_H
