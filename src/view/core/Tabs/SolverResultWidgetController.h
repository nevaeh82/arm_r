#ifndef SOLVERRESULTWIDGETCONTROLLER_H
#define SOLVERRESULTWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QThread>
#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "smtp1/smtpclientthread.h"
#include "EmailSettings.h"

#include "SolverResultWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

#include "SolverPacket1.pb.h"

class SolverResultWidgetController : public QObject, public IController<SolverResultWidget>,
									 public IRpcListener
{
	Q_OBJECT


	struct solverResultStruct
	{
		qint64 dateTime;
		int quality;
		int state;
		double freq;
		double lon;
		double lat;

		void clear(){
			dateTime = -1;
			quality = -1;
			state = -1;
			freq = 0;
			lon = 0;
			lat = 0;
		}

		solverResultStruct::solverResultStruct()
		{
			dateTime = -1;
			quality = -1;
			state = -1;
			freq = 0;
		}
	};

public:
	explicit SolverResultWidgetController(QObject *parent = 0);
	virtual ~SolverResultWidgetController();


private:
	SolverResultWidget* m_view;

	SmtpClientThread* m_smtpThread;
	QThread* m_smtpQThread;

	EmailSettings* m_emailSettings;

	QTime m_elapsedMail;

public:
	void appendView(SolverResultWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
	SolverResultWidget* getView();

public slots:
	void slotShowWidget();


signals:
	void onMethodCalledSignal(QString, QVariant);

	void solverResult(QByteArray);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);
	void onEmailSettings();

	void slotEmailUpdate();
private:
	QString getSolverResultToString(const SolveResult &result);
	void addResultToLog(const QByteArray &data);
	void addResultToLog1(const QByteArray &data);

	void parseForMail(const QByteArray& inData);

	bool sendMail(const solverResultStruct &res);
};

#endif // SOLVERRESULTWIDGETCONTROLLER_H
