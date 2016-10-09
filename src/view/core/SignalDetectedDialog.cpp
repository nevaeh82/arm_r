#include "SignalDetectedDialog.h"
#include "ui_SignalDetectedDialog.h"

SignalDetectedDialog::SignalDetectedDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SignalDetectedDialog)
{
	ui->setupUi(this);
	QPushButton* btnOk = ui->buttonBox->button(QDialogButtonBox::Ok);
	btnOk->setText(tr("Ok"));

	QPushButton* btnCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
	btnCancel->setText(tr("Cancel"));

	Qt::WindowFlags flags=windowFlags();
	Qt::WindowFlags helpFlag=Qt::WindowContextHelpButtonHint;
	flags=flags&(~helpFlag);

	setWindowFlags(flags);
	setWindowIcon(QIcon(":/images/icons/ListsDialog.png"));

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);
	metaInformationResolver = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);
}

SignalDetectedDialog::~SignalDetectedDialog()
{
	delete ui;
}

void SignalDetectedDialog::setFrequency(QString name, QList<double> freqs)
{
	ui->teDetected->clear();

	Phonon::MediaSource source("./audio/alarm.mp3");

	mediaObject->setCurrentSource(source);

	mediaObject->play();



//	Phonon::MediaObject *music =
//		 Phonon::createPlayer(Phonon::MusicCategory,
//							  Phonon::MediaSource("./audio/alarm.mp3"));
//	 music->play();

	ui->leName->setText(name);
	foreach (double freq, freqs) {
		QString value = QString("%1 MHz").arg(QString::number(freq, 'f', 4));
		ui->teDetected->append(value);
	}
    //	ui->dspFreq->setValue(freq);
}

void SignalDetectedDialog::setNewFreq(QString name, QList<OverthresholdBand> list)
{
    ui->teDetected->clear();

    Phonon::MediaSource source("./audio/alarm.mp3");

    mediaObject->setCurrentSource(source);

    mediaObject->play();



//	Phonon::MediaObject *music =
//		 Phonon::createPlayer(Phonon::MusicCategory,
//							  Phonon::MediaSource("./audio/alarm.mp3"));
//	 music->play();

    ui->leName->setText(name);
    foreach (OverthresholdBand overs, list) {
        double band = overs.finishFreq - overs.startFreq;
        double middle = band/2 + overs.startFreq;
        QString value = QString("Freq = %1, Band = %2 MHz").arg(QString::number(middle, 'f', 4)).arg(QString::number(band, 'f', 4));
        ui->teDetected->append(value);
    }
}
