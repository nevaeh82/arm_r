#ifndef SMSCOMPORTDIALOG_H
#define SMSCOMPORTDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SMSComPortDialog;
}

class SMSComPortDialog : public QWidget
{
	Q_OBJECT

public:
	explicit SMSComPortDialog(QWidget *parent = 0);
	~SMSComPortDialog();

public:
	void fillComPorts(QList<QString> list);
	QStringList getNumbers();
	QString getComPortName();
	void setComPortName(QString name);
	void setListNumbers(QStringList numbers);

	int getDelay() const;

	bool isStanding() const;
	bool isMoving() const;
	bool isUnknown() const;

	void setStanding(bool b);
	void setMoving(bool b);
	void setUnknown(bool b);

private:
	Ui::SMSComPortDialog *ui;
	QString m_currentComPort;
	QStringList m_listNumbers;
	int m_currentComPortIndex;

private:
	void showEvent(QShowEvent *event);

public slots:
	void slotAccept();
	void slotClose();

signals:
	void signalClose();
	void signalAccept();
};

#endif // SMSCOMPORTDIALOG_H
