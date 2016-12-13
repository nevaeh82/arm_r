#include "ISolver.h"
#include "LocationSetupWidgetController.h"

#include "RDSExchange.h"
#include "Logger/Logger.h"


LocationSetupWidgetController::LocationSetupWidgetController(QObject* parent):
	QObject(parent),
	m_plotCounter(0),
	m_incomePlotCounter(0),
	m_isStartLocation(false)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant))/*, Qt::DirectConnection*/);
	connect(this, SIGNAL(signalPlotComplete()), this, SLOT(slotPlotDrawCompleteInternal()));

	//To do read settings
	m_locationMessage.set_duration(10);
	m_locationMessage.set_central_frequency(20);
	RdsProtobuf::Range* range = m_locationMessage.mutable_range();
	range->set_start(10);
	range->set_end(30);
	m_locationMessage.set_convolution(true);
	m_locationMessage.set_doppler(false);
	m_locationMessage.set_convolution_plot(true);
	m_locationMessage.set_averaging_frequency_band(50);
	m_locationMessage.set_hump(true);

	m_locationTimer.setInterval(5000);

	connect(&m_locationTimer, SIGNAL(timeout()), this, SLOT(requestLocation()));
}

LocationSetupWidgetController::~LocationSetupWidgetController()
{
	if(m_view != NULL)
	{
		m_view->close();
		delete m_view;
	}
}

void LocationSetupWidgetController::setLocationSetup(const RdsProtobuf::ClientMessage_OneShot_Location &data)
{
	m_view->setLocationData( data );
}

void LocationSetupWidgetController::setPlatformList(const QStringList &platformList)
{
	m_view->setPlatformList( platformList );
}

void LocationSetupWidgetController::setDeviceEnableState(int dev, bool state)
{
	m_view->setDeviceEnableState(dev, state);
}

void LocationSetupWidgetController::setSpectrumSelection(double start, double end)
{
	RdsProtobuf::Range* range = m_locationMessage.mutable_range();
	range->set_start(start);
	range->set_end(end);

	m_view->setLocationData(m_locationMessage);

	emit signalSelectionUpdate();
}

void LocationSetupWidgetController::setDevicesState(RdsProtobuf::System_SystemOptions opt)
{
	m_view->setDeviceCommonState(opt);
}

RdsProtobuf::ClientMessage_OneShot_Location LocationSetupWidgetController::getCurrentLocation() const
{
	return m_locationMessage;
}

void LocationSetupWidgetController::slotSetReceiveSpectrums(bool receive)
{
	setLocationState(receive);
}

void LocationSetupWidgetController::setLocationState(bool state)
{
	m_isStartLocation = state;

	if( state ) {
		m_plotCounter = m_incomePlotCounter;
		requestLocation();
		m_locationTimer.start();
	} else {
		m_plotCounter = m_incomePlotCounter;
		m_locationTimer.stop();
	}
}

bool LocationSetupWidgetController::getReceiveSpectrums() const
{
	return m_isStartLocation;
}

void LocationSetupWidgetController::appendView(LocationSetupWidget *view)
{
	m_view = view;

	//connect(m_view, SIGNAL(onSendSignal()), this, SLOT(slotSendSettings()));
	//connect(m_view, SIGNAL(onSignalUpdate()), this, SLOT(slotOnUpdate()));
//	connect(m_view, SIGNAL(onSignalSet()), this, SLOT(slotOnSet()));

//	connect(m_view, SIGNAL(onSignalUpdateDet()), this, SLOT(slotOnUpdateDet()));
//	connect(m_view, SIGNAL(onSignalSetDet()), this, SLOT(slotOnSetDet()));

//	connect(m_view, SIGNAL(onSignalUpdateCor()), this, SLOT(slotOnUpdateCor()));
//	connect(m_view, SIGNAL(onSignalSetCor()), this, SLOT(slotOnSetCor()));

//	connect(m_view, SIGNAL(onSignalUpdateAnalysis()), this, SLOT(slotOnUpdateAnalysis()));
//	connect(m_view, SIGNAL(onSignalSetAnalysis()), this, SLOT(slotOnSetAnalysis()));

	connect(m_view, SIGNAL(analysisChannelChanged(int)), this, SIGNAL(analysisChannelChanged(int)));
	connect(m_view, SIGNAL(onSignalDeviceEnable(int, bool)), this, SLOT(slotOnDeviceEnable(int, bool)));

	connect(m_view, SIGNAL(sendRdsData(QByteArray)), this, SIGNAL(sendRdsData(QByteArray)));
}

void LocationSetupWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

LocationSetupWidget *LocationSetupWidgetController::getView()
{
	return m_view;
}

void LocationSetupWidgetController::requestLocation()
{
	RdsProtobuf::Packet pkt;
	createGetLocationStatus( pkt, m_locationMessage );
	m_plotCounter = m_incomePlotCounter;

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::requestAnalysis(int channel)
{
	//Request Analysis Data
	RdsProtobuf::Packet pkt1;
	RdsProtobuf::ClientMessage_OneShot_Analysis aMsg;
	aMsg.set_duration( m_view->getAnalysisDuration() );
	aMsg.set_central_frequency( m_locationMessage.central_frequency() );
	aMsg.set_detector_index( channel );
	aMsg.mutable_range()->set_start( m_locationMessage.range().start() );
	aMsg.mutable_range()->set_end( m_locationMessage.range().end() );

	createGetAnalysisStatus(pkt1, aMsg);

	emit sendRdsData( pack(pkt1) );

	//Request any plot
	RdsProtobuf::Packet pkt;
	createGetLocationStatus( pkt, m_locationMessage );
	m_plotCounter = m_incomePlotCounter;

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::requestLocationTimer()
{
	requestLocation();
}

void LocationSetupWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void LocationSetupWidgetController::onMethodCalledSlot(QString method, QVariant data)
{
	//Receiving rds raw protobuf
	if( method == RPC_METHOD_CONFIG_RDS_ANSWER) {

		RdsProtobuf::ServerMessage sMsg;
		if( !parseServerMessage(data.toByteArray(), sMsg) ) {
			return;
		}

		if( isServerLocationShot(sMsg) ) {
			RdsProtobuf::ServerMessage_OneShotData_LocationData data = getServerLocationShot( sMsg );

			bool one = data.success();
			QString error = QString::fromStdString(data.error());

			uint detErrorCounter = 0;
			for( int i = 0; i<data.detector_status_size(); i++ ) {
				if( !data.detector_status(i) ) {
					detErrorCounter++;
				}
			}

			int t1 = data.spectrum_plot_size();
			int t2 = data.convolution_plot_size();
			int t3 = detErrorCounter;

			uint plotCounter = (data.detector_status_size() + data.convolution_plot_size()) - detErrorCounter;
			if(m_incomePlotCounter != plotCounter && one) {
				m_incomePlotCounter = plotCounter;
				m_plotCounter = plotCounter;
			}

			if(m_isStartLocation) {
				m_locationTimer.start();
			}
		}

	}
}

void LocationSetupWidgetController::slotOnSetCommonFreq(int freq)
{
	int df = freq - m_locationMessage.central_frequency();

	m_locationMessage.set_central_frequency( freq );
	m_locationMessage.mutable_range()->set_start( m_locationMessage.range().start() + df );
	m_locationMessage.mutable_range()->set_end( m_locationMessage.range().end() + df );

	m_view->setLocationData(m_locationMessage);
}

void LocationSetupWidgetController::slotOnDeviceEnable(int id, bool enable) {
	RdsProtobuf::Packet pkt;

	createSetEnablePlatform( pkt, id, enable );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotPlotDrawComplete()
{
	emit signalPlotComplete();
}

void LocationSetupWidgetController::slotPlotDrawCompleteInternal()
{
	m_plotCounter--;

	log_debug(QString("Plot ready : %1").arg(m_plotCounter));

	if(m_isStartLocation) {
		m_locationTimer.start();
	}

	if(m_plotCounter <= 0) {
		m_plotCounter = m_incomePlotCounter;

		if(m_isStartLocation) {
			requestLocation();
			m_locationTimer.start();
		}
	}
}
