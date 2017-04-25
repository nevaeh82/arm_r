#include "ISolver.h"
#include "SolverResultWidgetController.h"

#define ELAPSED_MAIL_TIMEOUT 10000

SolverResultWidgetController::SolverResultWidgetController(QObject* parent):
	QObject(parent)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));


	m_smtpThread = new SmtpClientThread(0);
	m_smtpQThread = new QThread();
	m_emailSettings = new EmailSettings(0);

	slotEmailUpdate();

	connect(m_smtpQThread, SIGNAL(finished()), m_smtpQThread, SLOT(deleteLater()));
	connect(m_smtpQThread, SIGNAL(started()), m_smtpThread, SLOT(onStart()));
	connect(m_emailSettings, SIGNAL(signalMailSettingsUpdate()), this, SLOT(slotEmailUpdate()));

	m_smtpThread->moveToThread(m_smtpQThread);
	m_smtpQThread->start();

	m_elapsedMail.start();
}

SolverResultWidgetController::~SolverResultWidgetController()
{
	m_smtpQThread->quit();
	delete m_smtpThread;
	m_smtpQThread->deleteLater();

	if(m_view != NULL)
	{
		delete m_view;
	}
}

void SolverResultWidgetController::appendView(SolverResultWidget *view)
{
	m_view = view;
}

void SolverResultWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

SolverResultWidget*SolverResultWidgetController::getView()
{
	return m_view;
}

void SolverResultWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void SolverResultWidgetController::addResultToLog(const QByteArray& inData)
{
	QByteArray data = inData;

	QDataStream ds(&data, QIODevice::ReadOnly);
	int sourceType;
	int inResult;

	ds >> sourceType;
	ds >> inResult;

	SolveResult result = (SolveResult)inResult;

	QString source;

	switch(sourceType)
	{
		case AUTO_HEIGH:
			source = tr("AUTO");
			break;
		case MANUAL_HEIGH:
			source = tr("MANUAL");
			break;
		case ONE_DATA:
			source = tr("ALONE");
			break;
		case HYPERBOLES:
			source = tr("HYPERBOLES");
			break;
		default:
			break;
	}

	QString log = source + " - " + getSolverResultToString(result);

	m_view->appendSolverResult(log);

}

void SolverResultWidgetController::addResultToLog1(const QByteArray& inData)
{
	QByteArray data = inData;

	QDataStream ds(&data, QIODevice::ReadOnly);
	int sourceType;
	QByteArray solverData;

	ds >> sourceType;
	ds >> solverData;

	SolverProtocol::EstimateQuality quality = SolverProtocol::EstimateQuality(sourceType);

	QString source;

	switch(quality)
	{
		case SolverProtocol::GOOD_QUALITY:
			source = tr(" Good quality ");
			break;
		case SolverProtocol::BAD_QUALITY:
			source = tr(" Bad quality ");
			break;
		default:
			break;
	}

	QString log = source;

	m_view->appendSolverResult(QDateTime::currentDateTime().toString() + tr(" Found solve ") + log);

	parseForMail(solverData);

	emit solverResult(solverData);
}

void SolverResultWidgetController::parseForMail(const QByteArray &data)
{
	int quality = 0;
	bool isMoving = false;
	float movingFreq = 0;

	if(!data.isEmpty()) {
		SolverProtocol::Packet pkt;
		pkt.ParseFromArray(data.data(), data.size());

		bool isMail = false;

		SolverProtocol::Packet_DataFromSolver_SolverSolution solPkt;
		if(pkt.datafromsolver().has_solution_manual_altitude()) {
			solPkt = pkt.datafromsolver().solution_manual_altitude();

			for( int i = 0; i < solPkt.trajectory_size(); i++ ) {
				solverResultStruct res;

				res.dateTime = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).datetime();
				res.freq = solPkt.trajectory(i).central_frequency();
				res.state = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).state();
				if(res.state == 1) {
					isMoving = true;
					movingFreq = res.freq;
				}

				res.quality = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).quality();
				quality |= res.quality;

				res.lat = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lat();
				res.lon = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lon();

				isMail = sendMail(res);
			}
		}

		if(pkt.datafromsolver().has_solution_automatic_altitude()) {
			solPkt = pkt.datafromsolver().solution_automatic_altitude();

			for( int i = 0; i < solPkt.trajectory_size(); i++ ) {
				solverResultStruct res;

				res.dateTime = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).datetime();
				res.freq = solPkt.trajectory(i).central_frequency();
				res.state = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).state();
				if(res.state == 1) {
					isMoving = true;
					movingFreq = res.freq;
				}

				res.quality = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).quality();
				quality |= res.quality;

				res.lat = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lat();
				res.lon = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lon();

				isMail = sendMail(res);
			}
		}

		if(isMail) {
			m_elapsedMail.restart();
		}
	}
}

