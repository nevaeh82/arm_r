#ifndef SIGNALDETECTEDDIALOG_H
#define SIGNALDETECTEDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QList>
#include "DBStation/DBFillTables.h"

#include <QSound>

namespace Ui {
class SignalDetectedDialog;
}

class SignalDetectedDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SignalDetectedDialog(QWidget *parent = 0);
	~SignalDetectedDialog();

	void setFrequency(QString name, QList<double> freqs);
    void setNewFreq(QString name, QList<OverthresholdBand> list);
private:
	Ui::SignalDetectedDialog *ui;

signals:
	void onStopSpectrum();
};

#endif // SIGNALDETECTEDDIALOG_H
