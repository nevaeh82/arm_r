#ifndef SERVERCONNECTIONWIDGETCONTROLLER_H
#define SERVERCONNECTIONWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "ServerConnectionsWidget.h"
#include "Interfaces/IRpcListener.h"

class ServerConnectionWidgetController: public QObject, public IController<ServerConnectionsWidget>,
        public IRpcListener
{
Q_OBJECT
public:
    explicit ServerConnectionWidgetController(QObject *parent = 0);
    virtual ~ServerConnectionWidgetController();


private:
    ServerConnectionsWidget* m_view;

public:
    void appendView(ServerConnectionsWidget* view);
    virtual void onMethodCalled(const QString& method, const QVariant& argument);
    ServerConnectionsWidget* getView();
    void init();

public slots:
    void slotShowWidget();
    void addedNewConnectionSlot(int id, QString ip, quint16 port);
    void removeConnectionSlot(int id);
    void addedNewConnectionExtSlot(int id, QString ip, quint16 port);

signals:
    void onMethodCalledSignal(QString, QVariant);

private slots:
    void onMethodCalledSlot(QString method, QVariant data);

signals:
    void signalAddedNewConnection(int, QString, quint16);
    void signalRemoveConnection(int);

};

#endif // SERVERCONNECTIONWIDGETCONTROLLER_H
