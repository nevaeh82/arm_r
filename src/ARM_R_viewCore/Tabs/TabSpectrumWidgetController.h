#ifndef TABSPECTRUMWIDGETCONTROLLER_H
#define TABSPECTRUMWIDGETCONTROLLER_H

#include <QObject>
#include <QHostAddress>
#include <QList>

#include "Interfaces/IController.h"
#include "TabSpectrumWidget.h"
#include "SettingsTree/TreeWidgetDelegate.h"

#include "SpectrumWidgetDataSource.h"
#include "Correlations/CorrelationWidgetDataSource.h"

class TabSpectrumWidgetController : public QObject, public IController<TabSpectrumWidget>, public ITabWidget, public ITabSpectrum,
		public IDbChangedListener
{
	Q_OBJECT

private:
	TabSpectrumWidget* m_view;
	ISpectrumWidget* m_spectrumWidget;
	SpectrumWidgetDataSource* m_spectrumDataSource;
	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;

	IStation*		    m_station;

	ICorrelationControllersContainer*  m_correlationControllers;

	IDbManager*          m_dbManager;

	TreeModel*          m_treeModel;
	TreeWidgetDelegate* m_treeDelegate;

	RPCClient*          m_rpcClient;

	QMap<int, IGraphicWidget *>*     m_mapCorrelationWidget;
	ITabManager*        m_tabManager;

	ControlPRM*         m_controlPRM;

	double              m_threshold;

	QPixmap*            m_pmRoundRed;
	QPixmap*            m_pmRoundGreen;
	QLabel*             m_indicatorLabel;

	QString m_rpcHostAddress;
	quint16 m_rpcHostPort;


public:
	explicit TabSpectrumWidgetController(IStation*, ICorrelationControllersContainer*, IDbManager*, ITabManager*, QObject *parent = 0);
	virtual ~TabSpectrumWidgetController();

	void appendView(TabSpectrumWidget* view);

	void activate();
	void deactivate();

	QWidget* getWidget();
	QLabel* getIndicator();

	virtual ISpectrumWidget *getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual void setIndicator(int state);

	virtual void setShowControlPrm(bool state);
	virtual void setDoubleClicked(int id, double, double);

	virtual void setSelectedArea(const SpectrumSelection &selection);
	virtual void setPointsRpc(QVector<QPointF> points);

	virtual void sendCommand(TypeCommand type, IMessage *msg);

	virtual void setThreshold(double y);
	virtual void checkStatus();
	virtual void setPanorama(bool state);
	virtual double getCurrentFrequency();

	virtual void onSettingsNodeChanged(const SettingsNode &);
	virtual void onPropertyChanged(const Property &);
	virtual void onCleanSettings();

private:
	int init();
	void readSettings(const QString& settingsFile);

	int createRPC();
	int createView();
	int createTree();

signals:
	void signalGetPointsFromRPCFlakon(QByteArray points);
	void signalChangeIndicator(int state);
	void signalDoubleClicked(int id, double, double);

	void signalPanoramaState(bool state);

private slots:
	void slotGetPointsFromRpc(QByteArray points);
	void slotShowControlPrm(bool state);
	void spectrumDoubleClickedSlot(int id);

	void enablePanoramaSlot(bool isEnabled);
};

#endif // TABSPECTRUMWIDGETCONTROLLER_H
