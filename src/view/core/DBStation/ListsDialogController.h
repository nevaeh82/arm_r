#ifndef LISTSCONTROLLER_H
#define LISTSCONTROLLER_H

#include <QObject>

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


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
	void throwWordNfError();

public slots:
    void showAddDialog();
    void showAddDialog(QString station, double center, double bandwidth);

private slots:
	void update();
    void refreshTable();
	void filter(int type);

	void deleteSelectedRecords();
	void slotReport(int type);
};

#endif // LISTSCONTROLLER_H
