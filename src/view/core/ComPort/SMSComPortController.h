#pragma once

#include <QObject>
#include <QTime>

#include "SMSComPortDialog.h"
#include "Interfaces/ISMSComPortController.h"
#include "ComPort/ComPort.h"
#include "ComPort/SMSComPortDialog.h"


typedef struct SMSMessage
{
	int currentIndexNumber;
	QString msg;
}SMSMessage;

class SMSComPortController : public QObject, public ISMSComPortController
{
	Q_OBJECT
public:
	explicit SMSComPortController(QObject *parent = 0);
	~SMSComPortController();

public:
	void init();
	void appendView(SMSComPortDialog* view);
	void showDialog();
	QStringList getNumbers();
	void sendMessage(QString msg);
	void onStart();

    QTime m_initTime;

private:
	SMSComPortDialog* m_view;
	ComPort* m_comport;
	SMSComPortDialog* m_smsDialog;
	QStringList m_listNumbers;
	int m_listNumbersCurrentIndex;
	QList<SMSMessage> m_listMessages;
	QString m_comPortName;

private:
	void readSettings();
	void saveSettings();


signals:
	void signalSendNewMessage();

private slots:
	void slotCloseUi();
	void slotUpdateFromUi();
	void slotSendSMS();
    void slotSendSMByTimer();

};
