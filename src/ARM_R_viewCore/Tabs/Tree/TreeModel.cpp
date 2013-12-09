#include "TreeItem.h"
#include "TreeModel.h"

TreeModel::TreeModel(const QStringList &headers, QObject *parent)
     : QAbstractItemModel(parent),
       _id(-1)
 {
     QVector<QVariant> rootHeaders;
     foreach (QString header, headers)
     {
         rootHeaders << header;
     }

     _rootItem = new TreeItem(NULL);
 }

 TreeModel::~TreeModel()
 {
     delete _rootItem;
 }

 void TreeModel::set_db(IDBManager *db_manager)
 {
     _db_manager = db_manager;
 }

 int TreeModel::columnCount(const QModelIndex & /* parent */) const
 {
     return 2;//_rootItem->columnCount();
 }

 QVariant TreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
     {
         return QVariant();
     }

     int col = index.column();
     TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
     //TreeItem
     switch(role)
     {
        case Qt::DisplayRole:
        {
            if(!item)
            {
                return QVariant();
            }
            else if(col == 0)
             {
                if(item->data()->contains("name"))
                {
                    return item->data()->value("name");
                }
                return QVariant();
             }
            else if(col == 1)
             {
                if(item->data()->contains("value"))
                {
                    return item->data()->value("value");
                }
                return QVariant();
             }
            else
            {
                return QVariant();
            }
            break;
        }

        case Qt::CheckStateRole:
        {
//             if(!item->isCheckable(col))
//             {
//                 return QVariant();
//             }
            if(item->data()->value("state").toInt() > 1)
            {
                return QVariant();
            }
            if(col == 1)
            {
                 QModelIndex ind = index.sibling(index.row(), 0);                 
                 if(ind.data().isNull())
                 {
                     return static_cast<int>(item->data()->value("state").toBool() ? Qt::Checked : Qt::Unchecked);
                 }
                 else
                 {
                     return QVariant();
                 }
            }
            else
            {
                 if(!index.data().isNull())
                 {
                     return static_cast<int>(item->data()->value("state").toBool() ? Qt::Checked : Qt::Unchecked);
                 }
                 else
                 {
                     return QVariant();
                 }
            }
            break;
        }
        case Qt::SizeHintRole:
        {
            return QSize(150, 18);
        }
         break;
        case Qt::EditRole:
            if(!item)
            {
                return QVariant();
            }
            else if(col == 0)
            {
                if(item->data()->contains("name"))
                {
                    return item->data()->value("name");
                }
                return QVariant();
            }
            else if(col == 1)
            {
                if(item->data()->contains("value"))
                {
                    return item->data()->value("value");
                }
                return QVariant();
            }
            else
            {
                return QVariant();
            }
        break;
         case Qt::BackgroundColorRole:
//              return QColor(Qt::red);
         break;

         case Qt::UserRole+1:
         if(!item)
         {
             return QVariant();
         }
         if(item->data()->contains("value"))
         {
             return item->data()->value("value");
         }
         return QVariant();
         break;
    }
    return QVariant();
 }

 Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

     //if(!index.data(Qt::DisplayRole).isNull())

