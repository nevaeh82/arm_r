#ifndef PELENGATORFORM_H
#define PELENGATORFORM_H

#include <QWidget>
#include <QTextCodec>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>

#include <QDoubleSpinBox>

#include "ControllerPelengator.h"

namespace Ui {
class PelengatorForm;
}

class PelengatorForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit PelengatorForm(QWidget *parent = 0);
    ~PelengatorForm();
    
private:
    Ui::PelengatorForm      *ui;
    ControllerPelengator    *_contr;
    QLabel                  *_lbl_freq;
    QLabel                  *_lbl_bw;
    QDoubleSpinBox          *_sb_freq;
    QSpinBox                *_sb_bw;
    QPushButton             *_pbStop;

    QGridLayout             *_gl_freq;

    int                     _cur_mode;

public slots:
    void slotAddLog(QString str);
    void slotShow();

    void slotStop();

    void slotSetMode(int mode);

signals:
    void signalAddLog(QString str);
    void signalAdaptation(double, double);
    void signalDuty(double, double);
    void signalCheck(QVector<double>, double);
    void signalWork(QVector<double>, double);
    void signalScanStopRequest();
    void signalDFStopRequest();

};

#endif // PELENGATORFORM_H
