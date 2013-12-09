#ifndef ITREEMODEL_H
#define ITREEMODEL_H

#include <QString>
#include <QVariant>

class ITreeModel
{
public:
    virtual ~ITreeModel(){};

    virtual bool addChild(QMap<QString, QVariant> *data, int role)      = 0;

};

#endif // ITREEMODEL_H
