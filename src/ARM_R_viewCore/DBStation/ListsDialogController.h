#ifndef LISTSCONTROLLER_H
#define LISTSCONTROLLER_H

#include <QObject>

#include "DBStation/DBStationController.h"
#include "Interfaces/IController.h"

#include "ListsDialog.h"
#include "ListsProxyModel.h"
#include "StationsDataTableModel.h"

#include "IDBStationListener.h"

#include "AddStationDataDialog.h"
#include "AddStationDataDialogController.h"

class ListsDialogController: public QObject, public IController<ListsDialog>, public IDBStationListener
{
	Q_OBJECT

public:
	ListsDialogController(IDBStation* stationDb, QObject* parent = 0);
	virtual ~ListsDialogController();

	void appendView(ListsDialog* widget);

	// intefrace IDBStationListener
	virtual inline void onStationDataInserted(const StationData&) { update(); }
	virtual inline void onStationDataUpdated(const StationData&) { update(); }

private:
	IDBStation*					m_stationDb;
	StationsDataTableModel*		m_model;
	QTableView*					m_view;
	QSortFilterProxyModel*		m_proxyModel;

private:
	void adjustTableSize();

private slots:
	void update();
	void filter(int type);

	void showAddDialog();
	void deleteSelectedRecords();
};

#endif // LISTSCONTROLLER_H
