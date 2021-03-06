#include "CorrelationWidgetController.h"
#include "CorrelationWidget.h"

CorrelationWidgetController::CorrelationWidgetController(QObject *parent) 
	: QObject(parent)
	, m_view(0)
	, m_bandwidth(0)
	, m_pointCount(0)
	, m_isComplex(0)
	, m_graphicsWidget(0)
{
	connect(this, SIGNAL(signalonDataArrivedLS(QString,QVariant)), this, SLOT(onDataArrivedLS(QString,QVariant)));
	connect(this, SIGNAL(signalOnVisible(bool)), this, SLOT(onVisible(bool)));
}

QWidget *CorrelationWidgetController::getWidget() const
{
	return m_view;
}

void CorrelationWidgetController::setVisible(const bool isVisible)
{
	emit signalOnVisible(isVisible);
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

void CorrelationWidgetController::appendView(CorrelationWidget *view)
{
	m_view = view;
	m_graphicsWidget = m_view->getGraphicsWidget();
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

	if (list.count() == 4){
		setData(spectrum, spectrumPeakHold);
	} else {
		int pointCount = list.at(2).toInt();
		double bandwidth = list.at(3).toDouble();
		bool isComplex = list.at(4).toBool();
		setDataSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
	}

	QString base = list.at(list.size() - 2).toString();
	QString second = list.at(list.size() - 1).toString();

	setLabelName(base, second);
	m_graphicsWidget->ZoomOutFull();
}


void CorrelationWidgetController::clear()
{
	m_graphicsWidget->Reset();
}

void CorrelationWidgetController::setDataSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	//_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;

	float maxv = 1.0;
	float minv = 0.0;

	m_graphicsWidget->SetAutoscaleY(false);
	//  spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
	m_graphicsWidget->SetAlign(3);
	m_graphicsWidget->SetHorizontalLabel(tr("m"));
	m_graphicsWidget->Setup(true,m_bandwidth,tr("Level"), spectrum, m_pointCount, spectrum_peak_hold, m_pointCount,false, false, minv, maxv);
	m_graphicsWidget->SetSpectrumVisible(1, true);
	//_mux.unlock();
}

void CorrelationWidgetController::setData(float *spectrum, float *spectrum_peak_hold)
{
	m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, 0.0, 1.0);
}

void CorrelationWidgetController::setLabelName(QString base, QString second)
{
	QString name = base + " - " + second;
	if(m_labelName != name)
	{
		m_labelName = name;
		m_graphicsWidget->ClearAllLabels();
		m_graphicsWidget->SetLabel(0, m_labelName);
	}
}
