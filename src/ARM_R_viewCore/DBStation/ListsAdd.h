#ifndef LISTSADD_H
#define LISTSADD_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class ListsAdd;
}

class ListsAdd : public QDialog
{
	Q_OBJECT

public:
	explicit ListsAdd(QWidget *parent = 0);
	~ListsAdd();

	void fillStation(const QStringList& list);
	void fillCategory(const QStringList& list);
	void fillSignalType(const QStringList& list);

private:
	Ui::ListsAdd *ui;

private slots:
	void slotAccept();
	void slotClear();
	void slotClose();

signals:
	void signalAccept(const QStringList& list);

};

#endif // LISTSADDADD_H
