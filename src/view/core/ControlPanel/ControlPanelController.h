#pragma once

#include <QObject>
#include <QSettings>
#include <QTextCodec>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

#include "QtSerialPort/QSerialPort"

#include "Interfaces/IController.h"
#include "Interfaces/IDbManager.h"
#include "ControlPanelWidget.h"
#include "Interfaces/IControlPanelController.h"
#include "Interfaces/IControlPanelListener.h"

#include "QxtNetwork/qxtsmtp.h"
#include "QxtNetwork/qxtmailmessage.h"

#include "DBStation/IDBStation.h"

#include "Tabs/Rpc/RpcFlakonClientWrapper.h"
#include "Station.h"

#include "ListWhiteDialog.h"
#include "ListWhiteDialogController.h"
#include "DBStation/ListsDialog.h"

#include "smtp1/smtpclientthread.h"

#include <QSettings>
#include <QCoreApplication>

#define INTERVAL 20
#define MINIMUM_FREQ 20
#define MAXIMUM_FREQ 7000

#include "Interfaces/ICorrelationListener.h"

class ControlPanelController : public QObject, public IControlPanelController, public ICorrelationListener, public IRpcListener
{
	Q_OBJECT
public:
	struct solverResult
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

		solverResult::solverResult()
		{
			dateTime = -1;
			quality = -1;
			state = -1;
			freq = 0;
		}
	};

private:
	int m_tabId;
	ControlPanelWidget* m_view;

	IDbManager* m_dbManager;
	IDBStation* m_dbStation;

	QTimer m_timer;
	QTimer m_timerCheck;

	int m_startFreq;
	int m_finishFreq;
	double m_currentFreq;
	double m_currentFreqFromPlot;

	int m_timerInterval;
    int m_timerCheckInterval;
    int m_timerCheckIntervalDopler;
	int m_timerCheckIntervalDetected;

	bool m_isFollowMode;
    bool m_isDopplerSearch;

	QList<StationsFrequencyAndBandwith> m_listOfFreqs;
	QList<StationsFrequencyAndBandwith>::Iterator m_itCheckMode;

	QList<StationsFrequencyAndBandwith> m_listOfFreqsBlack;
	QList<StationsFrequencyAndBandwith>::Iterator m_itScanMode;

	QList<int> m_IdDetetcted;
	int m_itDetected;
	bool m_workCheckList;

	//solverResult m_solverResult;
	QList<solverResult> m_solverResultList;

	RpcFlakonClientWrapper* m_rpcFlakonClient;
	QMap<int, Station *> m_stationsMap;

	Station* m_mainStation;

	void init();

	bool slotIncCheckMode();
	void checkSolverResult();
    void checkDopplerResult();

	LocationSetupWidgetController* m_setupController;

	ListsDialog* m_listsDialog;

	bool checkSolverResult(int freq);
	int findResultInList(solverResult res);

    QMultiMap<QString, double> m_dopplerMap;

    QString m_title;
public:
    explicit ControlPanelController(int tabId, QString title, QObject *parent = 0);
	virtual ~ControlPanelController();

	void appendView(ControlPanelWidget* view);

	void setDbManager(IDbManager* dbManager);
	void setDbStationController(IDBStation* dbStationController);
	void setRpcFlakonClient(RpcFlakonClientWrapper* rpcFlakonClient);
	void setMapStations(QMap<int, Station *> stationsMap);
	void setResponseFreq(quint32 freq);

	//From correlation Listener
	void onCorrelationStateChanged(const bool isEnabled);

	void setCorrelationFrequencyValue(double value);
	void setCentralFreqValue(double freq);

	virtual void onMethodCalled(const QString &method, const QVariant &argument);

	void setLocationSetupController(LocationSetupWidgetController* controller);

	void stopLocationRequest();
	void setSpectrumReveive(bool);

	bool isPanorama(double& start, double& end);

	void setListDialog(ListsDialog* dlg);

	bool sleepMode() const;
	void setSleepMode(bool);

	void setManualMode();

    QList<solverResult> getSolverResultList() const {return m_solverResultList;}

    void setTitle(QString title) {m_title = title;}


signals:
	void signalSetComonFreq(double value);
	void setCorrelationStatus(QString correlationStatus);
	void setCorrelationStatusActive(bool isActive);

	void signalSetCentralFreq(double freq);

	void onStateChanged();

	void signalSystemMerge(bool);

    void skoChanged(float);

	void signalReceiveSpectrums(bool);
    void signalReadyToScreenShot();

    void signalDopplerDetect(QString);

public slots:

	void changeFrequency(double value);
	void updatePanorama(const bool enable, const double& start, const double& end);

	void setSolverConnectState(bool);
	void onSetSleepMode(bool);
    void slotDopplerStatus(QString name, double doppler);


private slots:
	void onPanoramaStateChangedSlot(bool isEnabled, int, int);
	void onAutoSearchStateChangedSlot(bool isEnabled);
	void onCommonFrequencyChangedSlot(double value);

	void onBandWidthChangedSlot(int start, int end);

	void slotManualMode();
	void slotScanMode(int start, int finish);
	void slotCheckMode();
	void slotViewMode();
	void slotViewAreaMode();
    void slotViewAreaDopplerMode();

	void slotChangeFreq();
	void slotCheckModeSetFreq();

	void slotDown1MHz();
	void slotDown10MHz();
	void slotDown100MHz();
	void slotUp1MHz();
	void slotUp10MHz();
	void slotUp100MHz();

	void changeCorrelationStatus(QString correlationStatus);
	void changeCorrelationStatusActive(bool isActive);

	void setLocationSettings();
	void setCentralFreqValueInternal(double freq);

	void slotSolverResult(QByteArray data);
};

