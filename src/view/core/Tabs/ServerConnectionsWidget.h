#ifndef SERVERCONNECTIONSWIDGET_H
#define SERVERCONNECTIONSWIDGET_H

#include <QDialog>
#include <QMap>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include "ServerConnectionSingleLineWidget.h"

namespace Ui {
class ServerConnectionsWidget;
}

class ServerConnectionsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ServerConnectionsWidget(QWidget *parent = 0);
    ~ServerConnectionsWidget();

    void init();

private:
    Ui::ServerConnectionsWidget *ui;
    QMap<int, ServerConnectionSingleLineWidget*> m_maplines;
    int m_counter;

    void saveServers();

    int checkExistServers(QString ip, quint16 port);

public slots:
    void addNewLineSlot(int id, QString ip, quint16 port);
    void addNewLineFromFileSlot(int id, QString ip, quint16 port);

private slots:

    void removeLineSlot(int id);

signals:
    void signalAddedNewConnectionSignal(int, QString, quint16);
    void signalRemoveConnection(int);
};

#endif // SERVERCONNECTIONSWIDGET_H
