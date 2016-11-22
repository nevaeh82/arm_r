#include "AnalysisWidgetController.h"
#include "AnalysisWidget.h"

#include "Logger/Logger.h"

AnalysisWidgetController::AnalysisWidgetController(int type, QObject *parent)
	: QObject(parent)
	, m_view(0)
	, m_bandwidth(0)
	, m_pointCount(0)
	, m_isComplex(0)
	, m_graphicsWidget(0)
	, m_type(type)
{
	connect(this, SIGNAL(signalonDataArrivedLS(QString,QVariant)), this, SLOT(onDataArrivedLS(QString,QVariant)));
	connect(this, SIGNAL(signalOnVisible(bool)), this, SLOT(onVisible(bool)));
}

AnalysisWidgetController::~AnalysisWidgetController()
{
	//log_debug("<<<<<<<<<<");
}

QWidget *AnalysisWidgetController::getWidget() const
{
	return m_view;
}

void AnalysisWidgetController::setVisible(const bool isVisible)
{
	emit signalOnVisible(isVisible);
}

void AnalysisWidgetController::onVisible(const bool b)
{
	m_view->setVisible(b);
}

bool AnalysisWidgetController::isGraphicVisible()
{
	return m_view->isVisible();
}

int AnalysisWidgetController::getType() const
{
	return m_type;
}

quint32 AnalysisWidgetController::getId()
{
	return 0;
}

void AnalysisWidgetController::setZeroFrequency(double)
{
}

void AnalysisWidgetController::appendView(AnalysisWidget *view)
{
	m_view = view;
	m_graphicsWidget = m_view->getGraphicsWidget();
}

Q_DECLARE_METATYPE(float*)
void AnalysisWidgetController::onDataArrived(const QString &method, const QVariant &arg)
{
	emit signalonDataArrivedLS(method, arg);
}

void AnalysisWidgetController::onDataArrivedLS(const QString method, const QVariant arg)
{
	Q_UNUSED(method);

	QList<QVariant> list = arg.toList();

	int type = list.at(0).toInt();
	if(type != m_type) {
		return;
	}

	float* spectrum = list.at(1).value<float*>();
	float* spectrumPeakHold = (float*)list.at(2).value<float*>();

	if (list.count() == 3){
		setData(spectrum, spectrumPeakHold);
	} else {
		int pointCount = list.at(3).toInt();
		double bandwidth = list.at(4).toDouble();
		bool isComplex = list.at(5).toBool();
		setDataSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
	}

	bool t = m_graphicsWidget->isVisible();

	m_graphicsWidget->ZoomOutFull();
}


void AnalysisWidgetController::clear()
{
	m_graphicsWidget->Reset();
}

void AnalysisWidgetController::setDataSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
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

void AnalysisWidgetController::setData(float *spectrum, float *spectrum_peak_hold)
{
	m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, 0, 0);
}

void AnalysisWidgetController::setLabelName(QString base, QString second)
{
    QString name = base + " - " + second;
	if(m_labelName != name)
	{
		m_labelName = name;
		m_graphicsWidget->ClearAllLabels();

		m_graphicsWidget->SetLabel(0, m_labelName);
	}
}
