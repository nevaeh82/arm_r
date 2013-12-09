#include "ButtonShowPanel.h"

#include <QDebug>

ButtonShowPanel::ButtonShowPanel(QWidget *parent) :
    QToolButton(parent)
{
    this->setMouseTracking(true);
    QPixmap _pm_tab_manager(":/images/tabs/images/Tabs/tabs_manager.png");
    this->setIcon(QIcon(_pm_tab_manager));
}

ButtonShowPanel::~ButtonShowPanel()
{

}

void ButtonShowPanel::enterEvent(QEvent *event)
{

    emit signalPanelShow();
}

void ButtonShowPanel::sh()
{

}
