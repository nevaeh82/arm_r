#ifndef STATIONSDATATABLEMODEL_H
#define STATIONSDATATABLEMODEL_H

#include <QSqlQueryModel>
#include <QObject>

class StationsDataTableModel :/* public QObject, */public QSqlQueryModel
{
	Q_OBJECT

private:
	QSqlDatabase m_database;

public:
	explicit StationsDataTableModel(QSqlDatabase database, QObject *parent = 0);
	virtual ~StationsDataTableModel();

	void update();
	Qt::ItemFlags flags(const QModelIndex & index) const;
	QVariant data(const QModelIndex & index, int role) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role);

signals:
	void updateSignal();
};

#endif // STATIONSDATATABLEMODEL_H
