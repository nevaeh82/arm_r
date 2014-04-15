#ifndef LISTSPROXYMODEL_H
#define LISTSPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include <QDateTime>

class ListsProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	explicit ListsProxyModel(QObject* parent = 0);
	virtual ~ListsProxyModel() {}

	QVariant data(const QModelIndex &index, int role) const;
};

#endif // LISTSPROXYMODEL_H
