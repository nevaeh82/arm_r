#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    ControlPanel(QWidget* parent);
    ~ControlPanel();

private:
    QWidget*                    _panel_widget;
    QCheckBox*                  _chb_auto_search_common;
    QSpinBox*                   _sb_from_freq;
    QSpinBox*                   _sb_to_freq;
    QCheckBox*                  _chb_panorama_common;
    QSpinBox*                   _sb_freq;
    QPushButton*                _pb_set_freq;
};

#endif // CONTROLPANEL_H
