#include "CommandMessage.h"

CommandMessage::CommandMessage(int type, QVariant data)
{
    _type = type;
    _data = data;
}

CommandMessage::~CommandMessage()
{
}

int CommandMessage::get(QVariant &data)
{
    data = _data;
    return _type;
}

void CommandMessage::clenup()
{
    delete this;
}
