#include "ISolver.h"
#include "LocationSetupWidgetController.h"

#include "RDSExchange.h"
#include "Logger/Logger.h"

#define LOCATION_TIMER_INTERVAL 5000

#define MINIMUM_FREQ 20
#define MAXIMUM_FREQ 7000


LocationSetupWidgetController::LocationSetupWidgetController(QObject* parent):
	QObject(parent),
	m_plotCounter(0),
	m_incomePlotCounter(0),
	m_isStartLocation(false),
	m_sleepMode(false)
{
	m_view = NULL;
	m_requestReady = true;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant))/*, Qt::DirectConnection*/);
	connect(this, SIGNAL(signalPlotComplete()), this, SLOT(slotPlotDrawCompleteInternal()));

	//To do read settings
	m_locationMessage.set_duration(10);
	m_locationMessage.set_central_frequency(20);
	RdsProtobuf::Range* range = m_locationMessage.mutable_range();
	range->set_start(10);
	range->set_end(30);
	m_locationMessage.set_convolution(true);
	m_locationMessage.set_doppler(true);
	m_locationMessage.set_convolution_plot(true);
	m_locationMessage.set_averaging_frequency_band(50);
	m_locationMessage.set_hump(false);

	m_locationTimerInterval = LOCATION_TIMER_INTERVAL;
	m_locationTimer.setInterval(m_locationTimerInterval);

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

void LocationSetupWidgetController::setPlatformList(const QStringList &platformList)
{
	m_view->setPlatformList( platformList );
}

void LocationSetupWidgetController::setDeviceEnableState(int dev, bool state)
{
	m_view->setDeviceEnableState(dev, state);
}

void LocationSetupWidgetController::setChannelEnableState(int dev, int channel, bool state)
{
	m_view->setChannelEnableState(dev, channel, state);
}

