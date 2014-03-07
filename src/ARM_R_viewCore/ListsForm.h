#ifndef LISTSFORM_H
#define LISTSFORM_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class ListsForm;
}

class ListsForm : public QDialog
{
	Q_OBJECT

public:
	explicit ListsForm(QWidget *parent = 0);
	~ListsForm();

	QTableView* getTableView();

private:
	Ui::ListsForm *ui;

signals:
	void signalTypeList(int);

public slots:
	void slotShow();
};

#endif // LISTSFORM_H
