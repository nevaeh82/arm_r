#ifndef IDBCACHE_H
#define IDBCACHE_H

#include <QMap>
class QString;
class QVariant;

class IDBCache
{
public:
    virtual ~IDBCache(){};

    virtual QMap<QString, QVariant>* get_list_bla()                  = 0;
    virtual QMap<QString, QVariant>* get_list_bpla()                 = 0;

    virtual QMap<QString, QVariant>* get_property_bla(int pid)       = 0;
    virtual QMap<QString, QVariant>* get_property_bpla(int pid)      = 0;

    virtual void set_bla(QMap<QString, QVariant>* data)              = 0;
    virtual void set_bla_property(QMap<QString, QVariant>* data)     = 0;

    virtual void set_bpla(QMap<QString, QVariant>* data)             = 0;
    virtual void set_bpla_property(QMap<QString, QVariant>* data)    = 0;

};

#endif // IDBCACHE_H