void LocationSetupWidgetController::setSpectrumSelection(double start, double end)
{
	double center = m_locationMessage.central_frequency();

	if( fabs(center-start) < 10 || fabs(center-end) < 10 ) {
		RdsProtobuf::Range* range = m_locationMessage.mutable_range();
		range->set_start(start);
		range->set_end(end);
		m_view->setLocationData(m_locationMessage);
	}

	emit signalSelectionUpdate();
	emit onStateChanged();
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

    CommonParams::Parameters params;
    params.set_spectrums(state);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

bool LocationSetupWidgetController::getReceiveSpectrums() const
{
	return m_isStartLocation;
}

bool LocationSetupWidgetController::getReceiveDopler() const
{
	return m_locationMessage.doppler();
}

void LocationSetupWidgetController::setSleepMode(bool val)
{
	m_sleepMode = val;
    CommonParams::Parameters params;
    params.set_sleep(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::appendView(LocationSetupWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(analysisChannelChanged(int)), this, SIGNAL(analysisChannelChanged(int)));
	connect(m_view, SIGNAL(onSignalDeviceEnable(int, bool)), this, SLOT(slotOnDeviceEnable(int, bool)));

	connect(m_view, SIGNAL(sendRdsData(QByteArray)), this, SIGNAL(sendRdsData(QByteArray)));
	connect(m_view, SIGNAL(signalSettingsChanged()), this, SLOT(slotWigdetSettingsChanged()));

	connect(m_view, SIGNAL(signalRestartRds()), this, SLOT(onRestartRds()));
	connect(m_view, SIGNAL(signalGetSystem()), this, SLOT(onGetSystem()));

	m_view->setLocationData(m_locationMessage);
}

void LocationSetupWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

LocationSetupWidget *LocationSetupWidgetController::getView()
{
	return m_view;
}

void LocationSetupWidgetController::slotWigdetSettingsChanged()
{
	RdsProtobuf::ClientMessage_OneShot_Location tmpMsg = m_view->getLocationData();

	if( (tmpMsg.central_frequency() != m_locationMessage.central_frequency()) ||
		(tmpMsg.range().start() != m_locationMessage.range().start()) ||
		(tmpMsg.range().end() != m_locationMessage.range().end()) ) {

		int center = tmpMsg.central_frequency();
		double start = tmpMsg.range().start();
		double end = tmpMsg.range().end();

		if( fabs(center-start) > 10 ) {
			tmpMsg.mutable_range()->set_start(center-10);
			tmpMsg.mutable_range()->set_end(end);
			start = tmpMsg.range().start();
		}

		if( fabs(center-end) > 10 ) {
			tmpMsg.mutable_range()->set_end(center+10);
			tmpMsg.mutable_range()->set_start(start);
		}

		m_view->setLocationData( tmpMsg );
	}

	m_locationMessage = tmpMsg;

	emit signalSettingsChanged();
	emit signalSelectionUpdate();

    emit onStateChanged();
}

void LocationSetupWidgetController::slotChangeMode(int val)
{
    CommonParams::Parameters params;
    params.set_mode(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotPanorama(bool val)
{
    CommonParams::Parameters params;
    params.set_panorama(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotSetStartFreq(int val)
{
    CommonParams::Parameters params;
    params.set_startfreq(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotSetEndFreq(int val)
{
    CommonParams::Parameters params;
    params.set_finishfreq(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotSystemMerge(bool val)
{
    CommonParams::Parameters params;
    params.set_mergesysctrl(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::requestLocation()
{
	RdsProtobuf::Packet pkt;
	createGetLocationStatus( pkt, m_locationMessage );
	m_plotCounter = m_incomePlotCounter;

//	logLoc.append( QString("duration %1 \n").arg( m_locationMessage.duration() ) );
//	logLoc.append( QString("Central freq %1 \n").arg( m_locationMessage.central_frequency() ) );
//	logLoc.append( QString("rangestart %1 \n").arg( m_locationMessage.range().start() ) );
//	logLoc.append( QString("rangeend %1 \n").arg( m_locationMessage.range().end() ) );
//	logLoc.append( QString("convol %1 \n").arg( m_locationMessage.convolution() ) );
//	logLoc.append( QString("doppler %1 \n").arg( m_locationMessage.doppler() ) );
//	logLoc.append( QString("convPlot %1 \n").arg( m_locationMessage.convolution_plot() ) );
//	logLoc.append( QString("hump %1 \n").arg( m_locationMessage.hump() ) );
//	logLoc.append( QString("average %1 \n").arg( m_locationMessage.averaging_frequency_band() ) );

//	log_debug("Request ??? >>>>>>>>>>>>>>>>>>>>>>>>>> ");

	m_requestReady = false;

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
//	RdsProtobuf::Packet pkt;
//	createGetLocationStatus( pkt, m_locationMessage );
//	m_plotCounter = m_incomePlotCounter;

//	emit sendRdsData( pack(pkt) );
	requestLocation();
}

void LocationSetupWidgetController::requestRecord(int channel)
{
	//Request Analysis Data
	RdsProtobuf::Packet pkt1;
	RdsProtobuf::ClientMessage_OneShot_Record aMsg;
	aMsg.set_duration( m_view->getRecordDuration() );
	aMsg.set_central_frequency( m_locationMessage.central_frequency() );
	aMsg.set_detector_index( channel );
	aMsg.mutable_range()->set_start( m_locationMessage.range().start() );
	aMsg.mutable_range()->set_end( m_locationMessage.range().end() );

	createRecordSignal(pkt1, aMsg);

	emit sendRdsData( pack(pkt1) );
}

void LocationSetupWidgetController::onRestartRds()
{
	RdsProtobuf::Packet pkt1;
	createRestartRds(pkt1);

	emit sendRdsData( pack(pkt1) );
}

void LocationSetupWidgetController::onGetSystem()
{
	RdsProtobuf::Packet pkt1;

	createGetSystemSystemOptions(pkt1);

	emit sendRdsData( pack(pkt1) );
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

			m_requestReady = true;

			uint plotCounter = (data.detector_status_size() + data.convolution_plot_size()) - detErrorCounter;
			if(m_incomePlotCounter != plotCounter && one) {
				m_incomePlotCounter = plotCounter;
				m_plotCounter = plotCounter;
			}

			if(m_isStartLocation) {
				m_locationTimer.start();
			}

			if(m_sleepMode) {
				requestLocation();
			}
		}

	} else if(method == RPC_SLOT_SERVER_SEND_SOLVER_CONNECT_STATE) {
		emit solverConnectionState(data.toBool());
	}
}

void LocationSetupWidgetController::slotOnSetCommonFreq(double freq)
{
	if(freq < MINIMUM_FREQ) {
		freq = MINIMUM_FREQ;
	}

	if(freq > MAXIMUM_FREQ) {
		freq = MAXIMUM_FREQ;
	}

	int df = freq - m_locationMessage.central_frequency();

	m_locationMessage.set_central_frequency( (int)freq );
	m_locationMessage.mutable_range()->set_start( m_locationMessage.range().start() + df );
	m_locationMessage.mutable_range()->set_end( m_locationMessage.range().end() + df );

	m_view->setLocationData(m_locationMessage);
	emit onStateChanged();
}

void LocationSetupWidgetController::slotOnSetCommonFreq(double freq, double bandwidth)
{
	if(freq < MINIMUM_FREQ) {
		freq = MINIMUM_FREQ;
	}

	if(freq > MAXIMUM_FREQ) {
		freq = MAXIMUM_FREQ;
	}

	if(bandwidth > 20) {
		bandwidth = 20;
	}

    double start = (double)freq - bandwidth/2;
    double end = (double)freq + bandwidth/2;

    m_locationMessage.set_central_frequency( (int)(freq+0.5) );
	m_locationMessage.mutable_range()->set_start( (double)freq - bandwidth/2 );
	m_locationMessage.mutable_range()->set_end( (double)freq + bandwidth/2 );

	m_view->setLocationData(m_locationMessage);

	emit onStateChanged();

    CommonParams::Parameters params;
    params.set_frequency(freq);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotOnDeviceEnable(int id, bool enable) {
	RdsProtobuf::Packet pkt;

	createSetEnablePlatform( pkt, id, enable );

	emit sendRdsData( pack(pkt) );
}

void LocationSetupWidgetController::slotSetConvolution(bool val)
{
	m_locationMessage.set_convolution(val);

	m_view->setLocationData(m_locationMessage);
	emit onStateChanged();

    CommonParams::Parameters params;
    params.set_convolution(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotSetDoppler(bool val)
{
	m_locationMessage.set_doppler(val);
	m_view->setLocationData(m_locationMessage);
	emit onStateChanged();

    CommonParams::Parameters params;
    params.set_doppler(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::slotSetHumps(bool val)
{
	m_locationMessage.set_hump(val);
	m_view->setLocationData(m_locationMessage);
	emit onStateChanged();

    CommonParams::Parameters params;
    params.set_humps(val);
    QByteArray outData;
    outData.resize( params.ByteSize() );
    params.SerializeToArray( outData.data(), outData.size() );
    emit sendCPPacketData(outData);
}

void LocationSetupWidgetController::updateLocation(RdsProtobuf::ClientMessage_OneShot_Location msg)
{
	m_locationMessage = msg;
	m_view->setLocationData(m_locationMessage);
	emit signalSettingsChanged();
}

/// Plot ready management ///

void LocationSetupWidgetController::slotPlotDrawComplete()
{
	emit signalPlotComplete();
}

void LocationSetupWidgetController::slotPlotDrawCompleteInternal()
{
	m_plotCounter--;

	//log_debug(QString("Plot ready : %1").arg(m_plotCounter));

	if(m_isStartLocation) {
		m_locationTimer.start();
	}

	if(m_plotCounter <= 0) {
		if(!m_requestReady) {
			return;
		}

		m_plotCounter = m_incomePlotCounter;

		if(m_isStartLocation) {
			requestLocation();
			m_locationTimer.start();
		}
	}
}
