#include "AtlantTabWidget.h"

#include "ui_AtlantTabWidget.h"

AtlantTabWidget::AtlantTabWidget(QWidget* parent) :
	QWidget(parent),
	_id(6),
	ui(new Ui::AtlantTabWidget)
{
	ui->setupUi(this);

	_tab_property = new TabsProperty(this);
	_tab_property->set_id(_id);
	_tab_property->set_name(tr("Atlant"));

	connect(ui->sendPB, SIGNAL(clicked()), this, SLOT(_slot_send()));
	connect(this, SIGNAL(signalAddLog(QString)), ui->logsTE, SLOT(append(QString)));
	createRPC();

}

AtlantTabWidget::~AtlantTabWidget()
{
	if(_rpc_client != NULL) {
		delete _rpc_client;
	}
}

void AtlantTabWidget::setLog(QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadWrite);
	int source_id;
	int request_id;
	double freq;
	double width;
	double direction;
	double angle;
	double level;
	double quality;
	ds >> source_id;
	ds >> request_id;
	ds >> freq;
	ds >> width;
	ds >> direction;
	ds >> angle;
	ds >> level;
	ds >> quality;

	QString str;
	str = QString::number(source_id);
	str.append(" ");
	str.append(QString::number(request_id));
	str.append(" ");
	str.append(QString::number(freq));
	str.append(" ");
	str.append(QString::number(width));
	str.append(" ");
	str.append(QString::number(direction));
	str.append(" ");
	str.append(QString::number(angle));
	str.append(" ");
	str.append(QString::number(level));
	str.append(" ");
	str.append(QString::number(quality));
	emit signalAddLog(str);

}

int AtlantTabWidget::createRPC()
{
	QString settingsFile = QCoreApplication::applicationDirPath();
	settingsFile.append("/Tabs/RPC.ini");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(settingsFile, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	QString ipRpc = m_settings.value("RPC_UI/IP", "127.0.0.1").toString();
	quint16 portRpc = m_settings.value("RPC_UI/Port", 24500).toInt();

	_rpc_client = new RPCAtlant(_tab_property->get_id(), this, this);
	_rpc_client->start(ipRpc, portRpc);
	/*QThread *thread_rpc_client = new QThread;

	connect(thread_rpc_client, SIGNAL(started()), _rpc_client, SLOT(slotInit()));
	connect(thread_rpc_client, SIGNAL(started()), this, SLOT(_slotStart()));

	connect(this, SIGNAL(signalStartRPC()), _rpc_client, SLOT(slotStart()));
	connect(_rpc_client, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
	connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

	connect(_rpc_client, SIGNAL(signalFinished()), _rpc_client, SLOT(deleteLater()));
	connect(this, SIGNAL(signalStopRPC()), _rpc_client, SLOT(slotStop()));
	connect(this, SIGNAL(signalFinishRPC()), _rpc_client, SLOT(slotFinish()));

	_rpc_client->setParent(0);
	_rpc_client->moveToThread(thread_rpc_client);
	thread_rpc_client->start();*/

	return 0;
}

int AtlantTabWidget::closeRPC()
{
	emit signalFinishRPC();
	return 0;
}

void AtlantTabWidget::_slotStart()
{
	emit signalStartRPC();
}

void AtlantTabWidget::_slot_send()
{
	QByteArray* ba = new QByteArray();
	QDataStream ds(ba, QIODevice::WriteOnly);

	int id = 1;
	int post  = 1;
	ds << id;
	ds << post;
	ds << ui->freqSB->value();
	ds << ui->widthSB->value();


	rpc_send_atlant_data ba1(*ba);

	CommandMessage* msg = new CommandMessage(COMMAND_ATLANT_SET_FREQ, QVariant::fromValue(ba1));
	_rpc_client->set_command(msg);
}

