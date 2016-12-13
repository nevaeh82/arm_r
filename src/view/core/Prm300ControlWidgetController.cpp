#include "Prm300ControlWidgetController.h"

#include "RDSExchange.h"

Prm300ControlWidgetController::Prm300ControlWidgetController(const QString& name, QObject *parent)
	: QObject( parent )
	, m_dbManager( 0 )
	, m_view( NULL )
	, m_rpcFlakonClient( NULL )
	, m_stationName( name )
	, m_stationId( 0 )
	, m_platformId( 0 )
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

void Prm300ControlWidgetController::setRpcFlakonClient(RpcFlakonClientWrapper *client)
{
	if(!client) {
		return;
	}
	m_rpcFlakonClient = client;
}

void Prm300ControlWidgetController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void Prm300ControlWidgetController::setName(const QString &name)
{
	m_stationName = name;
}

void Prm300ControlWidgetController::setId(const uint id)
{
	m_stationId = id;
}

void Prm300ControlWidgetController::setChannel(const uint id)
{
	m_channelId = id;
}

void Prm300ControlWidgetController::setPlatform(const uint id)
{
	m_platformId = id;
}

void Prm300ControlWidgetController::setChannelState(bool state)
{
	m_view->setState( state );
}

void Prm300ControlWidgetController::init()
{
	//connect(m_view, SIGNAL(signalShowLists()), this, SLOT(slotShowLists()));
	//	connect(m_view, SIGNAL(signalSetAtt1Value(int)), this, SLOT(slotSetAtt1(int)));
	//	connect(m_view, SIGNAL(signalSetAtt2Value(int)), this, SLOT(slotSetAtt2(int)));
	//	connect(m_view, SIGNAL(signalSetFilter(int)), this, SLOT(slotSetFilter(int)));
	connect(m_view, SIGNAL(signalOnSetParams()), this, SLOT(slotSetAtt1()));
	connect(m_view, SIGNAL(signalOnEnableReceiver(bool)), this, SLOT(slotEnableReceiver(bool)));
	m_view->hide();
}

void Prm300ControlWidgetController::slotSetAtt1()
{

	QByteArray data;
	QDataStream stream(&data, QIODevice::ReadWrite);
	stream << m_stationName
		   << m_view->getPrmParams();

	CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_ATT1, QVariant::fromValue(data));
	//m_rpcPrmClient->setCommand(msg);
}

void Prm300ControlWidgetController::slotSetAtt2(int value)
{
	CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_ATT2, QVariant::fromValue(value));
	//m_rpcPrmClient->setCommand(msg);
}

void Prm300ControlWidgetController::slotSetFilter(int index)
{
	CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_FILTER, QVariant::fromValue(index));
	//m_rpcPrmClient->setCommand(msg);
}

void Prm300ControlWidgetController::slotEnableReceiver(bool val)
{
	if(!m_rpcFlakonClient) {
		return;
	}

	RdsProtobuf::Packet pkt;

	createSetEnableReceiver( pkt, m_platformId, m_stationId, val );
	//createGetLocationStatus(pkt);

	m_rpcFlakonClient->sendRdsProto( pack(pkt) );
}
