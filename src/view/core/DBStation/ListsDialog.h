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

	void getFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith> &list);

private:
	Ui::ListsDialog *ui;

	void setFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith> &list);
	void getFrequencyAndBandwidthByWhiteAreasSave(QList<StationsFrequencyAndBandwith> &list);
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

	void slotAddClicked(int frequency1 = 30, int frequency2 = 30, int split = 1);
	void slotRemoveWhiteArea();

	void slotDelete();
	void slotAddInternalWhiteAreaClicked(int frequency1, int frequency2, int split);
};

#endif // LISTSDIALOG_H
