#include "Prm300ControlWidgetController.h"

Prm300ControlWidgetController::Prm300ControlWidgetController(const QString& name, QObject *parent)
	: QObject( parent )
	, m_dbManager( 0 )
	, m_view( NULL )
	, m_rpcPrmClient( NULL )
	, m_stationName( name )
{
}

Prm300ControlWidgetController::~Prm300ControlWidgetController()
{
}

void Prm300ControlWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	if(method == RPC_SLOT_SERVER_PRM_STATUS)
	{
        QList<QVariant> list = argument.toList();
        quint16 freq = list.at(0).toUInt();
        quint8 filter = list.at(1).toUInt();
        quint8 att1 = list.at(2).toUInt();
        quint8 att2 = list.at(3).toUInt();
        if(m_view)
        {
            m_view->setData(freq, filter, att1, att2);
        }
        m_dbManager->updatePropertyValue(m_stationName, DB_FREQUENCY_PROPERTY, freq);
	}
}

void Prm300ControlWidgetController::appendView(Prm300ControlWidget *view)
{
	m_view = view;
	init();
}

void Prm300ControlWidgetController::setRpcPrmClient(RpcPrmClient *client)
{
	m_rpcPrmClient = client;
}

void Prm300ControlWidgetController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void Prm300ControlWidgetController::init()
{
	//connect(m_view, SIGNAL(signalShowLists()), this, SLOT(slotShowLists()));
	connect(m_view, SIGNAL(signalSetAtt1Value(int)), this, SLOT(slotSetAtt1(int)));
	connect(m_view, SIGNAL(signalSetAtt2Value(int)), this, SLOT(slotSetAtt2(int)));
	connect(m_view, SIGNAL(signalSetFilter(int)), this, SLOT(slotSetFilter(int)));
	m_view->hide();
}

void Prm300ControlWidgetController::slotSetAtt1(int value)
{
	if(!m_rpcPrmClient)
		return;

	CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_ATT1, QVariant::fromValue(value));
	m_rpcPrmClient->setCommand(msg);
}

void Prm300ControlWidgetController::slotSetAtt2(int value)
{
	if(!m_rpcPrmClient)
		return;

	CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_ATT2, QVariant::fromValue(value));
	m_rpcPrmClient->setCommand(msg);
}

void Prm300ControlWidgetController::slotSetFilter(int index)
{
	CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_FILTER, QVariant::fromValue(index));
	m_rpcPrmClient->setCommand(msg);
}
