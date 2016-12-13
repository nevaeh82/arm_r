#include "AnalysisWidgetController.h"
#include "AnalysisWidget.h"

#include "Logger/Logger.h"

AnalysisWidgetController::AnalysisWidgetController(int type, QObject *parent)
	: QObject(parent)
	, m_view(0)
	, m_bandwidth(0)
	, m_pointCount(0)
	, m_isComplex(0)
	, m_type(type)
	, m_id(-1)
{
	connect(this, SIGNAL(signalonDataArrivedLS(QString,QVariant)), this, SLOT(onDataArrivedLS(QString,QVariant)));
	connect(this, SIGNAL(signalOnVisible(bool)), this, SLOT(onVisible(bool)));
}

AnalysisWidgetController::~AnalysisWidgetController()
{
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

void AnalysisWidgetController::setAlarm(bool)
{

}

quint32 AnalysisWidgetController::getId()
{
	return 0;
}

int AnalysisWidgetController::getAnalysisWorkId() const
{
	return m_view->getId();
}

void AnalysisWidgetController::setZeroFrequency(double)
{
}

void AnalysisWidgetController::appendView(AnalysisWidget *view)
{
	m_view = view;
}

Q_DECLARE_METATYPE(float*)
void AnalysisWidgetController::onDataArrived(const QString &method, const QVariant &arg)
{
	emit signalonDataArrivedLS(method, arg);
}

void AnalysisWidgetController::onDataArrivedLS(const QString method, const QVariant arg)
{
	Q_UNUSED(method)
	QByteArray data = arg.toByteArray();

	RdsProtobuf::ServerMessage_OneShotData_AnalysisData adata;
	if( !adata.ParseFromArray( data.data(), data.size() ) ) {
		return;
	}

	m_id = adata.detector_index();

	m_view->setAnalysisData(adata);
	emit analysisReady(adata.detector_index(), adata.central_frequency());
}

void AnalysisWidgetController::clear()
{
}


void AnalysisWidgetController::setLabelName(QString base, QString second)
{
}
