#ifndef TABSPECTRUM_H
#define TABSPECTRUM_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QTreeView>

#include <QThread>

#include "Abstracts/ITabWidget.h"
#include "Interfaces/ICorrelationControllersContainer.h"

#include "Interfaces/ISpectrumWidget.h"

#include "TabsProperty.h"
#include "SpectrumWidget.h"
#include "SpectrumWidgetController.h"

#include "Correlations/CorrelationWidget.h"

#include "Db/DbManager.h"
#include "SettingsTree/TreeModel.h"

#include "Interfaces/ITabManager.h"
#include "ITabSpectrum.h"

#include "RPC/RPCClient.h"

#include "Controls/ControlPRM.h"


namespace Ui {
class TabSpectrumWidget;
}

class TabSpectrumWidget: public QWidget
{
	Q_OBJECT

private:
	Ui::TabSpectrumWidget*     ui;

	QList<ISpectrumWidget*> m_spectrumWidgetsList;
	QList<ICorrelationWidget*> m_correlationWidgetsList;

	SpectrumWidget* m_spectrumWidget;
	SpectrumWidgetController* m_spectrumWidgetController;

	QPixmap*            _pm_round_red;
	QPixmap*            _pm_round_green;
	QLabel*             m_indicatorLabel;

public:
	TabSpectrumWidget(QWidget* parent = NULL);
	virtual ~TabSpectrumWidget();

public:
	void activate();
	void deactivate();

	QWidget* getWidget();
	QLabel* getIndicator();

	virtual ISpectrumWidget *getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual void setIndicatorState(int state);

	QTreeView *getTreeView() const;
	void insertCorrelationWidget(ICorrelationWidget *correlationWidget);

signals:
	void setIndicatorStateSignal(int state);
	void spectrumDoubleClickedSignal(int);

private slots:
	void setIndicatorStateSlot(int state);


};

#endif // TABSPECTRUM_H
