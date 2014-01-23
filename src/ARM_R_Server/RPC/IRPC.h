#ifndef IRPC_H
#define IRPC_H

#include <QtGlobal>

class IClient;

class IRPC
{
public:
    virtual ~IRPC(){};

    virtual int start()                                 = 0;
    virtual int stop()                                  = 0;
    virtual quint64 get_client_id(IClient* client)      = 0;


};

#endif // IRPC_H
