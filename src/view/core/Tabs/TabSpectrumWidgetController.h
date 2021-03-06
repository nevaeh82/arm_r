#ifndef TABSPECTRUMWIDGETCONTROLLER_H
#define TABSPECTRUMWIDGETCONTROLLER_H

#include <QObject>
#include <QHostAddress>
#include <QList>
#include <QTimer>

#include "Interfaces/IController.h"
#include "Interfaces/ITabWidget.h"

#include "TabSpectrumWidget.h"

#include "SettingsTree/TreeWidgetDelegate.h"
#include "SettingsTree/TreeModel.h"

#include "SpectrumWidgetDataSource.h"
#include "Correlations/CorrelationWidgetDataSource.h"

#include "Db/DbManager.h"
#include "DBStation/DBStationController.h"

#include "Rpc/RpcPrmClient.h"
#include "Rpc/RpcFlakonClientWrapper.h"

#include "ControlPanel/ControlPanelController.h"

class TabSpectrumWidgetController : public QObject, public IController<TabSpectrumWidget>,
		public ITabWidget, public ITabSpectrum, public IDbChangedListener, public IRpcListener
{
	Q_OBJECT

private:
	TabSpectrumWidget* m_view;
	ISpectrumWidget* m_spectrumWidget;
	SpectrumWidgetDataSource* m_spectrumDataSource;
	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;

	IStation*		    m_station;
	ICorrelationControllersContainer*  m_correlationControllers;
	IDbManager*         m_dbManager;
	ITabManager*        m_tabManager;

	TreeModel*          m_treeModel;
	TreeWidgetDelegate* m_treeDelegate;

	RpcPrmClient*		m_rpcPrmClient;
	RpcFlakonClientWrapper*	m_rpcFlakonClient;

	QLabel*             m_indicatorLabel;

	double              m_threshold;
	double              m_currentCorrelation;

	QString m_rpcHost;
	quint16 m_rpcPort;

	/// connection status
	QTimer	m_timerStatus;

	DBStationController* m_dbStationController;
	ICorrelationListener* m_controlPanelController;
	ControlPanelController* m_controlPanelControllerTrue;
	bool m_isPanoramaEnabled;

	QList<QThread*> m_threadSrcList;

public:
	explicit TabSpectrumWidgetController(
			IStation*, ICorrelationControllersContainer*, ITabManager*, TabSpectrumWidget*,
			const QString rpcHost, const quint16 rpcPort, QObject *parent = 0);
	virtual ~TabSpectrumWidgetController();

	void appendView(TabSpectrumWidget* view);

	void activate();
	void deactivate();

	QWidget* getWidget();
	QLabel* getIndicator();

	virtual ISpectrumWidget *getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual void setDbManager(IDbManager*);
	virtual void setDbStationController(DBStationController* controller);
	virtual void setStationNamesList(const QStringList &stationsList);
	virtual void setRpcFlakonClient(RpcFlakonClientWrapper *client);

	virtual void setIndicator(int state);

	virtual void setShowControlPrm(bool state);
	virtual void setDoubleClicked(int id, double, double);

	virtual void setSelectedArea(const SpectrumSelection &selection);
	virtual void setPointsRpc(QVector<QPointF> points);

	virtual void sendCommand(TypeCommand type, IMessage *msg);

	virtual void enableCorrelation(bool enable = true);

	virtual void setThreshold(double y);
	virtual void checkStatus();
	virtual void recognize();
	virtual void setPanorama(bool state);
	virtual double getCurrentFrequency();

	virtual void onSettingsNodeChanged(const SettingsNode &);
	virtual void onPropertyChanged(const Property &);
	virtual void onCleanSettings();

    /// update white and black lists
    virtual void updateDbStationsLists();

    virtual void updateListsSelections();


	// interface IRpcReceiver
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	//Set control Panel controller pointer
	void setControlPanelController(ICorrelationListener* controller);
	void setControlPanelControllerTrue(ControlPanelController* controller);

private:
	void init();

	void createRPC();
	void createView();
	void createTree();

signals:
	void signalGetPointsFromRPCFlakon(QByteArray points);
	void signalChangeIndicator(int state);
	void signalDoubleClicked(int id, double, double);

	void signalPanoramaState(bool state);

    void signalUpdateDBStationsLists();
    void signalUpdateDBAreas();

private slots:
	void slotGetPointsFromRpc(QByteArray points);
	void slotShowControlPrm(bool state);
	void spectrumDoubleClickedSlot(int id);

	void enablePanoramaSlot(bool isEnabled);

	void slotCheckStatus();

};

#endif // TABSPECTRUMWIDGETCONTROLLER_H
