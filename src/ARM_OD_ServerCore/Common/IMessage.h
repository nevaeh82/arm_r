#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <QByteArray>

class IMessage
{
public:
    virtual ~IMessage(){};
    virtual QByteArray* get(int& id, int& type) const      = 0;
};

#endif // IMESSAGE_H
