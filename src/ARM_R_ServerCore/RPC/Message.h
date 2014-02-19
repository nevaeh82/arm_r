#ifndef MESSAGE_OLD_H
#define MESSAGE_OLD_H

#include <QByteArray>
#include "../Common/IMessage.h"

class MessageOld : public IMessageOld
{
public:
	MessageOld(int id, int type, QByteArray* data);
	~MessageOld();

public:
    virtual QByteArray* get(int& id, int& type) const;

private:
    int                     _id;
    int                     _type;
    QByteArray*             _data;

};

#endif // MESSAGE_OLD_H
