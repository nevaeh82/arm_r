#include "ServerConnectionSingleLineWidget.h"
#include "ui_ServerConnectionSingleLineWidget.h"

ServerConnectionSingleLineWidget::ServerConnectionSingleLineWidget(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerConnectionSingleLineWidget),
    m_id(id),
    m_isBlank(true)
{
    ui->setupUi(this);
    ui->tblRemoveConnection->hide();
    connect(ui->tblAddConnection, SIGNAL(clicked(bool)), this, SLOT(addServerConnection(bool)));
    connect(ui->tblRemoveConnection, SIGNAL(clicked(bool)), this, SLOT(removeServerConnection(bool)));
}

ServerConnectionSingleLineWidget::~ServerConnectionSingleLineWidget()
{
    delete ui;
}

QString ServerConnectionSingleLineWidget::getIp()
{
    return ui->leIp->text();
}

quint16 ServerConnectionSingleLineWidget::getPort()
{
    return ui->sbPort->value();
}

int ServerConnectionSingleLineWidget::getId()
{
    return m_id;
}

void ServerConnectionSingleLineWidget::accept(QString ip, quint16 port)
{
    ui->leIp->setText(ip);
    ui->sbPort->setValue(port);
    if(ip == "...")
    {
        ui->tblAddConnection->show();
        ui->tblRemoveConnection->hide();
    } else {
        ui->tblAddConnection->hide();
        ui->tblRemoveConnection->show();
    }

}

void ServerConnectionSingleLineWidget::setDefault()
{
    ui->tblAddConnection->show();
    ui->tblRemoveConnection->hide();
}

bool ServerConnectionSingleLineWidget::isBlank()
{
    if(!ui->tblAddConnection->isVisible())
    {
        m_isBlank = false;
    }
    else {
        m_isBlank = true;
    }
    return m_isBlank;
}

void ServerConnectionSingleLineWidget::removeServerConnection(bool state)
{
    m_isBlank = true;
    emit removeLine(m_id);
}

void ServerConnectionSingleLineWidget::addServerConnection(bool state)
{
    if(ui->leIp->text() == "...")
    {
        return;
    }
    ui->tblAddConnection->hide();
    ui->tblRemoveConnection->show();
    emit addNewLine(m_id, getIp(), getPort());
    //emit signalAddedNewConnectionSignal(m_id, ui->leIp->text(), ui->sbPort->value());
}
