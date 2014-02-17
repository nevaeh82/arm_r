#ifndef CORRELATIONWIDGET_H
#define CORRELATIONWIDGET_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QMutex>

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "IGraphicWidget.h"

namespace Ui {
class CorrelationWidget;
}

class CorrelationWidget : public QWidget, public IGraphicWidget
{


private:
	Ui::CorrelationWidget*     ui;

    Q_OBJECT
public:
    CorrelationWidget(QWidget *parent = 0, Qt::WFlags flags = 0, QString name = "", int id = 0);
	virtual ~CorrelationWidget();

public:
    virtual void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
    virtual void setSignal(float* spectrum, float* spectrum_peak_hold);
    virtual bool isGraphicVisible();
    virtual void setDefModulation(QString modulation);
    virtual void setLabelName(QString base, QString second);

	virtual void setDetectedAreasUpdate(const QByteArray& vec){}
	virtual void setZeroFrequency(double val){}

public:
    void clear();



private:
	//Q_MG_SpectrumInterface  *_correlationWidget;
    double                  _bandwidth;
    int                     _pointCount;
    bool                    _isComplex;

    QMutex                  _mux;

    QString                 _label_name;

signals:
    void signalSetSignalSetup(float*,float*);
    void signalSetSignal(float*,float*);

    void signalSetLabelName(QString, QString);

private slots:
    void _slotSetSignalSetup(float* spectrum, float* spectrum_peak_hold);
    void _slotSetSignal(float* spectrum, float* spectrum_peak_hold);
    void _slotSetLabelName(QString base, QString second);

};

#endif // CORRELATIONWIDGET_H
