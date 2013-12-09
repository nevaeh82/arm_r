#ifndef TABATLANT_H
#define TABATLANT_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>

#include "TabsProperty.h"
#include "RPC/RPCAtlant.h"

#include "ITabAtlant.h"

class TabAtlant : public QWidget, public ITabAtlant
{
    Q_OBJECT
public:
    TabAtlant(/*TabsProperty* prop*/);
    ~TabAtlant();

public:
    virtual void setLog(QByteArray data);

private:
    int                 _id;
    QComboBox*          _cb_mode;
    QDoubleSpinBox*     _dsb_freq;
    QDoubleSpinBox*     _dsb_width;
    QPushButton*        _pb_send;
    TabsProperty*       _tab_property;
    RPCAtlant*          _rpc_client;
    QTextEdit*          _txt_log;



private:
    int createRPC();
    int closeRPC();

private slots:
    void _slotStart();

    void _slot_send();

signals:
    void signalFinishRPC();
    void signalStartRPC();
    void signalStopRPC();

signals:
    void signalAddLog(QString str);

};

#endif // TABATLANT_H
