#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include "../Common/IMessage.h"

class Message : public IMessage
{
public:
    Message(int id, int type, QByteArray* data);
    ~Message();

public:
    virtual QByteArray* get(int& id, int& type) const;

private:
    int                     _id;
    int                     _type;
    QByteArray*             _data;

};

#endif // MESSAGE_H
