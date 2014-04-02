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

#include "Interfaces/IController.h"

#include "ListsDialog.h"
#include "ListsProxyModel.h"

#include "AddStationDataDialog.h"
#include "AddStationDataDialogController.h"

class ListsDialogController: public QObject, public IController<ListsDialog>
{
	Q_OBJECT
public:
	ListsDialogController(const QSqlDatabase& db, QObject* parent = 0);
	virtual ~ListsDialogController();

	void appendView(ListsDialog* widget);

private:
	QSqlQueryModel*				m_model;
	QSqlDatabase				m_db;
	QTableView*					m_view;
	ListsProxyModel*			m_proxyModel;

private:
	QSqlQuery getAllStationsInfo();
	QSqlQuery getStationsInfoByCategory(int type);
	void adjustTableSize();

private slots:
	void m_slotChooseTypeList(int type);
	void m_slotAdd();
	void m_slotAddClose();
};

#endif // LISTSCONTROLLER_H
