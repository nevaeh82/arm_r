#pragma once

#include <QWidget>

#include "RDSExchange.h"
#include "AnalysisWidgetController.h"
#include "AnalysisResultWidget.h"

namespace Ui {
class AnalysisWidget;
}

class AnalysisWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AnalysisWidget*	ui;
	int m_type;

public:
	AnalysisWidget(int type, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~AnalysisWidget();

	void setAnalysisData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData& adata);

	int getId();

signals:
	void signalAnalysisSelection(int, double, double);
	void signalAddToList(double, double);
	void signalAnalysisContinue(int, bool);
	void analysisReady(int);
};
