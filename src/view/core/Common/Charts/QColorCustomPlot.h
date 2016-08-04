#ifndef QCOLORCUSTOMPLOT_H
#define QCOLORCUSTOMPLOT_H

#include <QWidget>
#include "qcustomplot.h"
#include "ColorGraph.h"

class QColorCustomPlot : public QCustomPlot
{
public:
    QColorCustomPlot(QWidget *parent = 0) : QCustomPlot(parent) {}
    ColorGraph *addGraph(double timming, int style, QCPAxis *keyAxis, QCPAxis *valueAxis);
    ColorGraph *addGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
};

#endif // QCOLORCUSTOMPLOT_H
