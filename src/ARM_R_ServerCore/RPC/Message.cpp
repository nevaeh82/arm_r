#include "Message.h"

#include <QDebug>

MessageOld::MessageOld(const int id, const int type, QByteArray* data)
{
    _id = id;
    _data = data;
    _type = type;
}

MessageOld::~MessageOld()
{
    delete _data;
}

QByteArray* MessageOld::get(int& id, int& type) const
{
    id = _id;
    type = _type;
//    double d = *(double*)(*_data.data());//*(double*)(data);

    return _data;
}
