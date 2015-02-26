#ifndef ICONTROLLERPELENGATOR_H
#define ICONTROLLERPELENGATOR_H

#include <QString>

class IControllerPelengator
{
public:
    virtual ~IControllerPelengator(){};
    virtual void addLog(QString)        = 0;
};

#endif // ICONTROLLERPELENGATOR_H
