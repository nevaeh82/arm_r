#ifndef IMESSAGE_OLD_H
#define IMESSAGE_OLD_H

#include <QByteArray>

class IMessageOld
{
public:
	virtual ~IMessageOld(){};
    virtual QByteArray* get(int& id, int& type) const      = 0;
};

#endif // IMESSAGE_OLD_H
