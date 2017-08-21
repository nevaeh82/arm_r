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

struct stListView
{
    QListWidget* listView;
    QListWidget* workListView;
    QLabel* lbl;

    bool operator ==(const stListView st)
    {
        if(listView == st.listView &&
           workListView == st.workListView &&
           lbl == st.lbl) {
            return true;
        }

        return false;
    }
};

inline bool operator ==(const stListView st1, const stListView st2)
{
    if(st1.listView == st2.listView &&
       st1.workListView == st2.workListView &&
       st1.lbl == st2.lbl) {
        return true;
    }

    return false;
}

class ListsDialog : public QDialog
{
	Q_OBJECT

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

	void getFrequencyAndBandwidthByWhiteAreasIni(QList<StationsFrequencyAndBandwith> &list);
	void getFrequencyAndBandwidthByWhiteAreasDoplerIni(QList<StationsFrequencyAndBandwith> &list);

private:
	Ui::ListsDialog *ui;

	void setFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith> &list);
	void getFrequencyAndBandwidthByWhiteAreasSave(QList<StationsFrequencyAndBandwith> &list);

	QMap<int, stListView> m_listViewMap;

	QList<StationsFrequencyAndBandwith> loadWhiteAreas();
	QList<StationsFrequencyAndBandwith> loadWhiteDoplerAreas();

signals:
	void signalTypeList(int);
	void signalAddClicked();
	void signalDelete();
	void signalReport(int);
	void onClosing();
    void onDetectAdd(int tab, int row);

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
	void saveAreasSettings();

    void slotWorkListItemClick(QListWidgetItem*item);
};

#endif // LISTSDIALOG_H
