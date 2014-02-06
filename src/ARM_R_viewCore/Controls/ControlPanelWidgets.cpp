#include "ControlPanelWidgets.h"
#include <QDebug>

ControlPanelWidgets::ControlPanelWidgets(QWidget *parent)
    : QWidget(parent)
{
//    _panel_controls_widget = new QWidget();

    QPalette p(palette());
    QColor cl(200, 200, 200, 150);

    p.setColor(QPalette::Background, cl);
    this->setAutoFillBackground(true);
    this->setPalette(p);
    _vbox_panel = new QVBoxLayout();
    _vbox_panel->setAlignment(Qt::AlignTop);
    this->setLayout(_vbox_panel);
    this->setMaximumWidth(50);

    /// set animation effect for panel
    _animation = new QPropertyAnimation(this, "geometry");
    _animation->setDuration(500);
    _animation->setStartValue(QRect(parent->minimumWidth(), 0, 0, 0));
    _animation->setEndValue(QRect(parent->minimumWidth() - 50, 0, 50, parent->height()));
    _animation->setEasingCurve(QEasingCurve::Linear);

    this->hide();
//    tb_map = new QToolButton(this);
//    QPixmap _pm_tab_map(":/images/tabs/images/Tabs/map.png");
//    tb_map->setIcon(QIcon(_pm_tab_map.scaled(32,32,Qt::KeepAspectRatio)));
//    tb_map->setFixedSize(32, 32);
//    tb_map->setToolTip(tr("Переключиться на карту"));
    int left, right, top, bottom;
    _vbox_panel->getContentsMargins(&left, &top, &right, &bottom);
    _vbox_panel->setContentsMargins(left, top + 20, right, bottom);
//    _add_widgets_to_panel(tb_map);

    tb_spectrum = new QToolButton(this);
    tb_spectrum->setText(tr("Спектр"));
    QPixmap _pm_tab_spectrum(":/images/tabs/images/Tabs/spectrum.png");
    tb_spectrum->setIcon(QIcon(_pm_tab_spectrum.scaled(32,32,Qt::KeepAspectRatio)));
    tb_spectrum->setFixedSize(32, 32);
    tb_spectrum->setToolTip(tr("Показать текущий спектр"));
    _add_widgets_to_panel(tb_spectrum);

    tb_spectra = new QToolButton(this);
    tb_spectra->setText(tr("Спектры"));
    QPixmap _pm_tab_spectra(":/images/tabs/images/Tabs/spectra.png");
    tb_spectra->setIcon(QIcon(_pm_tab_spectra.scaled(32,32,Qt::KeepAspectRatio)));
    tb_spectra->setFixedSize(32, 32);
    tb_spectra->setToolTip(tr("Показать все спектры"));
    _add_widgets_to_panel(tb_spectra);

//    tb_correlations = new QToolButton(this);
//    tb_correlations->setText("cor");
//    QPixmap _pm_tab_correlations(":/images/tabs/images/Tabs/spectra.png");
//    tb_correlations->setIcon(QIcon(_pm_tab_spectra.scaled(32,32,Qt::KeepAspectRatio)));
//    tb_correlations->setFixedSize(32, 32);
//    _add_widgets_to_panel(tb_correlations);

}

ControlPanelWidgets::~ControlPanelWidgets()
{
    delete _animation;
//    delete tb_map;
    delete tb_spectrum;
    delete tb_spectra;
}

void ControlPanelWidgets::_add_widgets_to_panel(QWidget *widget)
{
    _vbox_panel->addWidget(widget);
}

void ControlPanelWidgets::leaveEvent(QEvent *event)
{
    this->hide();
}

void ControlPanelWidgets::slotShow()
{
    this->show();
    _animation->start();
}

void ControlPanelWidgets::slotSetSize(int width, int height)
{
    _animation->setStartValue(QRect(width, 0, 0, 0));
    _animation->setEndValue(QRect(width - 50, 0, 50, height));
}
