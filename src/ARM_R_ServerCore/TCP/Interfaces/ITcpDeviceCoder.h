#ifndef ITCPDEVICECODER_H
#define ITCPDEVICECODER_H

class QByteArray;
class QObject;

class ITcpDeviceCoder
{

public:
	virtual ~ITcpDeviceCoder() {}

	virtual QByteArray encode(const QByteArray& data)	= 0;
	virtual QByteArray decode(const QByteArray& data)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPDEVICECODER_H
