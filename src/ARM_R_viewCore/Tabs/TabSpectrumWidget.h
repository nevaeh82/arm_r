#ifndef TABSPECTRUM_H
#define TABSPECTRUM_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QStackedWidget>

#include <QThread>



#include "Abstracts/ITabWidget.h"
#include "Abstracts/ICommonComponents.h"

#include "TabsProperty.h"
#include "SpectrumWidget.h"
#include "GraphicData.h"

#include "Correlations/CorrelationWidget.h"

#include "Tree/TreeView.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"
#include "ITabSpectrum.h"

#include "RPC/RPCClient.h"

#include "Controls/ControlPRM.h"

#include "Controls/ControlPanelWidgets.h"
#include "Controls/ButtonShowPanel.h"

namespace Ui {
class TabSpectrumWidget;
}

class TabSpectrumWidget: public QWidget
{
	Q_OBJECT

private:
	Ui::TabSpectrumWidget*     ui;

	QList<SpectrumWidget*> m_spectrumWidgetsList;
	QList<CorrelationWidget*> m_correlationWidgetsList;

	SpectrumWidget* m_spectrumWidget;

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

	virtual SpectrumWidget* getSpectrumWidget();

	virtual void insertSpectrumWidget(SpectrumWidget *spectrumWidget);
	virtual void setIndicatorState(int state);

	QTreeView *getTreeView() const;
	void insertCorrelationWidget(CorrelationWidget* correlationWidget);

signals:
	void setIndicatorStateSignal(int state);
	void spectrumDoubleClickedSignal(int);

private slots:
	void setIndicatorStateSlot(int state);


};

#endif // TABSPECTRUM_H
