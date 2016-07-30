#include "ServerConnectionWidgetController.h"

ServerConnectionWidgetController::ServerConnectionWidgetController(QObject* parent):
    QObject(parent)
{
    m_view = NULL;

    connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));

}

ServerConnectionWidgetController::~ServerConnectionWidgetController()
{
    if(m_view != NULL)
    {
        delete m_view;
    }
}

void ServerConnectionWidgetController::appendView(ServerConnectionsWidget *view)
{
    m_view = view;
    connect(m_view, SIGNAL(signalAddedNewConnectionSignal(int, QString,quint16)), this, SLOT(addedNewConnectionSlot(int, QString, quint16)));
}

void ServerConnectionWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
//	emit onMethodCalledSignal(method, argument);
}

ServerConnectionsWidget* ServerConnectionWidgetController::getView()
{
    return m_view;
}

void ServerConnectionWidgetController::slotShowWidget()
{
    if(m_view != NULL)
    {
        m_view->show();
    }
}

void ServerConnectionWidgetController::addedNewConnectionSlot(int id, QString ip, quint16 port)
{
    emit signalAddedNewConnection(id, ip, port);
}

void ServerConnectionWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
//	if( method == RPC_SLOT_SERVER_SEND_SOLVER_ERRORS ) {
//		addResultToLog(argument.toByteArray());
//		return;
//	}
}

