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

#include "Interfaces/IRpcListener.h"

class GraphicData : public QObject, public IRpcListener
{
    Q_OBJECT

public:
	GraphicData(IGraphicWidget *gr_widget, ICommonComponents* common_correlations, ITabManager* tab_manager, int id, QObject* parent = NULL );
	virtual ~GraphicData();

	virtual void onMethodCalled(const QString &method, const QVariant &arg);

	void set_data(quint32 point2, const QByteArray &points, bool isComplex);

private:
    ICommonComponents*      _common_correlations;
    ITabManager*            _tab_manager;
    int                     _id;

    qreal                   _startx_cor;
	bool                    _needSetup;

    QMap<int, float*>               _map_peaks_correlation;
    QMap<int, float*>               _map_spectrum_corelation;
    QMap<int, double>               _map_bandwidth_corelation;

signals:
	void signalFinished();
	void signalSetCorData(quint32 point2, QByteArray vecFFT, bool isComplex);

    void signalDataS(float*,float*);

    void signalData(float*,float*);

private slots:
	void m_slotSetCorData(quint32 point2, QByteArray vecFFTBA, bool isComplex);
};

#endif // GRAPHICDATA_H
