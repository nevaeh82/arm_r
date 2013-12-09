#ifndef IDBCACHE_H
#define IDBCACHE_H

#include <QMap>
class QString;
class QVariant;

class IDBCache
{
public:
    virtual ~IDBCache(){};

    virtual QMap<QString, QVariant>* get(int id, int pid)               = 0;
    virtual QMap<QString, QVariant>* get(int pid)                       = 0;
    virtual void set(QMap<QString, QVariant>* data)                     = 0;

};

#endif // IDBCACHE_H
