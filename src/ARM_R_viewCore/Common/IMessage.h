#ifndef IMESSAGE_H
#define IMESSAGE_H

/// interfase for pushing message through interfaces

#include <QVariant>

class IMessage
{
public:
    virtual ~IMessage(){};

    virtual int get(QVariant& data) = 0;

    virtual void clenup()           = 0;

};

#endif // IMESSAGE_H
