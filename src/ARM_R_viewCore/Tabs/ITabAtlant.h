#ifndef ITABATLANT_H
#define ITABATLANT_H

#include <QByteArray>

class ITabAtlant
{
public:
	virtual ~ITabAtlant(){}
    virtual void setLog(QByteArray data)       = 0;
};

#endif // ITABATLANT_H
