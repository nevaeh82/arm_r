#ifndef LISTSCONTROLLER_H
#define LISTSCONTROLLER_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "Interfaces/IController.h"

#include "ListsForm.h"

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

private:
	QSqlQuery getAllStationsInfo();

private slots:
	void m_slotChooseTypeList(int type);

};

#endif // LISTSCONTROLLER_H
