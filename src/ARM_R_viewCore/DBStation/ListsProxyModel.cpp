#include <Logger.h>

#include "StationHelper.h"
#include "ListsProxyModel.h"

ListsProxyModel::ListsProxyModel(QObject* parent) :
	QSortFilterProxyModel(parent)
{
}

QVariant ListsProxyModel::data(const QModelIndex& index, int role) const
{
	if( sourceModel() && index.column() == 4 && role == Qt::DisplayRole ) {
		return StationHelper::translateCategory( sourceModel()->data( index, role ).toString() );
	}

	return QSortFilterProxyModel::data( index, role );
}
