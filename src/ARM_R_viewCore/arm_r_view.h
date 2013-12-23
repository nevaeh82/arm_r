#ifndef ARM_R_VIEW_H
#define ARM_R_VIEW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QGridLayout>

#include <QMenu>
#include <QAction>
#include <QList>

#include "Tabs/TabManager.h"

#include "ControlPanel.h"

#include "Common/ServiceHandler/ServiceHandler.h"

typedef QVector<QPointF>         rpc_send_points_vector;
typedef QByteArray               rpc_send_atlant_data;


namespace Ui {
class ARM_R_view;
}

using namespace Pw::Common::ServiceControl;

class ARM_R_view : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ARM_R_view(QWidget *parent = 0);
    ~ARM_R_view();
    
private:
    Ui::ARM_R_view*     ui;
    TabManager*         _tab_manager;
    QGridLayout*        _layout;

    QMenu               *fileMenu;
    QAction             *newAct;
    QAction             *exitAct;
    QAction             *diagPelengatorAct;

    QMenu               *settingsMenu;

    ControlPanel*       _control_panel;

    /// start server
    QList<ServiceHandler*>  _serviceList;
    QStringList             _serviceNames;


    /// logger
    static Pw::Logger::ILogger* m_logger;


private:
    void _create_actions();
    void _create_menu();
    void _start_services();
};
Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(rpc_send_points_vector)
Q_DECLARE_METATYPE(quint32)
Q_DECLARE_METATYPE(rpc_send_atlant_data)


#endif // ARM_R_VIEW_H
