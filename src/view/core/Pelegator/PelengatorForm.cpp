#include "PelengatorForm.h"
#include "ui_PelengatorForm.h"

PelengatorForm::PelengatorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PelengatorForm)
{
    ui->setupUi(this);
    _cur_mode = 0;

    _contr = new ControllerPelengator("127.0.0.1", 1195, "Detector");

	/*QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);*/

    this->setWindowTitle(tr("Настройка пеленгаторов"));
    QGridLayout *gl = new QGridLayout;
    QLabel *lbl1 = new QLabel(tr("Режим"));
    gl->addWidget(lbl1, 0, 0, Qt::AlignTop | Qt::AlignLeft);

    QComboBox *cmb = new QComboBox();
    cmb->addItem(tr("Адаптация"));
    cmb->addItem(tr("Дежурство"));
    cmb->addItem(tr("Проверка"));
    cmb->addItem(tr("Работа"));

    gl->addWidget(cmb, 1, 0, Qt::AlignTop | Qt::AlignLeft);

    _pbStop = new QPushButton("Stop");
    gl->addWidget(_pbStop, 1, 1, Qt::AlignTop | Qt::AlignLeft);



    QTextEdit *txt_log = new QTextEdit();
    txt_log->setMinimumSize(300, 300);
    connect(this, SIGNAL(signalAddLog(QString)), txt_log, SLOT(append(QString)));
    gl->addWidget(txt_log, 2, 0);
    this->setLayout(gl);

    connect(cmb, SIGNAL(activated(int)), this, SLOT(slotSetMode(int)));

    _lbl_freq = new QLabel(tr("Частота"));
    _lbl_bw = new QLabel(tr("Полоса"));
    _sb_freq = new QDoubleSpinBox();
    _sb_bw = new QSpinBox();

    /// frequency in MHz
    _sb_freq->setRange(800, 3000);
    _sb_bw->setRange(0, 2000);

    _gl_freq = new QGridLayout;

    _gl_freq->addWidget(_lbl_freq, 0, 0, Qt::AlignTop | Qt::AlignCenter);
    _gl_freq->addWidget(_lbl_bw, 0, 1, Qt::AlignTop | Qt::AlignCenter);
    _gl_freq->addWidget(_sb_freq, 1, 0, Qt::AlignTop | Qt::AlignCenter);
    _gl_freq->addWidget(_sb_bw, 1, 1, Qt::AlignTop | Qt::AlignCenter);

    gl->addLayout(_gl_freq, 1, 2, Qt::AlignTop | Qt::AlignLeft);

    connect(this, SIGNAL(signalAdaptation(double,double)), _contr, SLOT(slotAdaptation(double,double)));
    connect(this, SIGNAL(signalDuty(double,double)), _contr, SLOT(slotDuty(double,double)));
    connect(this, SIGNAL(signalCheck(QVector<double>,double)), _contr, SLOT(slotCheck(QVector<double>,double)));
    connect(this, SIGNAL(signalWork(QVector<double>,double)), _contr, SLOT(slotWork(QVector<double>,double)));

    connect(this, SIGNAL(signalScanStopRequest()), _contr, SLOT(slotStopScan()));
    connect(this, SIGNAL(signalDFStopRequest()), _contr, SLOT(slotStopDF()));

    connect(_contr, SIGNAL(signalLog(QString)), this, SLOT(slotAddLog(QString)));

    connect(_pbStop, SIGNAL(clicked()), this, SLOT(slotStop()));
}

PelengatorForm::~PelengatorForm()
{
    delete ui;
}

void PelengatorForm::slotAddLog(QString str)
{
    emit signalAddLog(str);
}

/// display form
void PelengatorForm::slotShow()
{
    this->show();
    this->raise();
}

void PelengatorForm::slotSetMode(int mode)
{
//    QGridLayout *gl = this->layout();
    switch(_cur_mode)
    {
    case 0:
        emit signalScanStopRequest();
        break;
    case 1:
        emit signalScanStopRequest();
        break;
    case 2:
        emit signalDFStopRequest();
        break;
    case 3:
        emit signalDFStopRequest();
        break;
    default:
        break;
    }

    QVector<double> freqs;
    switch(mode)
    {
    case 0:
        _gl_freq->removeWidget(_sb_freq);
        _gl_freq->removeWidget(_sb_bw);

        _gl_freq->addWidget(_sb_freq, 1, 0, Qt::AlignTop | Qt::AlignCenter);
        _gl_freq->addWidget(_sb_bw, 1, 1, Qt::AlignTop | Qt::AlignCenter);
        emit signalAdaptation(_sb_freq->value(), _sb_bw->value());
        break;
    case 1:
        _gl_freq->removeWidget(_sb_freq);
        _gl_freq->removeWidget(_sb_bw);

        _gl_freq->addWidget(_sb_freq, 1, 0, Qt::AlignTop | Qt::AlignCenter);
        _gl_freq->addWidget(_sb_bw, 1, 1, Qt::AlignTop | Qt::AlignCenter);
        emit signalDuty(_sb_freq->value(), _sb_bw->value());
        break;
    case 2:
        _gl_freq->removeWidget(_sb_freq);
        _gl_freq->removeWidget(_sb_bw);

        _gl_freq->addWidget(_sb_freq, 1, 0, Qt::AlignTop | Qt::AlignCenter);
        _gl_freq->addWidget(_sb_bw, 1, 1, Qt::AlignTop | Qt::AlignCenter);

        freqs.push_back(_sb_freq->value());
        emit signalCheck(freqs, _sb_bw->value());
        break;
    case 3:
        _gl_freq->removeWidget(_sb_freq);
        _gl_freq->removeWidget(_sb_bw);

        _gl_freq->addWidget(_sb_freq, 1, 0, Qt::AlignTop | Qt::AlignCenter);
        _gl_freq->addWidget(_sb_bw, 1, 1, Qt::AlignTop | Qt::AlignCenter);

        freqs.push_back(_sb_freq->value());
        emit signalWork(freqs, _sb_bw->value());
        break;
    default:
        break;
    }
    _cur_mode = mode;
}

void PelengatorForm::slotStop()
{
    emit signalScanStopRequest();
    emit signalDFStopRequest();
}
