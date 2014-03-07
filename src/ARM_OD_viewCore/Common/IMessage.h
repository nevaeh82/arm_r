#ifndef IMESSAGE_H
#define IMESSAGE_H

/// interfase for pushing message through interfaces

#include <QVariant>
#include <QByteArray>

class IMessage
{
public:
    virtual ~IMessage(){};

    virtual int get(QByteArray& data) = 0;

    virtual void clenup()           = 0;

};

#endif // IMESSAGE_H
