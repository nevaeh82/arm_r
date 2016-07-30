#include "ServerConnectionSingleLineWidget.h"
#include "ui_ServerConnectionSingleLineWidget.h"

ServerConnectionSingleLineWidget::ServerConnectionSingleLineWidget(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerConnectionSingleLineWidget),
    m_id(id)
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

void ServerConnectionSingleLineWidget::removeServerConnection(bool state)
{
    emit removeLine(m_id);
}

void ServerConnectionSingleLineWidget::addServerConnection(bool state)
{
    ui->tblAddConnection->hide();
    ui->tblRemoveConnection->show();
    emit addNewLine(m_id, getIp(), getPort());
    //emit signalAddedNewConnectionSignal(m_id, ui->leIp->text(), ui->sbPort->value());
}
