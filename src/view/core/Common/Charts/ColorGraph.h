#ifndef COLORGRAPH_H
#define COLORGRAPH_H

#include <Logger/Logger.h>

#include "qcustomplot.h"
#include "QCPLinearColorMap.h"

class ColorGraph : public QCPGraph
{
public:
  ColorGraph(double timming, QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~ColorGraph() {}
  void setColorMap(QCPLinearColorMap *map);

protected:
//  virtual void draw( QCPPainter *painter);
  void drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const;
    // Draw your stuff here based on code in QCPGraph::draw().

private:
  QCPLinearColorMap* m_colorMap;
  int m_timmingGlobal;
  int m_timming;
};

#endif // COLORGRAPH_H
