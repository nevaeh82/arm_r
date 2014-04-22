#include "AtlantTabWidget.h"

#include "ui_AtlantTabWidget.h"

AtlantTabWidget::AtlantTabWidget(QWidget* parent) :
	QWidget(parent),
	m_id(6),
	ui(new Ui::AtlantTabWidget)
{
	ui->setupUi(this);

	connect(ui->sendPB, SIGNAL(clicked()), this, SLOT(_slot_send()));
	connect(this, SIGNAL(signalAddLog(QString)), ui->logsTE, SLOT(append(QString)));
    connect(ui->pbOpenRDP, SIGNAL(clicked()), this, SLOT(slotOpenRDP()));

	m_rpcClient = new RpcAtlantClient( m_id, this, this );
}

AtlantTabWidget::~AtlantTabWidget()
{
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

void AtlantTabWidget::setConfig(const AtlantConfiguration& config)
{
	m_rpcClient->start(config.portByRpc, QHostAddress(config.hostByRpc));
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
    m_rpcClient->set_command(msg);
}

void AtlantTabWidget::slotOpenRDP()
{
    int ret = openRDP();
    log_debug(QString("Open RDP for CellNet = %1").arg(QString::number(ret)));
}

int AtlantTabWidget::openRDP()
{
    QProcess process;
//    process.setReadChannelMode(ForwardedChannels);
    process.start("mstsc RDP/cell.net.RDP");
    if (!process.waitForFinished(-1))
        return -2;
    return process.exitStatus() == QProcess::NormalExit ? process.exitCode() : -1;
}

