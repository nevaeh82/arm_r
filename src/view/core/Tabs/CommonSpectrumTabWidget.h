#ifndef COMMONSPECTRUMTABWIDGET_H
#define COMMONSPECTRUMTABWIDGET_H

#include <QWidget>

#include "SpectrumWidget.h"

#include "Interfaces/ICorrelationControllersContainer.h"
#include "Interfaces/ITabWidget.h"

#include "SettingsTree/TreeModel.h"
#include "SettingsTree/TreeWidgetDelegate.h"

#include "Rpc/RpcFlakonClientWrapper.h"

namespace Ui {
class CommonSpectrumTabWidget;
}

class CommonSpectrumTabWidget : public QWidget, public ITabWidget, public IRpcListener
{
	Q_OBJECT

private:
	Ui::CommonSpectrumTabWidget *ui;

	QList<ISpectrumWidget*> m_widgetList;

	ICorrelationControllersContainer* m_correlationControllers;
	IAnalysisWidget* m_analysis;

	TreeModel*          m_treeModel;
	TreeWidgetDelegate* m_treeDelegate;

	IDbManager* m_dbManager;

	QPixmap* m_pmRoundRed;
	QPixmap* m_pmRoundGreen;
	QLabel* m_indicatorLabel;

	RpcFlakonClientWrapper* m_rpcFlakonClient;

    ControlPanelWidget* m_cpView;
	
public:
	explicit CommonSpectrumTabWidget(QWidget *parent = 0);
	~CommonSpectrumTabWidget();

	void clearSpectrumWidgetsContainer();

	void setCorrelationComponent(ICorrelationControllersContainer* correlationControllers);
	void setAnalysisComponent(IAnalysisWidget *analysis);

	virtual void activate();
	virtual void deactivate();

    virtual void updateListsSelections();

	QWidget* getWidget();
	ISpectrumWidget* getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual void setDbManager(IDbManager* dbManager);
	virtual void resetDbManager();

    virtual void setDbStationController(DBStationController*);
	virtual void setStationNamesList(const QStringList &stationsList);

    void setControlPanelWidget(ControlPanelWidget* wgt);

	QLabel* getIndicator();

	void setIndicatorState(int state);

	virtual void onMethodCalled(const QString &method, const QVariant &argument);

	virtual int createRPC();
	virtual int closeRPC();
	virtual int createView(QWidget *);
	virtual int createTree();

	void setFlakonRpcClient(RpcFlakonClientWrapper *rpcClient);

private:
	void setIndicator(int state);

signals:
	void setIndicatorStateSignal(int state);
    void signalFreqChanged(int);

private slots:
	void setIndicatorStateSlot(int state);
};

#endif // COMMONSPECTRUMTABWIDGET_H
