#include "TreeItem.h"

TreeItem::TreeItem(QMap<QString, QVariant>* data, TreeItem *parent)
{
    _parentItem = parent;
    _itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    _childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return _childItems.value(row);
}

int TreeItem::childCount() const
{
    return _childItems.count();
}

QMap<QString, QVariant>* TreeItem::data() const
{
    return _itemData;
}

TreeItem *TreeItem::parent()
{
    return _parentItem;
}

int TreeItem::row() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete _childItems.takeAt(position);

    return true;
}

bool TreeItem::removeChild(TreeItem* item)
{
    _childItems.removeAll(item);

    return true;
}

bool TreeItem::updateData(QMap<QString, QVariant>* data)
{
    _itemData = data;
    return true;
}
