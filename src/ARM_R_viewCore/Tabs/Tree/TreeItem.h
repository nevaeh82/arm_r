#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

#include <QMap>

//#include "IITemData.h"

class TreeItem
{
public:
    TreeItem(QMap<QString, QVariant>* data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    QMap<QString, QVariant>* data() const;
    int row() const;
    TreeItem *parent();

    bool removeChildren(int position, int count);
    bool removeChild(TreeItem* item);

    bool updateData(QMap<QString, QVariant>* data);

private:
    QList<TreeItem*>            _childItems;
    QMap<QString, QVariant>*    _itemData;
    TreeItem*                   _parentItem;
};

#endif // TREEITEM_H
