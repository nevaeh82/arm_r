#ifndef LISTSDIALOG_H
#define LISTSDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QListWidget>
#include <Logger/Logger.h>
#include <QLabel>

#include "DBStation/DBFillTables.h"

namespace Ui {
class ListsDialog;
}

class ListsDialog : public QDialog
{
	Q_OBJECT

	struct stListView
	{
		QListWidget* listView;
		QListWidget* workListView;
		QLabel* lbl;
	};

public:
	explicit ListsDialog(QWidget *parent = 0);
	~ListsDialog();

	QTableView* getTableView();

	void addDetectFreq(double num, int index, int id);
	void rmDetectFreq(double num, int id);
	void clearDetectFreq(int id);

	void setWorkList(QList<StationsFrequencyAndBandwith> list, int id, QString title);
	void clearWorkList(int id);

	void setCheckPointer(int ind, int id);
	void setDetectPointer(int ind, int id);

	void getFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith> &list);

private:
	Ui::ListsDialog *ui;

	void setFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith> &list);
	void getFrequencyAndBandwidthByWhiteAreasSave(QList<StationsFrequencyAndBandwith> &list);

	QMap<int, stListView> m_listViewMap;

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

	void slotAreasCellClicked(int row, int col);
};

#endif // LISTSDIALOG_H
