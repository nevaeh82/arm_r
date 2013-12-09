#ifndef GRAPHICDATA_H
#define GRAPHICDATA_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QMap>
#include "IGraphicWidget.h"
#include "Tabs/TabsProperty.h"

#include "Tabs/ITabManager.h"

#include "Common/CommonCorrelations.h"

class GraphicData : public QObject
{
    Q_OBJECT
public:
    GraphicData(IGraphicWidget *gr_widget, ICommonComponents* common_correlations, ITabManager* tab_manager, int id);
    ~GraphicData();

public:
    void set_data(QVector<QPointF> points, bool isComplex);
    void set_data(quint32 point2, QVector<QPointF> points, bool isComplex);
    void set_def_modulation(QString modulation);

private:
    ICommonComponents*      _common_correlations;
    ITabManager*            _tab_manager;
    int                     _id;
    IGraphicWidget*         _gr_widget;
    float*                  _spectrum;
    float*                  _spectrum_peak_hold;
    double                  _bandwidth;
    qreal                   _startx;
    qreal                   _startx_cor;
    bool                    _needSetup;
    QMap<int, IGraphicWidget* >*    _map_correlation_widget;
    QMap<int, float*>               _map_peaks_correlation;
    QMap<int, float*>               _map_spectrum_corelation;
    QMap<int, double>               _map_bandwidth_corelation;


signals:
    void signalFinished();
    void signalSetDefModulation(QString modulation);
    void signalSetData(QVector<QPointF> vecFFT, bool isComplex);
    void signalSetCorData(quint32 point2, QVector<QPointF> vecFFT, bool isComplex);


    void signalDataS(float*,float*);

    void signalData(float*,float*);


private slots:
    void _slotSetData(QVector<QPointF> vecFFT, bool isComplex);
    void _slotSetCorData(quint32 point2,QVector<QPointF> vecFFT, bool isComplex);
    void _slotSetDefModulation(QString modulation);
};

#endif // GRAPHICDATA_H
