#ifndef ADDSTATIONDATADIALOG_H
#define ADDSTATIONDATADIALOG_H

#include <QDialog>
#include <QPushButton>

#define DEFAULT_INDEX		0
#define DEFAULT_FREQUENCY	300
#define DEFAULT_BANDWIDTH	0
#define DEFAULT_PORT		1024

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
	void slotAccept();
	void slotClear();
	void slotClose();

signals:
	void signalAccept(const QStringList& list);
	void signalUpdateList();

};

#endif // ADDSTATIONDATADIALOG_H
