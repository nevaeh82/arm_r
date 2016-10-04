#include "ISolver.h"
#include "LocationSetupWidgetController.h"

#include "RDSExchange.h"


LocationSetupWidgetController::LocationSetupWidgetController(QObject* parent):
	QObject(parent),
	m_workMode(0)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));
}

LocationSetupWidgetController::~LocationSetupWidgetController()
{
	if(m_view != NULL)
	{
		m_view->close();
		delete m_view;
	}
}

void LocationSetupWidgetController::setLocationSetup(const RdsProtobuf::Location &data)
{
	m_view->setLocationData( data );
}

void LocationSetupWidgetController::setDetectorSetup(const RdsProtobuf::Detector &data)
{
	m_view->setDetectorData( data );
}

void LocationSetupWidgetController::setCorrectionSetup(const RdsProtobuf::Correction &data)
{
	m_view->setCorrectionData( data );
}

void LocationSetupWidgetController::setAnalysisSetup(const RdsProtobuf::Analysis &data)
{
	m_view->setAnalysisData( data );
	emit analysisChannelChanged( m_view->getAnalysisChannel() );
}

int LocationSetupWidgetController::getAnalysisWorkChannel() const
{
	return m_view->getAnalysisChannel();
}

void LocationSetupWidgetController::setAnalysisChannelCount(int count)
{
	m_view->setAnalysisChannelCount( count );
}

void LocationSetupWidgetController::setPlatformList(const QStringList &platformList)
{
	m_view->setPlatformList( platformList );
}

void LocationSetupWidgetController::setDeviceEnableState(int dev, bool state)
{
	m_view->setDeviceEnableState(dev, state);
}

void LocationSetupWidgetController::setSpectrumSelection(float bandwidth, float shift, double start, double end)
{
	if(m_workMode == 1) { // Location
		m_view->onSpectrumLocationSelection(bandwidth, shift);
		slotOnSet();
	} else if(m_workMode == 2) { //Analysis
		m_view->onSpectrumAnalysisSelection(start, end);
		slotOnSetAnalysis();
	}
}

void LocationSetupWidgetController::appendView(LocationSetupWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(onSendSignal()), this, SLOT(slotSendSettings()));

	connect(m_view, SIGNAL(onSignalUpdate()), this, SLOT(slotOnUpdate()));
	connect(m_view, SIGNAL(onSignalSet()), this, SLOT(slotOnSet()));

	connect(m_view, SIGNAL(onSignalUpdateDet()), this, SLOT(slotOnUpdateDet()));
	connect(m_view, SIGNAL(onSignalSetDet()), this, SLOT(slotOnSetDet()));

	connect(m_view, SIGNAL(onSignalUpdateCor()), this, SLOT(slotOnUpdateCor()));
	connect(m_view, SIGNAL(onSignalSetCor()), this, SLOT(slotOnSetCor()));

	connect(m_view, SIGNAL(onSignalUpdateAnalysis()), this, SLOT(slotOnUpdateAnalysis()));
	connect(m_view, SIGNAL(onSignalSetAnalysis()), this, SLOT(slotOnSetAnalysis()));

	connect(m_view, SIGNAL(analysisChannelChanged(int)), this, SIGNAL(analysisChannelChanged(int)));
	connect(m_view, SIGNAL(onSignalDeviceEnable(int, bool)), this, SLOT(slotOnDeviceEnable(int, bool)));
}

void LocationSetupWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

LocationSetupWidget *LocationSetupWidgetController::getView()
{
	return m_view;
}

void LocationSetupWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void LocationSetupWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
	if( method == RPC_SLOT_SERVER_SEND_RDS_DATA ) {
	}
}

void LocationSetupWidgetController::slotSendSettings()
{
	emit sendRdsData( QByteArray() );
}

void LocationSetupWidgetController::slotOnUpdate()
{
	RdsProtobuf::Packet pkt;
	createGetLocationStatus(pkt);


	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnSet()
{
	RdsProtobuf::Packet pkt;

	createSetLocationStatus( pkt, m_view->getLocationData() );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnSetCommonFreq(int freq)
{
	RdsProtobuf::Packet pkt;
	RdsProtobuf::Location loc = m_view->getLocationData();
	loc.mutable_options()->set_central_frequency(freq);

	m_view->setLocationData( loc );

	createSetLocationStatus( pkt, loc );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnUpdateDet()
{
	RdsProtobuf::Packet pkt;
	createGetDetectorOptions( pkt, m_view->getDetectorData() );


	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnSetDet()
{
	RdsProtobuf::Packet pkt;

	createSetDetectorOptions( pkt, m_view->getDetectorData() );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnUpdateCor()
{
	RdsProtobuf::Packet pkt;
	createGetCorrectionOptions( pkt, m_view->getCorrectionData() );


	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnSetCor()
{
	RdsProtobuf::Packet pkt;

	createSetCorrectionOptions( pkt, m_view->getCorrectionData() );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnUpdateAnalysis()
{
	RdsProtobuf::Packet pkt;
	createGetAnalysisOptions( pkt, m_view->getAnalysisData() );


	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnSetAnalysis()
{
	RdsProtobuf::Packet pkt;
	createSetAnalysisOptions( pkt, m_view->getAnalysisData() );

	emit sendRdsData( pack(pkt) );

	emit analysisChannelChanged( m_view->getAnalysisChannel() );
}

void LocationSetupWidgetController::slotOnDeviceEnable(int id, bool enable) {
	RdsProtobuf::Packet pkt;

	createSetEnablePlatform( pkt, id, enable );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotOnChangeWorkMode(int mode, bool)
{
	m_workMode = mode;
	m_view->setWorkMode(mode);
}

//void LocationSetupWidgetController::slotGetVersion()
//{
//	SolverProtocol::Packet pkt;
//	pkt.mutable_datafromclient()->add_getrequest( SolverProtocol::Packet_DataFromClient_GetRequest_getSolverVersion );

//	QByteArray data;
//	data.resize( pkt.ByteSize() );
//	pkt.SerializeToArray(data.data(), data.size());

//	addPreambula( data );

//	emit onSendSolverCommandSettings( data );
//}
