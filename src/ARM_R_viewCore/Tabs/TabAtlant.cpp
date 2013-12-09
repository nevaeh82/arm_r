#include "TabAtlant.h"

TabAtlant::TabAtlant(/*TabsProperty* prop*/) :
    _id(6)
{
    _tab_property = new TabsProperty();
    _tab_property->set_id(_id);
    _tab_property->set_name("Atlant");
    QGridLayout *gl = new QGridLayout;
//    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *lbl1 = new QLabel(tr("Режим"));

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(lbl1/*, Qt::AlignTop | Qt::AlignLeft*/);

//    gl->addWidget(lbl1, 0, 0, Qt::AlignLeft);

    _cb_mode = new QComboBox();
    _cb_mode->addItem(tr("Адаптация"));
    _cb_mode->addItem(tr("Дежурство"));
    _cb_mode->addItem(tr("Проверка"));
    _cb_mode->addItem(tr("Работа"));
    _cb_mode->setFixedWidth(200);

    QHBoxLayout* hb1 = new QHBoxLayout();
    hb1->addWidget(_cb_mode, Qt::AlignLeft);

    _dsb_freq = new QDoubleSpinBox();
    _dsb_freq->setRange(800, 3000);
    _dsb_freq->setFixedWidth(150);
    hb1->addWidget(_dsb_freq, Qt::AlignRight);

    _dsb_width = new QDoubleSpinBox();
    _dsb_width->setRange(0, 20);
    _dsb_width->setFixedWidth(150);
    hb1->addWidget(_dsb_width,Qt::AlignRight);

    _pb_send = new QPushButton(tr("Отправить"));
    hb1->addWidget(_pb_send, Qt::AlignRight);

    connect(_pb_send, SIGNAL(clicked()), this, SLOT(_slot_send()));

    vbox->addLayout(hb1);

//    _pbStop = new QPushButton("Stop");
//    hbox->addWidget(_pbStop, Qt::AlignLeft);

    gl->addLayout(vbox, 0, 0, Qt::AlignTop | Qt::AlignLeft);
    _txt_log = new QTextEdit();
    connect(this, SIGNAL(signalAddLog(QString)), _txt_log, SLOT(append(QString)));

    gl->addWidget(_txt_log, 1, 0);

    this->setLayout(gl);
    createRPC();

}

TabAtlant::~TabAtlant()
{
    if(_rpc_client != NULL)
    {
        delete _rpc_client;
    }
    delete _cb_mode;
    delete _dsb_freq;
    delete _dsb_width;
    delete _tab_property;
}

void TabAtlant::setLog(QByteArray data)
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

int TabAtlant::createRPC()
{
    _rpc_client = new RPCAtlant(_tab_property->get_id(), this);
    QThread *thread_rpc_client = new QThread;

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
    thread_rpc_client->start();

    return 0;
}

int TabAtlant::closeRPC()
{
    emit signalFinishRPC();
    return 0;
}

void TabAtlant::_slotStart()
{
    emit signalStartRPC();

    //    return 0;
}

void TabAtlant::_slot_send()
{
    QByteArray* ba = new QByteArray();
    QDataStream ds(ba, QIODevice::WriteOnly);

    int id = 1;
    int post  = 1;
    ds << id;
    ds << post;
    ds << _dsb_freq->value();
    ds << _dsb_width->value();


    rpc_send_atlant_data ba1(*ba);

    CommandMessage* msg = new CommandMessage(COMMAND_ATLANT_SET_FREQ, QVariant::fromValue(ba1));
    _rpc_client->set_command(msg);
}

