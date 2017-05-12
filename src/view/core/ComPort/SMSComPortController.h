#pragma once

#include <QObject>
#include "SMSComPortDialog.h"
#include "Interfaces/ISMSComPortController.h"
#include "ComPort/ComPort.h"
#include "ComPort/SMSComPortDialog.h"

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

private:
	SMSComPortDialog* m_view;
	ComPort* m_comport;
	SMSComPortDialog* m_smsDialog;


signals:

private slots:
	void slotCloseUi();
	void slotUpdateFromUi();

};
