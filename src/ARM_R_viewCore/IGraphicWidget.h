#ifndef IGRAPHICWIDGET_H
#define IGRAPHICWIDGET_H

#include <QString>
#include <QPointF>

class IGraphicWidget
{
public:
    virtual ~IGraphicWidget(){};
	virtual void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)   = 0;
	virtual void setSignal(float* spectrum, float* spectrum_peak_hold)                                                          = 0;
	virtual void setDefModulation(QString modulation)																			= 0;
	virtual bool isGraphicVisible()																								= 0;
	virtual void setLabelName(QString base, QString second)																		= 0;
	virtual void setDetectedAreasUpdate(QVector<QPointF> vec)																	= 0;
	virtual void setZeroFrequency(double val)																					= 0;
};

#endif // IGRAPHICWIDGET_H
