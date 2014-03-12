#ifndef LISTSADDCONTROLLER_H
#define LISTSADDCONTROLLER_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDateTime>

#include "Interfaces/IController.h"

#include "ListsAdd.h"

class ListsAddController: public QObject, public IController<ListsAdd>
{
	Q_OBJECT
public:
	ListsAddController(const QSqlDatabase& db, QObject* parent = 0);
	virtual ~ListsAddController();

	void appendView(ListsAdd* widget);

private:
	QSqlDatabase				m_db;
	ListsAdd*					m_view;

private:
	QStringList getNames(QString table);

private slots:
	void slotAccept(const QStringList& list);
};

#endif // LISTSADDCONTROLLER_H
