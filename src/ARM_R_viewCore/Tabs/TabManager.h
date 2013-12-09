#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include <QString>
#include <QSettings>
#include <QMap>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QMutex>

#include "../Abstracts/IModuleController.h"
#include "../Common/CommonSpectra.h"
#include "../Common/CommonCorrelations.h"

#include "TabsProperty.h"

#include "TabSpectrum.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"

/// ATLANT
#include "TabAtlant.h"

class TabManager: public QTabWidget, public IModuleController, public ITabManager
{
    Q_OBJECT
public:
    TabManager(QWidget *parent = 0);
    ~TabManager();

private:
    unsigned int    _id;
    QString         _name;
    QMap<int, TabsProperty *>   _map_settings;
    QMap<int, TabSpectrum* >    _map_tabs;
    ICommonComponents*          _common_spectra;
    ICommonComponents*          _common_correlations;
    QVBoxLayout*                _layout;
//    Controller*                 _model_controller;

    DBManager*                 _db_manager_spectrum;

    TreeModel*                  _model_spectrum;

    TabSpectrum*                _current_tab_widget;

    QMutex                      _mux;

public:
    virtual int start();
    virtual int stop();
    virtual void show();
    virtual void hide();
    virtual int createSubModules(QString path_to_ini_file);

public:
    virtual QString getStationName(int id);
    virtual void send_data(int pid, IMessage* msg);
    virtual void set_tab(int id);


private:
    int _read_settings(QString path_to_ini_file);
    void _check_status();

private slots:
    void _slot_change_tab(int index);

signals:
    void signalChangeTab(int index);
};

#endif // TABMANAGER_H
