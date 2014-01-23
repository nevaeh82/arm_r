#ifndef IMESSAGE_H
#define IMESSAGE_H

#include "../Defines.h"

class QByteArray;

template <typename T>
class IMessage
{
public:
	virtual ~IMessage(){}
	virtual T data() const = 0;
	virtual InternalCommands::TypeCommandEnum type() const = 0;
};

#endif // IMESSAGE_H
