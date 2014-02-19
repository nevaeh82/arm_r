#include "CorrelationWidget.h"

#include "ui_CorrelationWidget.h"

CorrelationWidget::CorrelationWidget(QWidget *parent, Qt::WFlags flags, QString name, int id):
	QWidget(parent, flags), ui(new Ui::CorrelationWidget)
{

	ui->setupUi(this);


    _bandwidth = 0;
    _pointCount = 0;
    _isComplex = false;

	//QGridLayout *mainLayout = new QGridLayout(this);
	//mainLayout->setSpacing(0);
	//mainLayout->setContentsMargins(0, 0, 0, 0);


	//_correlationWidget = new Q_MG_SpectrumInterface(this, "Correlations");
//    _correlationWidget->setContextMenu(_graphicsContextMenu);

	//_correlationWidget->setMinimumSize(100, 100);
	ui->spectrumWidget->SetZoomOutMaxOnDataSet(true);
	ui->spectrumWidget->SetAlign(2);

//    float* spectrum = new float[1];
//    spectrum[0] = 0;
////    _correlationWidget->Setup(true,0,"",NULL,0, 0, 0);

//    QString fileNameSpec = "1.pcm";

//    QFile fl(fileNameSpec);
//    if (fl.open(QIODevice::ReadOnly))
//    {
//        unsigned int PointCount = (fl.size()/sizeof(float));
//        if (PointCount < 5) {QMessageBox::critical(this,"File error","Very small file or file not found"); return;}
//        float* spectrum = new float[PointCount];
//        fl.read((char*)spectrum,PointCount*sizeof(float));
//        fl.close();


//        _correlationWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

//        _correlationWidget->ZoomOutFull();

//        delete[] spectrum;
//    }

	//mainLayout->addWidget(_correlationWidget, 0, 0/*, Qt::AlignLeft | Qt::AlignTop*/);


	//this->setLayout(mainLayout);
    this->installEventFilter(this);

    connect(this, SIGNAL(signalSetSignalSetup(float*,float*)), this, SLOT(_slotSetSignalSetup(float*,float*)), Qt::QueuedConnection);
    connect(this, SIGNAL(signalSetSignal(float*, float*)), this, SLOT(_slotSetSignal(float*, float*)), Qt::QueuedConnection);
//    connect(this, SIGNAL(signalSetDefModulation(QString)), this, SLOT(_slotSetDefModulation(QString)), Qt::QueuedConnection);


    connect(this, SIGNAL(signalSetLabelName(QString,QString)), this, SLOT(_slotSetLabelName(QString,QString)));
	ui->spectrumWidget->SetHorizontalLabel(tr("m"));

}

CorrelationWidget::~CorrelationWidget()
{
	//delete _correlationWidget;
}

void CorrelationWidget::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
    _mux.lock();
    _bandwidth = bandwidth;
    _pointCount = PointCount;
	_isComplex = isComplex;

   float maxv = 1.0;
   float minv = 0.0;

	ui->spectrumWidget->SetAutoscaleY(false);
//        spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
	ui->spectrumWidget->SetAlign(3);


	ui->spectrumWidget->SetHorizontalLabel(tr("m"));

	ui->spectrumWidget->Setup(true,_bandwidth,tr("Level"), spectrum, _pointCount, spectrum_peak_hold, _pointCount,false, false, minv, maxv);
	ui->spectrumWidget->SetSpectrumVisible(1, true);
	_mux.unlock();

   // emit signalSetSignalSetup(spectrum, spectrum_peak_hold);
}

void CorrelationWidget::setSignal(float *spectrum, float *spectrum_peak_hold)
{
    emit signalSetSignal(spectrum, spectrum_peak_hold);
}

bool CorrelationWidget::isGraphicVisible()
{
    return isVisible();
}

void CorrelationWidget::setDefModulation(QString modulation)
{
//        emit signalSetDefModulation(modulation);
}

void CorrelationWidget::setLabelName(QString base, QString second)
{
	emit signalSetLabelName(base, second);
}

void CorrelationWidget::clear()
{
//    _correlationWidget->SetSpectrumVisible(1, false);
//    _correlationWidget->Setup(false, 0, "", NULL, 0, NULL, 0, 0,0,0,0);
//    _correlationWidget->Reset();
//    _correlationWidget->SetLabel(0, "No Signal");
}

void CorrelationWidget::_slotSetSignalSetup(float *spectrum, float *spectrum_peak_hold)
{
  /*  _mux.lock();

       float maxv = 1.0;
       float minv = 0.0;

		ui->spectrumWidget->SetAutoscaleY(false);
//        spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
		ui->spectrumWidget->SetAlign(3);


	ui->spectrumWidget->SetHorizontalLabel(tr("m"));

	ui->spectrumWidget->Setup(true,_bandwidth,tr("Level"), spectrum, _pointCount, spectrum_peak_hold, _pointCount,false, false, minv, maxv);
	ui->spectrumWidget->SetSpectrumVisible(1, true);
	_mux.unlock();*/
}

void CorrelationWidget::_slotSetSignal(float *spectrum, float *spectrum_peak_hold)
{
    _mux.lock();

       float maxv = 1.0;
       float minv = 0.0;

//        _correlationWidget->SetAutoscaleY(false);
//        spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
//        _correlationWidget->SetAlign(3);

    _mux.unlock();
	ui->spectrumWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
}

void CorrelationWidget::_slotSetLabelName(QString base, QString second)
{

	QString name = base + " - " + second;
	if(_label_name != name)
	{
		_label_name = name;
		ui->spectrumWidget->ClearAllLabels();
		ui->spectrumWidget->SetLabel(0, _label_name);
	}
}

Q_DECLARE_METATYPE(float*)
void CorrelationWidget::onDataArrived(const QString &method, const QVariant &arg)
{
	QList<QVariant> list = arg.toList();
	float* spectrum = list.at(0).value<float*>();
	float* spectrumPeakHold = (float*)list.at(1).value<float*>();

	if (list.count() == 4){
		_slotSetSignal(spectrum, spectrumPeakHold);
	} else {
		int pointCount = list.at(2).toInt();
		double bandwidth = list.at(3).toDouble();
		bool isComplex = list.at(4).toBool();
		setSignalSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
	}

	QString base = list.at(list.size() - 2).toString();
	QString second = list.at(list.size() - 1).toString();

	setLabelName(base, second);


	//setCorData(list.at(1).toUInt(), list.at(0).toByteArray(), true);

}

void CorrelationWidget::onDataArrived(float *spectrum, float *spectrumPeakHold, int pointCount, double bandwidth, bool isComplex)
{
	//setSignalSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
}

void CorrelationWidget::onDataArrived(float *spectrum, float *spectrumPeakHold)
{
	//_slotSetSignal(spectrum, spectrumPeakHold);
}
