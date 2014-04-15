#ifndef ADDSTATIONDATADIALOGCONTROLLER_H
#define ADDSTATIONDATADIALOGCONTROLLER_H

#include <QObject>

#include "Interfaces/IController.h"

#include "DBStation/IDBStation.h"
#include "DBStation/AddStationDataDialog.h"

class AddStationDataDialogController: public QObject, public IController<AddStationDataDialog>
{
	Q_OBJECT
public:
	AddStationDataDialogController(IDBStation* stationDb, QObject* parent = 0);
	virtual ~AddStationDataDialogController();

	void appendView(AddStationDataDialog* widget);

private:
	IDBStation* m_stationDb;
	AddStationDataDialog* m_view;

private slots:
	void insert(const StationData& data);
};

#endif // ADDSTATIONDATADIALOGCONTROLLER_H
