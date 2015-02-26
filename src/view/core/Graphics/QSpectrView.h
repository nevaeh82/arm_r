#ifndef QSPECTRVIEW_H
#define QSPECTRVIEW_H

#include <QObject>
#include <QGraphicsView>

class QSpectrView: public QGraphicsView
{
    Q_OBJECT

public:
    QSpectrView(QGraphicsScene *, QWidget *pwgt=0);
    ~QSpectrView();
public slots:
    void slotFreqZoomIn();
};

#endif // QSPECTRVIEW_H
