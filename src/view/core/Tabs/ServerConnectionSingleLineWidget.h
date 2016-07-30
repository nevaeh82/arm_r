#ifndef SERVERCONNECTIONSINGLELINEWIDGET_H
#define SERVERCONNECTIONSINGLELINEWIDGET_H

#include <QWidget>

namespace Ui {
class ServerConnectionSingleLineWidget;
}

class ServerConnectionSingleLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerConnectionSingleLineWidget(int id, QWidget *parent = 0);
    ~ServerConnectionSingleLineWidget();
    QString getIp();
    quint16 getPort();

private:
    Ui::ServerConnectionSingleLineWidget *ui;
    int m_id;

private slots:
    void addServerConnection(bool state);
    void removeServerConnection(bool state);

signals:
    void addNewLine(int id, QString ip, quint16 port);
    void removeLine(int id);
};

#endif // SERVERCONNECTIONSINGLELINEWIDGET_H
