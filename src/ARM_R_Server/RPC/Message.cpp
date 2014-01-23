#include "Message.h"

#include <QDebug>

Message::Message(const int id, const int type, QByteArray* data)
{
    _id = id;
    _data = data;
    _type = type;
}

Message::~Message()
{
    delete _data;
}

QByteArray* Message::get(int& id, int& type) const
{
    id = _id;
    type = _type;
//    double d = *(double*)(*_data.data());//*(double*)(data);

    return _data;
}
