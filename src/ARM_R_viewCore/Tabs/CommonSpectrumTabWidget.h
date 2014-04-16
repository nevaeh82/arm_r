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

	TreeModel*          m_treeModel;
	TreeWidgetDelegate* m_treeDelegate;

	IDbManager* m_dbManager;

	QPixmap* m_pmRoundRed;
	QPixmap* m_pmRoundGreen;
	QLabel* m_indicatorLabel;

	/// connection status
	QTimer	m_timerStatus;

	RpcFlakonClientWrapper* m_rpcFlakonClient;
	
public:
	explicit CommonSpectrumTabWidget(QWidget *parent = 0);
	~CommonSpectrumTabWidget();

	//void insertSpectrumWidget(GraphicWidget* widget);
	void clearSpectrumWidgetsContainer();

	void setCorrelationComponent(ICorrelationControllersContainer* correlationControllers);

	virtual void activate();
	virtual void deactivate();

    virtual void updateListsSelections();

	QWidget* getWidget();
	ISpectrumWidget* getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual void setDbManager(IDbManager* dbManager);
    virtual void setDbStationController(DBStationController*);
	virtual void setStationNamesList(const QStringList &stationsList);

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

private slots:
	void setIndicatorStateSlot(int state);
	void slotCheckStatus();
};

#endif // COMMONSPECTRUMTABWIDGET_H
