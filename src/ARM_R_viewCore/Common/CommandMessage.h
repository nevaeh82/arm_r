#ifndef COMMANDMESSAGE_H
#define COMMANDMESSAGE_H

#include <QVariant>

#include "IMessage.h"

#define COMMAND_TOBLACKLIST     201
#define COMMAND_TOWHITELIST     202
#define COMMAND_RECOGNIZESIGNAL 203
#define COMMAND_KM              204

#define COMMAND_FLAKON_SET_MAIN_STATION_COR 300
#define COMMAND_PRM_SET_FREQ                301
#define COMMAND_PRM_REQUEST_FREQ            302
#define COMMAND_PRM_SET_ATT1                303
#define COMMAND_PRM_SET_ATT2                304
#define COMMAND_PRM_SET_FILTER              305
#define COMMAND_FLAKON_SET_AVARAGE          306

#define COMMAND_ATLANT_SET_FREQ             401

#define COMMAND_REQUEST_STATUS              500


class CommandMessage : public IMessage
{
public:
    CommandMessage(int type, QVariant data);
    ~CommandMessage();

    virtual int get(QVariant &data);

    virtual void clenup();

private:
    int         _type;
    QVariant    _data;
};

#endif // COMMANDMESSAGE_H
