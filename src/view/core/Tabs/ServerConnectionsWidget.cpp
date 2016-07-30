#include "ServerConnectionsWidget.h"
#include "ui_ServerConnectionsWidget.h"

ServerConnectionsWidget::ServerConnectionsWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerConnectionsWidget)
{
    ui->setupUi(this);
    m_counter = 0;

    ServerConnectionSingleLineWidget *line = new ServerConnectionSingleLineWidget(m_counter);
    ui->vloyoutConnactions->addWidget(line);
    connect(line, SIGNAL(addNewLine(int, QString, quint16)), this, SLOT(addNewLineSlot(int, QString, quint16)));
    connect(line, SIGNAL(removeLine(int)), this, SLOT(removeLineSlot(int)));
    m_maplines.insert(m_counter++, line);
    this->adjustSize();
}

ServerConnectionsWidget::~ServerConnectionsWidget()
{
    delete ui;
}

void ServerConnectionsWidget::addNewLineSlot(int id, QString ip, quint16 port)
{
    ServerConnectionSingleLineWidget *line = new ServerConnectionSingleLineWidget(m_counter);
    ui->vloyoutConnactions->addWidget(line);
    m_maplines.insert(m_counter++, line);
    connect(line, SIGNAL(addNewLine(int, QString, quint16)), this, SLOT(addNewLineSlot(int, QString, quint16)));
    connect(line, SIGNAL(removeLine(int)), this, SLOT(removeLineSlot(int)));
    emit signalAddedNewConnectionSignal(id, ip, port);
}

void ServerConnectionsWidget::removeLineSlot(int id)
{
    ServerConnectionSingleLineWidget *line = m_maplines.value(id);
    ui->vloyoutConnactions->removeWidget(line);
    delete line;
    m_maplines.remove(id);
    this->adjustSize();
}

