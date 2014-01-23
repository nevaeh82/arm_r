#ifndef ISIMPLECODER_H
#define ISIMPLECODER_H

#include <QByteArray>
#include "Message.h"

class ISimpleCoder
{
public:
	virtual ~ISimpleCoder(){}

	virtual QByteArray encode(const IMessage<QByteArray>* message) const = 0;
	virtual IMessage<QByteArray>* decode(const QByteArray& data) = 0;
};

#endif // ISIMPLECODER_H
