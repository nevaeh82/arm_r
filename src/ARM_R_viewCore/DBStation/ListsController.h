#ifndef LISTSCONTROLLER_H
#define LISTSCONTROLLER_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


#include <QHeaderView>

#include "Interfaces/IController.h"

#include "ListsForm.h"
#include "ListsProxyModel.h"

#include "ListsAdd.h"
#include "ListsAddController.h"

class ListsController: public QObject, public IController<ListsForm>
{
	Q_OBJECT
public:
	ListsController(const QSqlDatabase& db, QObject* parent = 0);
	virtual ~ListsController();

	void appendView(ListsForm* widget);

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
};

#endif // LISTSCONTROLLER_H
