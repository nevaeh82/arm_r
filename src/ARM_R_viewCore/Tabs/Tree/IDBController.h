#ifndef IDBCONTROLLER_H
#define IDBCONTROLLER_H

#include <QMap>
class QString;
class QVariant;

class IDBController
{
public:
    virtual ~IDBController(){};

    virtual QMap<QString, QVariant>* get_data(int id, int pid)               = 0;
    virtual QMap<QString, QVariant>* get_data(int pid)                       = 0;
    virtual void set_data(QMap<QString, QVariant>* data)                     = 0;


//    virtual QList<QMap<QString, QVariant> > get_all_data_table(int pid)     = 0;

};

#endif // IDBCONTROLLER_H
