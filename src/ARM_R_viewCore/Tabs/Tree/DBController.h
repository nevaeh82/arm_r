#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QtSql>
#include <QString>
#include <QStringList>

#include <QMessageBox>

#include <QMap>
#include <QVariant>

#include <PwLogger/PwLogger.h>

#include "IDBController.h"

class DBController : public QObject, public IDBController
{
    Q_OBJECT
public:
    DBController();
    ~DBController();

public:
    virtual QMap<QString, QVariant>* get_data(int id, int pid);
    virtual QMap<QString, QVariant>* get_data(int pid);

public slots:
    virtual void set_data(QMap<QString, QVariant>* data);

private slots:
    void _slot_set(QMap<QString, QVariant>* data);

private:
    QSqlDatabase _sdb;
    QString     _db_name;
    static Pw::Logger::ILogger*    _logger;

private:
	 bool readSettings(const QString &settingsFile);
     int _set_db();
     void _set_table(QString name);

signals:
     void signalFinished();
     void signalSet(QMap<QString, QVariant> *);

};

#endif // DBCONTROLLER_H