//         flags |= Qt::ItemIsUserCheckable;

     TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
     ///only 1 column will be editable
     if((index.column() != 0 )&&(item->data()->value("editable").toBool() == true))
     {
        flags |= Qt::ItemIsEditable;
     }
     else
     {
     }

     if(item->data()->value("state") != 3)
     {
            flags |= Qt::ItemIsUserCheckable;
     }

     return flags;
 }

 bool TreeModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
 {
     bool b = true;
     for (QMap<int, QVariant>::ConstIterator it = roles.begin(); it != roles.end(); ++it)
         b = b && setData(index, it.value(), it.key());
     return b;
 }

 QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
     {
         switch(section)
         {
         case 0:
             return tr("�������");
         case 1:
             return tr("��������");
         default:
             break;
         }
     }
     return QVariant();
 }

 QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
 {
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();

     TreeItem *childItem = _rootItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

// bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
// {
//     bool success;

//     beginInsertColumns(parent, position, position + columns - 1);
//     success = _rootItem->insertColumns(position, columns);
//     endInsertColumns();

//     return success;
// }

// bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
// {
//     TreeItem *parentItem = static_cast<TreeItem *>(parent.internalPointer());
//     bool success;

//     beginInsertRows(parent, position, position + rows - 1);
////     success = parentItem->insertChildren(position, rows, _rootItem->columnCount());
//     endInsertRows();

//     return success;
// }

 QModelIndex TreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());

     if(!childItem)
         return QModelIndex();

     TreeItem *parentItem = childItem->parent();

     if (parentItem == _rootItem)
         return QModelIndex();

     return createIndex(parentItem->childCount(), 0, parentItem);
 }

 bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
 {
     bool success;

     beginRemoveColumns(parent, position, position + columns - 1);
//     success = _rootItem->removeColumns(position, columns);
     endRemoveColumns();

//     if (_rootItem->columnCount() == 0)
//         removeRows(0, rowCount());

     return success;
 }

 bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
 {
     TreeItem *parentItem = static_cast<TreeItem *>(parent.internalPointer());
     bool success = true;

     beginRemoveRows(parent, position, position + rows - 1);
     success = parentItem->removeChildren(position, rows);
     endRemoveRows();

     return success;
 }

 int TreeModel::rowCount(const QModelIndex &parent) const
 {
//     if(!parent.isValid())
//     {
//         return _rootItem->childCount();
//     }
     TreeItem *parentItem;
     if (parent.isValid())
     {
         parentItem = static_cast<TreeItem *>(parent.internalPointer());
     }
     else
     {
         parentItem = _rootItem;
     }

     if(parentItem == 0)
     {
         return 0;
     }

     return parentItem->childCount();
 }

 bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                         int role)
 {
     if (role != Qt::EditRole)
         return false;

     TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
     QMap<QString, QVariant>* data = item->data();
     int r = index.row();
     QModelIndex ind = index.sibling(r, 0);
     QVariant val = this->data(ind, Qt::DisplayRole);
     data->insert("value", value);
     _db_manager->set(data);
     bool result = item->updateData(data);
     if (result)
     {
         emit dataChanged(index, index);
     }

     return result;
//     return false;
 }

 bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                               const QVariant &value, int role)
 {
     if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
         return false;

     bool result;// = _rootItem->setData(section, value);

     if (result)
         emit headerDataChanged(orientation, section, section);

     return result;
 }


 void TreeModel::_refreshModel()
 {
     int index = _rootItem->childCount();

     for (int i = 0; i < _rootItem->childCount(); i++)
     {
         index += _rootItem->child(i)->childCount();

     }

     emit dataChanged(createIndex(0,0), createIndex(index,columnCount()-1));
 }


 bool TreeModel::addChild(QMap<QString, QVariant> *data, int role)
 {
     int index = _rootItem->childCount();


     int id = data->value("id").toInt();
     int pid = data->value("pid").toInt();
     if(pid != _id)
     {
         return false;
     }
     TreeItem *item;
     QMap<QString, QVariant>* item_data;
     for (int i = 0; i < _rootItem->childCount(); i++)
     {
         item = _rootItem->child(i);
         item_data = item->data();
         if(item_data->value("id").toInt() == id)
         {
             if(item_data->value("pid").toInt() == pid)
             {
                 item->updateData(data);
                 _refreshModel();
                 return false;
             }
         }
     }
     emit layoutAboutToBeChanged();
     item = new TreeItem(data, _rootItem);
     _rootItem->appendChild(item);
     emit layoutChanged();
     return true;
 }

 void TreeModel::fill_model(int pid)
 {
     _id = pid;
     this->reset();
     delete _rootItem;

     _rootItem = new TreeItem(NULL);
     QMap<QString, QVariant>* data;
     int i = 0;
     do
     {
         data = _db_manager->get(i++, pid);
         if(data->count() == 0)
             break;
         addChild(data, Qt::DisplayRole);
     }while(data->count());
 }
