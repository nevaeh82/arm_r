#include "ColorGraph.h"

ColorGraph::ColorGraph(double timming, QCPAxis *keyAxis, QCPAxis *valueAxis)
: QCPGraph( keyAxis, valueAxis)
{
    m_timming = timming;
    m_timmingGlobal = 50;
}

void ColorGraph::setColorMap(QCPLinearColorMap *map)
{
    m_colorMap = map;
}

//void ColorGraph::draw(QCPPainter *painter)
//{
//    if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
//    if (mKeyAxis.data()->range().size() <= 0 || mData->isEmpty()) return;
//    if (mLineStyle == lsNone && mScatterStyle.isNone()) return;

//    // allocate line and (if necessary) point vectors:
//    QVector<QPointF> *lineData = new QVector<QPointF>;
//    QVector<QCPData> *scatterData = 0;
//    mScatterStyle.setShape(QCPScatterStyle::ScatterShape::ssDot);
////    if (!mScatterStyle.isNone())
//      scatterData = new QVector<QCPData>;

//    // fill vectors with data appropriate to plot style:
//    getPlotData(lineData, scatterData);

//    // check data validity if flag set:

//    // check data validity if flag set:
//  #ifdef QCUSTOMPLOT_CHECK_DATA
//    QCPDataMap::const_iterator it;
//    for (it = mData->constBegin(); it != mData->constEnd(); ++it)
//    {
//      if (QCP::isInvalidData(it.value().key, it.value().value) ||
//          QCP::isInvalidData(it.value().keyErrorPlus, it.value().keyErrorMinus) ||
//          QCP::isInvalidData(it.value().valueErrorPlus, it.value().valueErrorPlus))
//          log_debug(QString("Data point at %1 invalid. Plottable name: %2").arg(it.key()).arg(name()));
//    }
//  #endif

////    // draw fill of graph:
////    if (mLineStyle != lsNone)
////      drawFill(painter, lineData);

////    // draw line:
////    if (mLineStyle == lsImpulse)
////      drawImpulsePlot(painter, lineData);
////    else if (mLineStyle != lsNone)
////      drawLinePlot(painter, lineData); // also step plots can be drawn as a line plot

////    // draw scatters:
//      drawScatterPlot(painter, scatterData);

////    // free allocated line and point vectors:
////    delete lineData;
////    if (scatterData)
////      delete scatterData;
//}

void ColorGraph::drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

  // draw error bars:
  if (mErrorType != etNone)
  {
    applyErrorBarsAntialiasingHint(painter);
    painter->setPen(mErrorPen);
    if (keyAxis->orientation() == Qt::Vertical)
    {
      for (int i=0; i<scatterData->size(); ++i)
        drawError(painter, valueAxis->coordToPixel(scatterData->at(i).value), keyAxis->coordToPixel(scatterData->at(i).key), scatterData->at(i));
    } else
    {
      for (int i=0; i<scatterData->size(); ++i)
        drawError(painter, keyAxis->coordToPixel(scatterData->at(i).key), valueAxis->coordToPixel(scatterData->at(i).value), scatterData->at(i));
    }
  }

  // draw scatter point symbols:
  applyScattersAntialiasingHint(painter);
  mScatterStyle.applyTo(painter, mPen);

  if (keyAxis->orientation() == Qt::Vertical)
  {
    for (int i=0; i<scatterData->size(); ++i)
      if (!qIsNaN(scatterData->at(i).value))
      {
          QColor color = m_colorMap->color(-100, -50, scatterData->at(i).value);
          painter->setPen(color);
//          log_debug(QString("Color = %1 %2 %3 %4").arg(color.alpha()).arg(color.red()).arg(color.green()).arg(color.blue()));
        mScatterStyle.drawShape(painter, valueAxis->coordToPixel(scatterData->at(i).value), keyAxis->coordToPixel(scatterData->at(i).key));

      }
  }else
  {
    for (int i=0; i<scatterData->size(); ++i)
      if (!qIsNaN(scatterData->at(i).value))
      {
          QColor color = m_colorMap->color(-100, -50, scatterData->at(i).value);
          painter->setPen(color);
//          log_debug(QString("Color = %1 %2 %3 %4").arg(color.alpha()).arg(color.red()).arg(color.green()).arg(color.blue()));

          mScatterStyle.drawShape(painter, keyAxis->coordToPixel(scatterData->at(i).key), valueAxis->coordToPixel(m_timming/*scatterData->at(i).value*/));

      }
  }
}

//void ColorGraph::drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const
//{
//  QCPAxis *keyAxis = mKeyAxis.data();
//  QCPAxis *valueAxis = mValueAxis.data();
//  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

//  // draw error bars:
//  if (mErrorType != etNone)
//  {
//    applyErrorBarsAntialiasingHint(painter);
//    painter->setPen(mErrorPen);
//    if (keyAxis->orientation() == Qt::Vertical)
//    {
//      for (int i=0; i<scatterData->size(); ++i)
//        drawError(painter, valueAxis->coordToPixel(scatterData->at(i).value), keyAxis->coordToPixel(scatterData->at(i).key), scatterData->at(i));
//    } else
//    {
//      for (int i=0; i<scatterData->size(); ++i)
//        drawError(painter, keyAxis->coordToPixel(scatterData->at(i).key), valueAxis->coordToPixel(scatterData->at(i).value), scatterData->at(i));
//    }
//  }

//  // draw scatter point symbols:
//  applyScattersAntialiasingHint(painter);
//  mScatterStyle.applyTo(painter, mPen);

//    for (int i=0; i<scatterData->size(); ++i)
//      if (!qIsNaN(scatterData->at(i).value))
//      {
//          QColor color = m_colorMap->color(-100, -75, scatterData->at(i).value);
//          painter->setPen(color);
//          mScatterStyle.drawShape(painter,  scatterData->at(i).key, scatterData->at(i).value);
//      }
//       // mScatterStyle.drawShape(painter, valueAxis->coordToPixel(scatterData->at(i).value), keyAxis->coordToPixel(scatterData->at(i).key));

//}
