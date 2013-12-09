#include "QSpectrView.h"

QSpectrView::QSpectrView(QGraphicsScene *scene, QWidget *pwgt)
    : QGraphicsView(scene,pwgt)
{

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QSpectrView::~QSpectrView()
{
}

void QSpectrView::slotFreqZoomIn()
{
    scale(1.1,1.0);
}
