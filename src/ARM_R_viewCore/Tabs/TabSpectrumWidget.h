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

#include "RPC/RPCClient.h"

#include <PwLogger/PwLogger.h>

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
	QLabel* m_indicatorLabel;

	Pw::Logger::ILogger* m_logger;

	RPCClient*	m_rpcClient;
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

	void setRpcClient(RPCClient* rpcClient);
	RPCClient* getRpcClient();

signals:
	void setIndicatorStateSignal(int state);
	void spectrumDoubleClickedSignal(int);

private slots:
	void setIndicatorStateSlot(int state);


};

#endif // TABSPECTRUM_H
