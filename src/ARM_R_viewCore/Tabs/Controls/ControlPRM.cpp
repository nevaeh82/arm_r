#include "ControlPRM.h"
#include <QDebug>

ControlPRM::ControlPRM(QWidget *parent, ITabSpectrum* tab) : QWidget(parent)
{
    _tab = tab;
//    _panel_controls_widget = new QWidget();

//    QPalette p(palette());
//    QColor cl(200, 200, 200, 150);

//    p.setColor(QPalette::Background, cl);
//    this->setAutoFillBackground(true);
//    this->setPalette(p);
    _vbox_panel = new QVBoxLayout();
    _vbox_panel->setAlignment(Qt::AlignTop);

    QHBoxLayout *hb_att1 = new QHBoxLayout();
    _ln_freq = new QLCDNumber(4);
    _ln_freq->display(0);
    _vbox_panel->addWidget(_ln_freq/*, Qt::AlignCenter | Qt::AlignTop*/);

    _gb_att1 = new QGroupBox("Аттенюатор 1");
    _tb_att1_up = new QToolButton();
    _tb_att1_up->setText("+");
    _tb_att1_up->setFixedSize(40,40);
    hb_att1->addWidget(_tb_att1_up);

    _ln_att1 = new QLCDNumber(2);
    _ln_att1->display(-1);
    hb_att1->addWidget(_ln_att1);
    _tb_att1_down = new QToolButton();
    _tb_att1_down->setText("-");
    _tb_att1_down->setFixedSize(40,40);

    hb_att1->addWidget(_tb_att1_down);

    _gb_att1->setLayout(hb_att1);
    _vbox_panel->addWidget(_gb_att1);

    QHBoxLayout *hb_att2 = new QHBoxLayout();
    _gb_att2 = new QGroupBox("Аттенюатор 2");

    _tb_att2_up = new QToolButton();
    _tb_att2_up->setText("+");
    _tb_att2_up->setFixedSize(40,40);

    hb_att2->addWidget(_tb_att2_up);

    _ln_att2 = new QLCDNumber(2);
    _ln_att2->display(-1);
    hb_att2->addWidget(_ln_att2);

    _tb_att2_down = new QToolButton();
    _tb_att2_down->setText("-");
    _tb_att2_down->setFixedSize(40,40);


    hb_att2->addWidget(_tb_att2_down);
    _gb_att2->setLayout(hb_att2);
    _vbox_panel->addWidget(_gb_att2);

    _cb_filter = new QComboBox();
    _cb_filter->addItem("140/70 МГц");
    _cb_filter->addItem("140/22 МГц");
    _cb_filter->addItem("140/20 МГц");
    _cb_filter->addItem("150/80 МГц");

    _vbox_panel->addWidget(_cb_filter);
    this->setLayout(_vbox_panel);

    connect(_tb_att1_up, SIGNAL(clicked()), this, SLOT(_slotAtt1Up()));
    connect(_tb_att1_down, SIGNAL(clicked()), this, SLOT(_slotAtt1Down()));
    connect(_tb_att2_up, SIGNAL(clicked()), this, SLOT(_slotAtt2Up()));
    connect(_tb_att2_down, SIGNAL(clicked()), this, SLOT(_slotAtt2Down()));
    connect(_cb_filter, SIGNAL(currentIndexChanged(int)), this, SLOT(_slotSetFilter(int)));
}

ControlPRM::~ControlPRM()
{
//    delete _animation;
//    delete tb_map;
//    delete tb_spectrum;
    //    delete tb_spectra;
}

void ControlPRM::set_status(QMap<int, QVariant> data)
{
    int att1 = 0;
    QMap<int, QVariant>::iterator it;
    for(it = data.begin(); it != data.end(); ++it)
    {
        switch(it.key())
        {
        /// frequency
        case 0:
            _ln_freq->display(it.value().toInt());
            break;
        /// filter
        case 1:
            _cb_filter->setCurrentIndex(it.value().toInt());
            break;
        /// att1
        case 2:
            if(it.value().toInt() == 1)
            {
                att1 = 10;
            }
            _ln_att1->display(att1);
            break;
        /// att2
        case 3:
            _ln_att2->display(it.value().toInt());
            break;
        default:
            break;
        }
    }
}

void ControlPRM::_add_widgets_to_panel(QWidget *widget)
{
    _vbox_panel->addWidget(widget);
}



void ControlPRM::leaveEvent(QEvent *event)
{
//    this->hide();
}

void ControlPRM::slotShow()
{
    this->show();
//    _animation->start();
}

void ControlPRM::slotSetSize(int width, int height)
{
//    _animation->setStartValue(QRect(width, 0, 0, 0));
    //    _animation->setEndValue(QRect(width - 50, 0, 50, height));
}

void ControlPRM::_setAtt1(int value)
{
    CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_ATT1, QVariant::fromValue(value));
	_tab->set_command(TypeCommand(TypeGraphicCommand), msg);
}

void ControlPRM::_setAtt2(int value)
{
    CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_ATT2, QVariant::fromValue(value));
	_tab->set_command(TypeCommand(TypeGraphicCommand), msg);
}

void ControlPRM::_slotAtt1Up()
{
    if(_ln_att1->value() == 10)
    {
        return;
    }
    int value = 10;
    int aval = 1;
    _ln_att1->display(value);
    _setAtt1(aval);
}

void ControlPRM::_slotAtt1Down()
{
    if(_ln_att1->value() == 0)
    {
        return;
    }
    int value = 0;
    _ln_att1->display(value);
    _setAtt1(value);
}

void ControlPRM::_slotAtt2Up()
{
    if(_ln_att2->value() == 15)
    {
        return;
    }
    int value = _ln_att2->value();
    value++;
    _ln_att2->display(value);
    _setAtt2(value);
}

void ControlPRM::_slotAtt2Down()
{
    if(_ln_att2->value() == 0)
    {
        return;
    }
    int value = _ln_att2->value();
    value--;
    _ln_att2->display(value);
    _setAtt2(value);
}

void ControlPRM::_slotSetFilter(int index)
{
    CommandMessage *msg = new CommandMessage(COMMAND_PRM_SET_FILTER, QVariant::fromValue(index));
	_tab->set_command(TypeCommand(TypeGraphicCommand), msg);
}
