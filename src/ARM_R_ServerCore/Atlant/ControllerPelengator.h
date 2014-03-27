#ifndef CONTROLLERPELENGATOR_H
#define CONTROLLERPELENGATOR_H

#include <QObject>
#include <QString>
#include <QVector>

#include <QDebug>

#include "EMS/pch.h"
#include "EMS/StormEmsClient.h"

#include "IControllerPelengator.h"

#include "../Common/IRouter.h"
#include "../Common/ISubscriber.h"
#include "../Common/IClient.h"
#include "../RPC/Message.h"

#include "../TCP/ITCPController.h"
#include "../TCP/ParserAtlant.h"

class ControllerPelengator : public QObject, public IControllerPelengator
{
    Q_OBJECT
public:
    ControllerPelengator(QString host, unsigned short port, QString name, IRouter* router);
    ~ControllerPelengator();

public:
    static ControllerPelengator *_instance;
    void setInstance(IControllerPelengator *inst);
    static IControllerPelengator *instance()
    {
        return _instance;
    }
    void setCon(QString host, unsigned short port, QString name);
    virtual void send_msg(int id, QByteArray *data);


private:
    EMS::Storm::StormEmsClient emsClient;
    int _id_requestion;
    int _id_diap_cur;
    QString         _host;
    unsigned short  _port;
    QString         _name;
    IRouter*        _router;
    ISubscriber*    _subscriber;
    ITCPController* _tcp_controller;


private:
    static void DirectionAnswerMessageHandler(::Storm::DirectionAnswerMessage& message);
    static void PositionAnswerMessageHandler(::Storm::PositionAnswerMessage& message);
    static void StatusMessageHandler(::Storm::StatusMessage& message);

public slots:
    void slotSetMode(int mode);
    void slotAdaptation(double freq, double brandwidth);
    void slotDuty(double freq, double brandwidth);
    void slotCheck(QVector<double> freqs, double brandwidth);
    void slotWork(QVector<double> freqs, double brandwidth);
    void slotStopScan();
    void slotStopDF();

private:
    void addLog(QString msg);

signals:
    void signalLog(QString msg);
};

#endif // CONTROLLERPELENGATOR_H
