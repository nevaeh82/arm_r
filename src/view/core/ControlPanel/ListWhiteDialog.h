#ifndef LISTWHITEDIALOG_H
#define LISTWHITEDIALOG_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class ListWhiteDialog;
}

class ListWhiteDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ListWhiteDialog(QWidget *parent = 0);
	~ListWhiteDialog();

	QTableView* getTableView();

private:
	Ui::ListWhiteDialog *ui;

signals:
	void signalAccept();
	void SignalCancel();
};

#endif // LISTWHITEDIALOG_H
