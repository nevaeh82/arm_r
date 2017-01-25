#include "CorrelationWidgetController.h"
#include "CorrelationWidget.h"

#include "Logger/Logger.h"

#define DOPPLER_LIMIT 60

CorrelationWidgetController::CorrelationWidgetController(int corType, QObject *parent)
	: QObject(parent)
	, m_view(0)
	, m_bandwidth(0)
	, m_pointCount(0)
	, m_isComplex(0)
	, m_type(corType)
	, m_locationController(NULL)
{
	connect(this, SIGNAL(signalonDataArrivedLS(QString,QVariant)), this, SLOT(onDataArrivedLS(QString,QVariant)));
	connect(this, SIGNAL(signalOnVisible(bool)), this, SLOT(onVisible(bool)));
	connect(this, SIGNAL(signalClearDopler()), this, SLOT(clearDoplerInternal()));

	m_doplerLim = DOPPLER_LIMIT;
}

CorrelationWidgetController::~CorrelationWidgetController()
{
	//log_debug("<<<<<<<<");
}

QWidget *CorrelationWidgetController::getWidget() const
{
	return m_view;
}

void CorrelationWidgetController::setVisible(const bool isVisible)
{
    emit signalOnVisible(isVisible);
}

void CorrelationWidgetController::setLabels(QString l1, QString l2)
{
	setLabelName(l1, l2);
}

void CorrelationWidgetController::setLocationController(LocationSetupWidgetController *controller)
{
	m_locationController = controller;

	if(m_type != 1) {
		connect(this, SIGNAL(onGraphReady()), m_locationController, SLOT(slotPlotDrawComplete()));
	}
}

void CorrelationWidgetController::setAlarm(bool)
{

}

void CorrelationWidgetController::clearDopler()
{
	emit signalClearDopler();
}

void CorrelationWidgetController::clearDoplerInternal()
{
	m_dopplerList.clear();
}

void CorrelationWidgetController::onVisible(const bool b)
{
	m_view->setVisible(b);
}

bool CorrelationWidgetController::isGraphicVisible()
{
	return m_view->isVisible();
}

quint32 CorrelationWidgetController::getId()
{
	return 0;
}

void CorrelationWidgetController::setZeroFrequency(double)
{
}

void CorrelationWidgetController::appendView(BaseCorrelationWidget *view)
{
	m_view = view;
}

Q_DECLARE_METATYPE(float*)
void CorrelationWidgetController::onDataArrived(const QString &method, const QVariant &arg)
{
	emit signalonDataArrivedLS(method, arg);
}

void CorrelationWidgetController::onDataArrivedLS(const QString method, const QVariant arg)
{
	Q_UNUSED(method);

	QList<QVariant> list = arg.toList();
	float* spectrum = list.at(0).value<float*>();
	float* spectrumPeakHold = (float*)list.at(1).value<float*>();

	float skoQuality = list.at(list.size() - 2).toFloat();
	double doppler = list.at(list.size() - 1).toDouble();

	if (list.count() == 6){
		setData(spectrum, spectrumPeakHold, skoQuality);
	} else {
		int pointCount = list.at(2).toInt();
		double bandwidth = list.at(3).toDouble();
		bool isComplex = list.at(4).toBool();
		setDataSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex, skoQuality);
		m_dopplerList.clear();
	}

	if(m_locationController && m_locationController->getReceiveDopler() ) {
		m_view->setDopplerVisible(true);
	} else {
		m_view->setDopplerVisible(false);
		m_dopplerList.clear();
	}

	if(m_dopplerList.size() > m_doplerLim) {
		m_dopplerList.enqueue(doppler);
		m_dopplerList.dequeue();
	} else {
		m_dopplerList.enqueue(doppler);
	}


	m_view->setupDoppler(m_dopplerList);

	emit onGraphReady();

	QString base = list.at(list.size() - 4).toString();
	QString second = list.at(list.size() - 3).toString();

	setLabelName(base, second);
}

void CorrelationWidgetController::clear()
{
	m_view->reset();
}

void CorrelationWidgetController::setDataSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex, float sko)
{
	//_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;

    m_view->setDataSetup(bandwidth, m_pointCount, spectrum, spectrum_peak_hold,
                         sko);
	//_mux.unlock();
}

void CorrelationWidgetController::setData(float *spectrum, float *spectrum_peak_hold, float sko)
{
    m_view->permanentSetup(spectrum, spectrum_peak_hold, sko);
}

void CorrelationWidgetController::setLabelName(QString base, QString second)
{
    QString name = base + " - " + second;
    //if(m_labelName != name)
    //{
		m_labelName = name;
        m_view->setLabel(m_labelName);
    //}
}
