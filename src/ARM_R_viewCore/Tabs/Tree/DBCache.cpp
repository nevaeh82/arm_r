#include "DBCache.h"

#include <QDebug>

DBCache::DBCache()
{
}

DBCache::~DBCache()
{
}

int DBCache::set(QMap<QString, QVariant>* data)
{
    QString ids = data->value("pid").toString();
    ids.append(", ");
    ids.append(data->value("id").toString());

    _map_db.insert(ids, data);

    return 0;
}

QMap<QString, QVariant>* DBCache::get(int pid, int id)
{
    QString ids = QString::number(pid);
    ids.append(", ");
    ids.append(QString::number(id));

    if(!_map_db.contains(ids))
    {
        return new QMap<QString, QVariant>;
    }
    return _map_db.value(ids);
}
