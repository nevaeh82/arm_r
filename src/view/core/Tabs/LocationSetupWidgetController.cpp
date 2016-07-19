#include "ISolver.h"
#include "LocationSetupWidgetController.h"

#include "RDSExchange.h"


LocationSetupWidgetController::LocationSetupWidgetController(QObject* parent):
	QObject(parent)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));
}

LocationSetupWidgetController::~LocationSetupWidgetController()
{
	if(m_view != NULL)
	{
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
