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

//class GraphicsDataTest;

class GraphicData : public QObject
{
    Q_OBJECT

	friend class GraphicsDataTest;

public:
    GraphicData(IGraphicWidget *gr_widget, ICommonComponents* common_correlations, ITabManager* tab_manager, int id);
    ~GraphicData();

public:
    void set_data(QVector<QPointF> points, bool isComplex);
    void set_data(quint32 point2, QVector<QPointF> points, bool isComplex);
    void set_def_modulation(QString modulation);
	void set_bandwidth(double bandwidth);
	void set_panorama(double start, double end);
	void set_panorama_stop();

	void setDetectedAreas(QVector<QPointF> vec);

	int _find_index(qreal startx);
	void m_dataProccess(QVector<QPointF> vecFFT, bool isComplex);
private:
    ICommonComponents*      _common_correlations;
    ITabManager*            _tab_manager;
    int                     _id;
    IGraphicWidget*         _gr_widget;
    float*                  _spectrum;
    float*                  _spectrum_peak_hold;
    double                  _bandwidth;
	double                  m_bandwidthSingleSample;
	bool					m_isPanaramaStart;
//	double					m_pointCountWhole;

    qreal                   _startx;
    qreal                   _startx_cor;
    bool                    _needSetup;
	bool					m_needSetupSpectrum;
    QMap<int, IGraphicWidget* >*    _map_correlation_widget;
    QMap<int, float*>               _map_peaks_correlation;
    QMap<int, float*>               _map_spectrum_corelation;
    QMap<int, double>               _map_bandwidth_corelation;

	QList<qreal>					_list_startx;

	QMap<qreal, QList<qreal> >			_map_spectrum;
	int								_PointCount;
	int								m_pointCountWhole;


signals:
    void signalFinished();
    void signalSetDefModulation(QString modulation);
    void signalSetData(QVector<QPointF> vecFFT, bool isComplex);
    void signalSetCorData(quint32 point2, QVector<QPointF> vecFFT, bool isComplex);


    void signalDataS(float*,float*);

    void signalData(float*,float*);

	void signalSetDetectedAreas(QVector<QPointF> vec);

	void signalSetBandwidth(double);
	void signalPanoramaStart(double start, double end);
	void signalPanoramaStop();


private slots:
	void m_slotSetData(QVector<QPointF> vecFFT, bool isComplex);
	void m_slotSetCorData(quint32 point2,QVector<QPointF> vecFFT, bool isComplex);
	void m_slotSetDefModulation(QString modulation);
	void m_slotSetBandwidth(double bandwidth);

	void m_slotPanoramaStart(double start, double end);
	void m_slotPanoramaStop();

	void m_slotSetDetectedAreas(QVector<QPointF> vec);
};

#endif // GRAPHICDATA_H
