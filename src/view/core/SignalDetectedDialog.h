#ifndef SIGNALDETECTEDDIALOG_H
#define SIGNALDETECTEDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QList>
//#include <phonon/audiooutput.h>
//#include <phonon/seekslider.h>
//#include <phonon/mediaobject.h>
//#include <phonon/volumeslider.h>
//#include <phonon/backendcapabilities.h>

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
private:
	Ui::SignalDetectedDialog *ui;

//private:
//	Phonon::MediaObject *mediaObject;
//	Phonon::MediaObject *metaInformationResolver;
//	Phonon::AudioOutput *audioOutput;
};

#endif // SIGNALDETECTEDDIALOG_H
