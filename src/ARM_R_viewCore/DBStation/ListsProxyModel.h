#ifndef LISTSPROXYMODEL_H
#define LISTSPROXYMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

#include <QDateTime>

class ListsProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit ListsProxyModel(QSqlQueryModel*	model, QObject* parent = 0);

private:
	QSqlQueryModel*		m_model;


public:
	void clear();

protected:
	 bool lessThan ( const QModelIndex & left, const QModelIndex & right ) const;
	 bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

	 bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;

	 bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

};

#endif // LISTSPROXYMODEL_H
