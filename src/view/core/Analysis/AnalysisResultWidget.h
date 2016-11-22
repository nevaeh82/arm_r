#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

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

    void createSections(int count);

    void addRange(int i, double start, double end);
    void addSignalInfo(const int i, const QString &system, const QString &modulation, const QString &type, const QString &info);

private slots:
    void addSignal();

    void slotContinue(bool);
public:
    AnalysisResultWidget(QWidget *parent = 0);
    virtual ~AnalysisResultWidget();

	void setAnalysisData(RdsProtobuf::ServerMessage_OneShotData_AnalysisData_AnalysisOutput msg);

signals:
    void signalContinue(bool);
    void onAddToList(double, double);
};
