#ifndef IRPCLISTENER_H
#define IRPCLISTENER_H

#include <QObject>

/// TODO: add port identification

class IRpcListener
{
public:
    virtual ~IRpcListener(){}

	virtual void onMethodCalled(const QString& method, const QVariant& arg) = 0;
};


#endif // IRPCLISTENER_H
