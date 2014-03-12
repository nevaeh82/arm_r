#include "ListsProxyModel.h"

ListsProxyModel::ListsProxyModel(QSqlQueryModel* model, QObject* parent) :
	QSortFilterProxyModel(parent)
{
	m_model = model;

	setSourceModel(model);
	setDynamicSortFilter(true);
	setFilterKeyColumn(0);
	setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void ListsProxyModel::clear()
{
	m_model->clear();
}

bool ListsProxyModel::lessThan ( const QModelIndex & left, const QModelIndex & right ) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);

	if(leftData.type() == QVariant::DateTime)
	{
		return leftData.toDateTime() < rightData.toDateTime();
	}

	if(leftData.type() == QVariant::String)
	{
		return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
	}

	if(leftData.type() == QVariant::Int)
	{
		return leftData.toInt() < rightData.toInt();
	}

	if(leftData.type() == QVariant::Double)
	{
		return leftData.toDouble() < rightData.toDouble();
	}
	return false;
}

bool ListsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	return true;
}

bool ListsProxyModel::filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const
{
	return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool ListsProxyModel::hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const
{
	QModelIndex item = sourceModel()->index(source_row,0,source_parent);
	if (!item.isValid()) {
		return false;
	}

	//check if there are children
	int childCount = item.model()->rowCount(item);
	if (childCount == 0)
		return false;

	for (int i = 0; i < childCount; ++i) {
		if (filterAcceptsRowItself(i, item))
			return true;
		//recursive call -> NOTICE that this is depth-first searching, you're probably better off with breadth first search...
		if (hasAcceptedChildren(i, item))
			return true;
	}

	return false;
}
