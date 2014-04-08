#ifndef LISTWHITEDIALOGCONTROLLER_H
#define LISTWHITEDIALOGCONTROLLER_H

#include <QObject>
#include "Interfaces/IController.h"
#include "ListWhiteDialog.h"
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QCheckBox>

#include "CheckBoxDelegate.h"

class ListWhiteDialogController: public QObject, public IController<ListWhiteDialog>
{
	Q_OBJECT
public:
	ListWhiteDialogController(const QSqlDatabase& db, QObject* parent = 0);

	virtual ~ListWhiteDialogController();

	void appendView(ListWhiteDialog* widget);

private:
	QSqlQueryModel*				m_model;
	QSqlDatabase				m_db;
	QTableView*					m_view;

private:
	QSqlQuery getAllFromWhiteList();
	void adjustTableSize();

private slots:
	void slotAccept();
	void slotCancel();

};

#endif // LISTWHITEDIALOGCONTROLLER_H
