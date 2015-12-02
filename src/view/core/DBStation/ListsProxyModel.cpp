#include <Logger/Logger.h>

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

	if( sourceModel() && role == Qt::BackgroundColorRole ) {

		QString target = sourceModel()->data( sourceModel()->index(index.row(), 4), Qt::DisplayRole ).toString();
		if( target == QString("Black") ||
			target == StationHelper::translateCategory("Black") ) {
			return QColor(255, 0, 0, 80);
		}
	}

	return QSortFilterProxyModel::data( index, role );
}
