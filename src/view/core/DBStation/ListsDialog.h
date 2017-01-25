#ifndef LISTSDIALOG_H
#define LISTSDIALOG_H

#include <QDialog>
#include <QTableView>
#include <Logger/Logger.h>

#include "DBStation/DBFillTables.h"

namespace Ui {
class ListsDialog;
}

class ListsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ListsDialog(QWidget *parent = 0);
	~ListsDialog();

	QTableView* getTableView();

	void addDetectFreq(double num, int index);
	void rmDetectFreq(double num);
	void clearDetectFreq();

	void setWorkList(QList<StationsFrequencyAndBandwith> list);
	void clearWorkList();

	void setCheckPointer(int ind);
	void setDetectPointer(int ind);

private:
	Ui::ListsDialog *ui;

signals:
	void signalTypeList(int);
	void signalAddClicked();
	void signalDelete();
    void signalReport(int);
	void onClosing();

public slots:
	void slotShow();
    void slotReportType();
	void slotTypeList(int ind);

//	void slotClose();
private slots:
    void on_tvLists_doubleClicked(const QModelIndex &index);
};

#endif // LISTSDIALOG_H
