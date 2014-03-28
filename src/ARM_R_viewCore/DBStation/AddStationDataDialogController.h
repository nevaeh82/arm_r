#ifndef ADDSTATIONDATADIALOGCONTROLLER_H
#define ADDSTATIONDATADIALOGCONTROLLER_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "Interfaces/IController.h"

#include "AddStationDataDialog.h"

class AddStationDataDialogController: public QObject, public IController<AddStationDataDialog>
{
	Q_OBJECT
public:
	AddStationDataDialogController(const QSqlDatabase& db, QObject* parent = 0);
	virtual ~AddStationDataDialogController();

	void appendView(AddStationDataDialog* widget);

private:
	QSqlDatabase				m_db;
	AddStationDataDialog*					m_view;

private:
	QStringList getNames(QString table);

private slots:
	void slotAccept(const QStringList& list);
};

#endif // ADDSTATIONDATADIALOGCONTROLLER_H
