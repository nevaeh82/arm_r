#ifndef TABSPECTRUM_H
#define TABSPECTRUM_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QStackedWidget>

#include <QThread>



#include "../Abstracts/ISubModule.h"
#include "../Abstracts/ICommonComponents.h"

#include "TabsProperty.h"
#include "../GraphicWidget.h"
#include "../GraphicData.h"

#include "../CorrelationWidget.h"

#include "Tree/TreeView.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"
#include "ITabSpectrum.h"

#include "RPC/RPCClient.h"

#include "Controls/ControlPRM.h"

#include "../Controls/ControlPanelWidgets.h"
#include "../Controls/ButtonShowPanel.h"

class TabSpectrum: public QWidget, public ISubModule, public ITabSpectrum
{
    Q_OBJECT
public:
    TabSpectrum(TabsProperty* prop, ICommonComponents* common_components, ICommonComponents* common_correlations, TreeModel* model, DBManager* db_manager, ITabManager* tab_manager);
    ~TabSpectrum();

private:
    int                 _id;
    QString             _name;
    TabsProperty*       _tab_property;
    ICommonComponents*  _common_components;
    ICommonComponents*  _common_correlations;
    GraphicWidget*      _spectrumWidget;
    GraphicData*        _spectrumData;


    QHBoxLayout*        _hboxlayout;

    TreeView*           _tree_view;

    IDBManager*          _db_manager;

    TreeModel*          _model;

    RPCClient*          _rpc_client1;

    QMap<int, IGraphicWidget *>*     _map_correlation_widget;
    ITabManager*        _tab_manager;

    QVBoxLayout*        _vboxlayout;

    ControlPRM*         _controlPRM;
    QDockWidget*        _dock_controlPRM;

    double              _threshold;

    QPixmap*            _pm_round_red;
    QPixmap*            _pm_round_green;
    QLabel*             _indicator;

    ControlPanelWidgets*    _control_widget;

    QStackedWidget*         _view_stacked_widget;
    QWidget*                _com_spectra_widget;

    QGridLayout*            _gl_spectra;

    QWidget*                _com_spectrum_widget;
    QGridLayout*            _gl_spectrum;

    int                     _current_stecked_widget;



//    CorrelationWidget *cor;

//    IController         *_models_controller;

//    QWidget *view;
//    RSpectr*

public:
    virtual int start();
    virtual int stop();
    virtual void show();
    virtual void hide();
    virtual int createRPC();
    virtual int closeRPC();
    virtual int createView(QWidget* view);
    virtual int createTree();

    virtual void set_indicator(int state);
    virtual QLabel* get_indicator();

    virtual void set_show_controlPRM(bool state);
    virtual void set_double_clicked(int id, double, double);

public:
    virtual void set_selected_area(QMap<int, QVariant> data);
    virtual void set_command(IMessage *msg);
    virtual void set_points_rpc(QVector<QPointF> points);

    virtual void set_thershold(double y);

    virtual void check_status();

private:
    int _init();
    void _init_spectra();
    void _init_spectrum();



public:
    TabsProperty *get_tab_property();


signals:
    void signalStartRPC();
    void signalStopRPC();
    void signalFinishRPC();
    void signalGetPointsFromRPCFlakon(QVector<QPointF> points);

    void signalChangeIndicator(int state);

    void signalDoubleClicked(int id, double, double);

private slots:
    void _slot_get_points_from_rpc(QVector<QPointF> points);

    void _slot_set_indicator(int state);

    void _slot_show_spectra();
    void _slot_show_spectrum();
    void _slot_show_controlPRM(bool state);

    void _slot_double_clicked(int id, double d1, double d2);


};

#endif // TABSPECTRUM_H
