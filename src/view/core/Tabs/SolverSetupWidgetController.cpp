#include "ISolver.h"
#include "SolverSetupWidgetController.h"


SolverSetupWidgetController::SolverSetupWidgetController(QObject* parent):
	QObject(parent)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));
}

SolverSetupWidgetController::~SolverSetupWidgetController()
{
	if(m_view != NULL)
	{
		delete m_view;
	}
}

void SolverSetupWidgetController::appendView(SolverSetupWidget *view)
{
	m_view = view;

    if(m_view != NULL)
    {
        connect(m_view, SIGNAL(onSendSignal()), this, SLOT(slotSendSettings()));
        connect(m_view, SIGNAL(onGetVersion()), this, SLOT( slotGetVersion()) );
    }
}

void SolverSetupWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

SolverSetupWidget *SolverSetupWidgetController::getView()
{
	return m_view;
}

void SolverSetupWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void SolverSetupWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
	if( method == RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1 ) {
		QByteArray data = argument.toByteArray();
		SolverProtocol::Packet pkt;
		pkt.ParseFromArray( data.data(), data.size() );

		if( !pkt.has_datafromsolver() ) {
			return;
		}

		if( pkt.datafromsolver().has_solverresponse() ) {
			SolverProtocol::Packet_DataFromSolver_SolverResponse response = pkt.datafromsolver().solverresponse();
			if( response.has_solverversion() ) {
				m_view->setSolverVersion( QString::fromStdString(response.solverversion()) );
			}
		}
		return;
	}
}

void SolverSetupWidgetController::slotSendSettings()
{
	emit onSendSolverCommandSettings(QByteArray());
}

void SolverSetupWidgetController::slotGetVersion()
{
	SolverProtocol::Packet pkt;
	pkt.mutable_datafromclient()->add_getrequest( SolverProtocol::Packet_DataFromClient_GetRequest_getSolverVersion );

	QByteArray data;
	data.resize( pkt.ByteSize() );
	pkt.SerializeToArray(data.data(), data.size());

	addPreambula( data );

	emit onSendSolverCommandSettings( data );
}

void SolverSetupWidgetController::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend("SolverPacket");
}
