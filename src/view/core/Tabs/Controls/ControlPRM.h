#ifndef CONTROLPRM_H
#define CONTROLPRM_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QGroupBox>
#include <QLCDNumber>
#include <QLineEdit>

#include <QMap>
#include <QVariant>

#include <QPropertyAnimation>

#include <QComboBox>

#include "../ITabSpectrum.h"
#include "IControlPRM.h"

#include "../../Common/CommandMessage.h"

class ControlPRM : public QWidget, public IControlPRM
{
    Q_OBJECT
public:
    ControlPRM(QWidget *parent = 0, ITabSpectrum* tab = 0);
    ~ControlPRM();

public:
    virtual void set_status(QMap<int, QVariant> data);

private:
    ITabSpectrum*       _tab;

    QLCDNumber*         _ln_freq;
    QComboBox*          _cb_filter;

    QGroupBox*          _gb_att1;
    QToolButton*        _tb_att1_up;
    QToolButton*        _tb_att1_down;
    QGroupBox*          _gb_att2;
    QToolButton*        _tb_att2_up;
    QToolButton*        _tb_att2_down;
    QLCDNumber*         _ln_att1;
    QLCDNumber*         _ln_att2;


private:
    QWidget             *_panel_controls_widget;
    QVBoxLayout         *_vbox_panel;
    QPropertyAnimation  *_animation;


private:
    void _add_widgets_to_panel(QWidget *);
    void _setAtt1(int value);
    void _setAtt2(int value);

    void leaveEvent(QEvent *);

public slots:
    void slotShow();
    void slotSetSize(int width, int height);

private slots:
    void _slotAtt1Up();
    void _slotAtt1Down();
    void _slotAtt2Up();
    void _slotAtt2Down();
    void _slotSetFilter(int index);
};

#endif // CONTROLPRM_H
