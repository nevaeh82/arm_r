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

	Station*		    m_station;
	ICorrelationControllersContainer*  m_correlationControllers;

	IDbManager*          m_dbManager;

	TreeModel*          m_treeModel;
	TreeWidgetDelegate* m_treeDelegate;

	RPCClient*          m_rpcClient;

	QMap<int, IGraphicWidget *>*     _map_correlation_widget;
	ITabManager*        m_tabManager;

	ControlPRM*         _controlPRM;

	double              _threshold;

	QPixmap*            _pm_round_red;
	QPixmap*            _pm_round_green;
	QLabel*             m_indicatorLabel;

	QString m_rpcHostAddress;
	quint16 m_rpcHostPort;


public:
	explicit TabSpectrumWidgetController(Station* prop, ICorrelationControllersContainer* correlationControllers, IDbManager* db_manager, ITabManager* tab_manager, QObject *parent = 0);
	virtual ~TabSpectrumWidgetController();

	void appendView(TabSpectrumWidget* view);

	void activate();
	void deactivate();

	QWidget* getWidget();
	QLabel* getIndicator();

	virtual ISpectrumWidget *getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual int createRPC();
	virtual int closeRPC();
	virtual int createView();
	virtual int createTree();

	virtual void set_indicator(int state);

	virtual void set_show_controlPRM(bool state);
	virtual void set_double_clicked(int id, double, double);

	virtual void set_selected_area(const SpectrumSelection &selection);
	virtual void set_command(TypeCommand type, IMessage *msg);
	virtual void set_points_rpc(QVector<QPointF> points);

	virtual void set_thershold(double y);
	virtual void check_status();
	virtual void set_panorama(bool state);
	virtual double get_current_frequency();

	virtual void onSettingsNodeChanged(const SettingsNode &);
	virtual void onPropertyChanged(const Property &);
	virtual void onCleanSettings();

signals:
	void signalStartRPC();
	void signalStopRPC();
	void signalFinishRPC();
	void signalGetPointsFromRPCFlakon(QByteArray points);
	void signalChangeIndicator(int state);
	void signalDoubleClicked(int id, double, double);

	void signalPanoramaState(bool state);

private slots:
	void _slot_get_points_from_rpc(QByteArray points);
	void _slot_show_controlPRM(bool state);
	void spectrumDoubleClickedSlot(int id);

	void enablePanoramaSlot(bool isEnabled);

private:
	int init();
	void readSettings(const QString& settingsFile);

};

#endif // TABSPECTRUMWIDGETCONTROLLER_H
