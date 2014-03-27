#ifndef ICONTROLLERPELENGATOR_H
#define ICONTROLLERPELENGATOR_H

#include <QString>
#include <QByteArray>

class IControllerPelengator
{
public:
    virtual ~IControllerPelengator(){};
    virtual void addLog(QString log)                                        = 0;
    virtual void setCon(QString host, unsigned short port, QString name)    = 0;
    virtual void send_msg(int id, QByteArray* data)                                 = 0;
};

#endif // ICONTROLLERPELENGATOR_H
