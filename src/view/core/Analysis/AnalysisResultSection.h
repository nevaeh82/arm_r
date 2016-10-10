#pragma once

#include <QWidget>

#include "AnalysisWidgetController.h"

namespace Ui {
class AnalysisResultSection;
}

class AnalysisResultSection : public QWidget
{
	Q_OBJECT

private:
    Ui::AnalysisResultSection*	ui;

public:
    AnalysisResultSection(QWidget *parent = 0);
    virtual ~AnalysisResultSection();
};
