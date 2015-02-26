#ifndef TABSPECTRUM_H
#define TABSPECTRUM_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QTreeView>

#include <QThread>

#include "Interfaces/ISpectrumWidget.h"

#include "SpectrumWidget.h"
#include "SpectrumWidgetController.h"

#include "Correlations/CorrelationWidget.h"

#include "RPC/RpcPrmClient.h"

namespace Ui {
class TabSpectrumWidget;
}

class TabSpectrumWidget: public QWidget
{
	Q_OBJECT

private:
	Ui::TabSpectrumWidget* ui;

	QList<ISpectrumWidget*> m_spectrumWidgetsList;
	QList<ICorrelationWidget*> m_correlationWidgetsList;

	SpectrumWidget* m_spectrumWidget;
	SpectrumWidgetController* m_spectrumWidgetController;

	QPixmap* m_pmRoundRed;
	QPixmap* m_pmRoundGreen;
    QPixmap* m_pmRoundYellow;
	QLabel* m_indicatorLabel;

	RpcPrmClient*	m_rpcPrmClient;
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
    virtual void updateDBListsAreas();

	QTreeView *getTreeView() const;
	void insertCorrelationWidget(ICorrelationWidget *correlationWidget);

	void setRpcPrmClient(RpcPrmClient* client);
	RpcPrmClient* getRpcClient();

signals:
	void setIndicatorStateSignal(int state);
	void spectrumDoubleClickedSignal(int);

private slots:
	void setIndicatorStateSlot(int state);


};

#endif // TABSPECTRUM_H
