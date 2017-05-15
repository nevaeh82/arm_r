#ifndef SMSCOMPORTDIALOG_H
#define SMSCOMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class SMSComPortDialog;
}

class SMSComPortDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SMSComPortDialog(QWidget *parent = 0);
	~SMSComPortDialog();

public:
	void fillComPorts(QList<QString> list);
	QStringList getNumbers();
	QString getComPortName();

private:
	Ui::SMSComPortDialog *ui;
	QString m_currentComPort;
	QStringList m_listNumbers;
	int m_currentComPortIndex;

private slots:
	void slotAccept();
	void slotClose();

signals:
	void signalClose();
	void signalAccept();
};

#endif // SMSCOMPORTDIALOG_H
