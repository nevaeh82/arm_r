#ifndef DBCACHE_H
#define DBCACHE_H

#include <QMap>
#include <QString>
#include <QVariant>

typedef struct DBId
{
    int pid;
    int id;
}DBId;

class DBCache
{
public:
    DBCache();
    ~DBCache();

public:
    int set(QMap<QString, QVariant>* data);
    QMap<QString, QVariant>* get(int pid, int id);

private:
    QMap<QString, QMap<QString, QVariant>*>    _map_db;
};

#endif // DBCACHE_H
