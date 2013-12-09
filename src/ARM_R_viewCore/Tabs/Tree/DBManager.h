#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>


#include "IDBManager.h"
#include "IDBController.h"
#include "DBController.h"

#include "ITreeModel.h"

#include "DBCache.h"

#include "../../Common/CommandMessage.h"
#include "../ITabManager.h"

class DBManager : public QObject, public IDBManager
{
    Q_OBJECT
public:
    DBManager(ITabManager* tab_manager);
    ~DBManager();

public:
    void set_model(ITreeModel *model);

public:
    virtual int set(QMap<QString, QVariant>* data);
    virtual QMap<QString, QVariant>*  get(int id, int pid);
    virtual QMap<QString, QVariant>*  get(int pid);

private:
    ITabManager*        _tab_manager;
    DBController*       _db_controller;
    ITreeModel*         _model;

    DBCache             _db_cache;

signals:
    void signalSetDataDBController(QMap<QString, QVariant> *);
};

#endif // DBMANAGER_H
