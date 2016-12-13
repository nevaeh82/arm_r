#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>

#include "AnalysisResultSection.h"
#include "AnalysisWidgetController.h"

#include "RDSExchange.h"

namespace Ui {
class AnalysisResultWidget;
}

class AnalysisResultWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AnalysisResultWidget*	ui;

	QMap<int, AnalysisResultSection*> m_sectionMap;

	int m_currentIndex;

	void createSections(int count);

	void addRange(int i, double start, double end);
	void addSignalInfo(const int i, const QString &system, const QString &modulation, const QString &type, const QString &info);

private slots:
	void addSignal();
	void slotContinue(bool);

	void onCellClicked(int row, int col);
	void onItemClicked(QTableWidgetItem*);
	void onItemActivated(QModelIndex);

public:
	AnalysisResultWidget(QWidget *parent = 0);
	virtual ~AnalysisResultWidget();

	void setAnalysisData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData& adata);

	void onHide();
	int getCurrentIndex() const {return m_currentIndex;}

signals:
	void signalContinue(int, bool);
	void onAddToList(double, double);
	void analysisSelection(int, double, double);
	void analysisReady(int);
};
