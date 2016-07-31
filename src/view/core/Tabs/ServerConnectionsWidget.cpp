#include "ServerConnectionsWidget.h"
#include "ui_ServerConnectionsWidget.h"

ServerConnectionsWidget::ServerConnectionsWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerConnectionsWidget)
{
    ui->setupUi(this);
    m_counter = 0;
}

void ServerConnectionsWidget::init()
{
    ServerConnectionSingleLineWidget *line = new ServerConnectionSingleLineWidget(m_counter);
    ui->vloyoutConnactions->addWidget(line);
    connect(line, SIGNAL(addNewLine(int, QString, quint16)), this, SLOT(addNewLineSlot(int, QString, quint16)));
    connect(line, SIGNAL(removeLine(int)), this, SLOT(removeLineSlot(int)));
    m_maplines.insert(m_counter++, line);
    this->adjustSize();
}

ServerConnectionsWidget::~ServerConnectionsWidget()
{
    saveServers();
    delete ui;
}

void ServerConnectionsWidget::saveServers()
{
    QString stationsSettingsFile = QCoreApplication::applicationDirPath();
    stationsSettingsFile.append("./TCP/servers.ini");

    ///clear settings file before write
    QFile f(stationsSettingsFile);
    f.open(QIODevice::WriteOnly);
    f.close();
    ///

    QSettings settings(stationsSettingsFile, QSettings::IniFormat, this);
    QMapIterator<int, ServerConnectionSingleLineWidget*> i(m_maplines);
    while (i.hasNext()) {
          i.next();

            if(i.value()->getIp() != "...")
            {
              settings.beginGroup(QString::number(i.key()));
              ServerConnectionSingleLineWidget* line = i.value();
              settings.setValue("id", i.key());
              settings.setValue("ip", line->getIp());
              settings.setValue("port", line->getPort());
              settings.endGroup();
            }
      }
}

void ServerConnectionsWidget::addNewLineSlot(int id, QString ip, quint16 port)
{
    ServerConnectionSingleLineWidget *line = new ServerConnectionSingleLineWidget(m_counter);
    m_maplines.value(id)->accept(ip, port);
    ui->vloyoutConnactions->addWidget(line);
    m_maplines.insert(m_counter++, line);
    connect(line, SIGNAL(addNewLine(int, QString, quint16)), this, SLOT(addNewLineSlot(int, QString, quint16)));
    connect(line, SIGNAL(removeLine(int)), this, SLOT(removeLineSlot(int)));
    emit signalAddedNewConnectionSignal(id, ip, port);
}

void ServerConnectionsWidget::addNewLineFromFileSlot(int id, QString ip, quint16 port)
{
    ServerConnectionSingleLineWidget *line = new ServerConnectionSingleLineWidget(m_counter);
    line->accept(ip, port);
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
    emit signalRemoveConnection(id);
}

