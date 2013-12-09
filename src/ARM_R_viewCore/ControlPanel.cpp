#include "ControlPanel.h"

ControlPanel::ControlPanel(QWidget* parent = 0)
{
//    _panel_widget = new QWidget(this);
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::gray);
    this->setAutoFillBackground(true);
    this->setPalette(p);

    QHBoxLayout *hbox = new QHBoxLayout(this);

    _sb_from_freq = new QSpinBox();
    _sb_from_freq->setValue(0);
    _sb_from_freq->setRange(300, 7000);
    _sb_from_freq->setMaximumWidth(100);
    hbox->addWidget(_sb_from_freq, Qt::AlignLeft);

    QLabel* lbl1 = new QLabel(tr("-"));
    lbl1->setMaximumWidth(10);
    hbox->addWidget(lbl1, Qt::AlignLeft);


    _sb_to_freq = new QSpinBox();
    _sb_to_freq->setValue(0);
    _sb_to_freq->setRange(300, 7000);
    _sb_to_freq->setMaximumWidth(100);
    hbox->addWidget(_sb_to_freq, Qt::AlignLeft);

    QVBoxLayout* v1 = new QVBoxLayout();

    _chb_auto_search_common = new QCheckBox(tr("Автопоиск"));
    _chb_auto_search_common->setChecked(false);
    v1->addWidget(_chb_auto_search_common, Qt::AlignTop);

    _chb_panorama_common = new QCheckBox(tr("Панорама"));
    _chb_panorama_common->setChecked(false);
    v1->addWidget(_chb_panorama_common, Qt::AlignTop);

    hbox->addLayout(v1, Qt::AlignLeft);

    _sb_freq = new QSpinBox();
    _sb_freq->setValue(0);
    _sb_freq->setRange(300, 7000);
    _sb_freq->setMaximumWidth(100);
    hbox->addWidget(_sb_freq, Qt::AlignLeft);

    _pb_set_freq = new QPushButton(tr("Общая частота"));
    _pb_set_freq->setMaximumWidth(100);

    hbox->addWidget(_pb_set_freq, Qt::AlignLeft);

//    this->setAlignment(Qt::AlignLeft);
//    _panel_widget->setLayout(hbox);
//    _panel_widget->setMaximumHeight(50);

//    _panel_widget->setLayout(hbox);
//    _panel_widget->setMaximumHeight(50);



    this->setMinimumHeight(50);

    this->setLayout(hbox);
}

ControlPanel::~ControlPanel()
{
}
