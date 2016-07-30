#ifndef SERVERCONNECTIONSWIDGET_H
#define SERVERCONNECTIONSWIDGET_H

#include <QDialog>
#include <QMap>
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

private:
    Ui::ServerConnectionsWidget *ui;
    QMap<int, ServerConnectionSingleLineWidget*> m_maplines;
    int m_counter;

private slots:
    void addNewLineSlot(int id, QString ip, quint16 port);
    void removeLineSlot(int id);

signals:
    void signalAddedNewConnectionSignal(int, QString, quint16);
};

#endif // SERVERCONNECTIONSWIDGET_H
