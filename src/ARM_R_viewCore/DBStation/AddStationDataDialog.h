#ifndef ADDSTATIONDATADIALOG_H
#define ADDSTATIONDATADIALOG_H

#include <QDialog>
#include <QPushButton>

#define DEFAULT_INDEX		0
#define DEFAULT_FREQUENCY	300
#define DEFAULT_BANDWIDTH	0
#define DEFAULT_PORT		1024

#include "DBStation/DBFillTables.h"

namespace Ui {
class AddStationDataDialog;
}

class AddStationDataDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddStationDataDialog(QWidget *parent = 0);
	~AddStationDataDialog();

	void fillStation(const QStringList& list);
	void fillCategory(const QStringList& list);
	void fillSignalType(const QStringList& list);

private:
	Ui::AddStationDataDialog *ui;

private slots:
	virtual void accept();
	void clear();

signals:
	void accepted(const StationData& data);

};

#endif // ADDSTATIONDATADIALOG_H
