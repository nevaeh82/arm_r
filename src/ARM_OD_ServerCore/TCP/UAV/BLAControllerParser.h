#ifndef BLACONTROLLERPARSER_H
#define BLACONTROLLERPARSER_H

#include <QObject>
#include <QByteArray>
#include "../ITCPParser.h"
#include "../../Common/IRouter.h"
#include "IBLAController.h"

class BLAControllerParser : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    BLAControllerParser(IBLAController *controller);
    ~BLAControllerParser();

public:
    virtual void set_router(IRouter *router);
    virtual void parsing(QByteArray data);
    virtual void distruct();

private:
    IRouter         *_router;
    IBLAController  *_controller;
};

#endif // BLACONTROLLERPARSER_H
