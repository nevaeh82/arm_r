#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QMap>;
class QVariant;
class QString;

class IDBManager
{
public:
    virtual ~IDBManager(){};

    virtual int set(QMap<QString, QVariant>* data)         = 0;
    virtual QMap<QString, QVariant>* get(int id, int pid)  = 0;
    virtual QMap<QString, QVariant>* get(int pid)          = 0;
};

#endif // IDBMANAGER_H
