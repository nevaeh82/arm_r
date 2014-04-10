#ifndef SIGNALDETECTEDDIALOG_H
#define SIGNALDETECTEDDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class SignalDetectedDialog;
}

class SignalDetectedDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SignalDetectedDialog(QWidget *parent = 0);
	~SignalDetectedDialog();

	void setFrequency(double freq);
private:
	Ui::SignalDetectedDialog *ui;

signals:
	void signalContinueSpectrum();
	void signalStopSpectrum();
};

#endif // SIGNALDETECTEDDIALOG_H
