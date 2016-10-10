#include "AnalysisResultSection.h"

#include "ui_AnalysisResultSection.h"

#include <QLabel>

AnalysisResultSection::AnalysisResultSection(QWidget *parent):
    QWidget(parent),
    ui(new Ui::AnalysisResultSection)
{
	ui->setupUi(this);
}

AnalysisResultSection::~AnalysisResultSection()
{
}