bool SolverResultWidgetController::sendMail(const solverResultStruct &res)
{
	if(m_elapsedMail.elapsed() < ELAPSED_MAIL_TIMEOUT) {
		return false;
	}

	QString state = "";
	QString quality = "";

	if(res.state == 1) {
		if(!m_emailSettings->isMoving()) {
			return true;
		}
		state = tr("Moving");
	} else if(res.state == 2) {
		if(!m_emailSettings->isStanding()) {
			return true;
		}

		state = tr("Standing");
	} else if(res.state == 3) {
		if(!m_emailSettings->isUnknown()) {
			return true;
		}

		state = tr("UNKNOWN");
	}

	if(res.quality == 1) {
		quality = tr("Good");
	} else if(res.quality == 2) {
		quality = tr("Bad");
	} else if(res.quality == 3) {
		quality = tr("---");
	}


	if(m_emailSettings->isSend()) {

		m_smtpThread->sendMessage(tr("Found AIM: \n   freq %1 \n   state %2 \n   quality %3 \n   Lon: %4 \n   Lat: %5 \n   Time: %6").arg(res.freq).arg(state).arg(quality).arg(res.lon).arg(res.lat).arg(QDateTime::currentDateTime().toString()) + "\n" );

	}

	return true;
}

void SolverResultWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
	if( method == RPC_SLOT_SERVER_SEND_BPLA_RESULT ) {
		addResultToLog(argument.toByteArray());
		return;
	} else if( method == RPC_SLOT_SERVER_SEND_BPLA_RESULT_1 ) {
		addResultToLog1(argument.toByteArray());
		return;
	}
//	else if( method == RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1 ) {
//		addResultToLog1(argument.toByteArray());
//		return;
	//	}
}

void SolverResultWidgetController::onEmailSettings()
{
	m_emailSettings->show();
}

QString SolverResultWidgetController::getSolverResultToString(const SolveResult &result)
{
	QString resultString;
	switch(result)
	{
		case SOLVED:
			resultString = tr("Successed solved");
			break;
		case ERROR_OCCURED:
			resultString = tr("Some errors during solve");
			break;
		case NOT_ENOUGH_DATA:
			resultString = tr("Not enough data for solve");
			break;
		case THERE_IS_NO_SOLUTION:
			resultString = tr("There is no solution");
			break;
		case CANT_DETERMINE_REAL_TRAJECTORY:
			resultString = tr("No solve cause 2 trajectory");
			break;
		case TOO_FEW_RANGES:
			resultString = tr("Not enough distances for solve");
			break;
		case TOO_LOW_SOLUTION_ACCURACY:
			resultString = tr("There is not enough accuracy for solve");
			break;
		case TOO_LOW_DATA_ACCURACY:
			resultString = tr("There is not enough input accuracy for solve");
			break;
		case COORDS_DOES_NOT_HIT_IN_AREA:
			resultString = tr("Solves data is out of range of resposible zone");
			break;
		case COORDS_DOES_NOT_HIT_IN_STROB:
			resultString = tr("Coords is out of catching strobe");
			break;
		default:
			break;
	}
	return resultString;

}

void SolverResultWidgetController::slotEmailUpdate()
{
	m_smtpThread->setLocalMailList(m_emailSettings->localMailList());
	m_smtpThread->setRemoteMailList(m_emailSettings->remoteMailList());
	m_smtpThread->setLocalMailSettings(m_emailSettings->localMailSettings());
	m_smtpThread->setRemoteMailSettings(m_emailSettings->remoteMailSettings());
}
