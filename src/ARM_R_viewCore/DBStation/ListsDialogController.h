#ifndef LISTSCONTROLLER_H
#define LISTSCONTROLLER_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>

#include <QSqlRecord>

#include "DBStation/DBStationController.h"

#include "Interfaces/IController.h"

#include "ListsDialog.h"
#include "ListsProxyModel.h"

#include "IDBStationListener.h"

#include "AddStationDataDialog.h"
#include "AddStationDataDialogController.h"

class ListsDialogController: public QObject, public IController<ListsDialog>, public IDBStationListener
{
	Q_OBJECT
public:
	ListsDialogController(DBStationController* stationDb, QObject* parent = 0);
	virtual ~ListsDialogController();

	void appendView(ListsDialog* widget);

	// intefrace IDBStationListener
	virtual void onStationDataInserted(const StationData&);
	virtual void onStationDataUpdated(const StationData&);

private:
	DBStationController*		m_stationDb;
	QSqlDatabase				m_db;
	QSqlQueryModel*				m_model;
	QTableView*					m_view;
	ListsProxyModel*			m_proxyModel;
	int m_type;

private:
	QSqlQuery getAllStationsInfo();
	QSqlQuery getStationsInfoByCategory(int type);
	void adjustTableSize();
	QSqlQuery deleteFromStationData(int id);

private slots:
	void update(int type = -1);
	void m_slotAdd();
	void m_slotAddClose();
	void m_slotDelete();
};

#endif // LISTSCONTROLLER_H
