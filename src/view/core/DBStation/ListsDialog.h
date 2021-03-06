#ifndef LISTSDIALOG_H
#define LISTSDIALOG_H

#include <QDialog>
#include <QTableView>

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
//	void slotClose();
};

#endif // LISTSDIALOG_H
