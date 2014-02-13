#ifndef TABSPECTRUMWIDGETCONTROLLER_H
#define TABSPECTRUMWIDGETCONTROLLER_H

#include <QObject>
#include <QHostAddress>

#include "Interfaces/IController.h"
#include "TabSpectrumWidget.h"

class TabSpectrumWidgetController : public QObject, public IController<TabSpectrumWidget>, public ITabWidget, public ITabSpectrum
{
	Q_OBJECT

private:
	TabSpectrumWidget* m_view;

	int                 _id;
	QString             _name;
	TabsProperty*       _tab_property;
	ICommonComponents*  _common_components;
	ICommonComponents*  _common_correlations;
	GraphicData*        _spectrumData;

	IDBManager*          _db_manager;

	TreeModel*          _model;

	RPCClient*          _rpc_client1;

	QMap<int, IGraphicWidget *>*     _map_correlation_widget;
	ITabManager*        _tab_manager;

	ControlPRM*         _controlPRM;

	double              _threshold;

	QPixmap*            _pm_round_red;
	QPixmap*            _pm_round_green;
	QLabel*             m_indicatorLabel;


public:
	explicit TabSpectrumWidgetController(TabsProperty* prop, ICommonComponents* common_components, ICommonComponents *common_correlations, TreeModel* model, DBManager* db_manager, ITabManager* tab_manager, QObject *parent = 0);
	virtual ~TabSpectrumWidgetController();

	void appendView(TabSpectrumWidget* view);

	void activate();
	void deactivate();

	QWidget* getWidget();
	QLabel* getIndicator();

	virtual SpectrumWidget* getSpectrumWidget();

	virtual void insertSpectrumWidget(SpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual int createRPC();
	virtual int closeRPC();
	virtual int createView();
	virtual int createTree();

	virtual void set_indicator(int state);

	virtual void set_show_controlPRM(bool state);
	virtual void set_double_clicked(int id, double, double);

	virtual void set_selected_area(QMap<int, QVariant> data);
	virtual void set_command(TypeCommand type, IMessage *msg);
	virtual void set_points_rpc(QVector<QPointF> points);

	virtual void set_thershold(double y);

	virtual void check_status();

	virtual void set_panorama(bool state);

	virtual double get_current_frequency();

	TabsProperty* get_tab_property();

signals:
	void signalStartRPC();
	void signalStopRPC();
	void signalFinishRPC();
	void signalGetPointsFromRPCFlakon(QVector<QPointF> points);
	void signalChangeIndicator(int state);
	void signalDoubleClicked(int id, double, double);

	void signalPanoramaState(bool state);

private slots:
	void _slot_get_points_from_rpc(QVector<QPointF> points);
	void _slot_show_controlPRM(bool state);
	void spectrumDoubleClickedSlot(int id);

	void _slotPanoramaState(bool state);

private:
	int init();

};

#endif // TABSPECTRUMWIDGETCONTROLLER_H
