#ifndef STATIONSDATATABLEMODEL_H
#define STATIONSDATATABLEMODEL_H

#include <QSqlQueryModel>

class StationsDataTableModel : public QSqlQueryModel
{
	Q_OBJECT

private:
	QSqlDatabase m_database;

public:
	explicit StationsDataTableModel(QSqlDatabase database, QObject *parent = 0);
	virtual ~StationsDataTableModel() {}

	void update();

signals:

public slots:

};

#endif // STATIONSDATATABLEMODEL_H
