#ifndef MESSAGESP_H
#define MESSAGESP_H

#include <QSharedPointer>

#include <Templates/Message.h>

typedef QSharedPointer<IMessage<QByteArray>> MessageSP;

#endif // MESSAGESP_H
