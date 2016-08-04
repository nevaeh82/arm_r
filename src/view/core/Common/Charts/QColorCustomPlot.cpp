#include "QColorCustomPlot.h"

ColorGraph *QColorCustomPlot::addGraph(double timming,int style, QCPAxis *keyAxis, QCPAxis *valueAxis)
{
    if (!keyAxis) keyAxis = xAxis;
    if (!valueAxis) valueAxis = yAxis;
    if (!keyAxis || !valueAxis)
    {
      qDebug() << Q_FUNC_INFO << "can't use default QCustomPlot xAxis or yAxis, because at least one is invalid (has been deleted)";
      return 0;
    }
    if (keyAxis->parentPlot() != this || valueAxis->parentPlot() != this)
    {
      qDebug() << Q_FUNC_INFO << "passed keyAxis or valueAxis doesn't have this QCustomPlot as parent";
      return 0;
    }

    ColorGraph *newGraph = new ColorGraph(timming, keyAxis, valueAxis);
    if (addPlottable(newGraph))
    {
      newGraph->setName(QLatin1String("Graph ")+QString::number(mGraphs.size()));
      return newGraph;
    } else
    {
      delete newGraph;
      return 0;
    }
}
