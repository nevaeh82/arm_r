#ifndef ARM_OD_VIEW_H
#define ARM_OD_VIEW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QGridLayout>
#include <QPointF>
#include <QMenu>
#include <QAction>

#include "Tabs/TabManager.h"
#include "Tabs/Solver/Solver.h"

typedef QVector<QPointF>         rpc_send_points_vector;
typedef QPointF                  rpc_QPointF;
typedef QVector<DataFly>         rpc_data_fly;

namespace Ui {
class ARM_OD_view;
}

class ARM_OD_view : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ARM_OD_view(QWidget *parent = 0);
    ~ARM_OD_view();
    
private:
    Ui::ARM_OD_view*     ui;
    TabManager*         _tab_manager;
    QGridLayout*        _layout;

    QMenu   *fileMenu;
    QAction *newAct;
    QAction *exitAct;
    QAction* solverAction;
    QAction *diagMsgServerAct;
    QAction *diagPelengatorAct;
    QAction *diagDBListAct;
    QAction *diagKTRSettings;

    QMenu   *mapMenu;
    QMenu   *settingsMenu;
    QAction *openMapFromAtlas;
    QAction *openMapFromLocalFile;

    Solver* _solver;


private:
    void _read_settings();
    void _create_actions();
    void _create_menu();
};
Q_DECLARE_METATYPE(rpc_send_points_vector)
Q_DECLARE_METATYPE(rpc_QPointF)
Q_DECLARE_METATYPE(rpc_data_fly)


#endif // ARM_OD_VIEW_H
