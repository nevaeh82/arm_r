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

#include "Interfaces/IDataSourceListener.h"

namespace Ui {
class CorrelationWidget;
}

class CorrelationWidget : public QWidget, public IGraphicWidget, public IDataSourceListener
{


private:
	Ui::CorrelationWidget*     ui;

    Q_OBJECT
public:
    CorrelationWidget(QWidget *parent = 0, Qt::WFlags flags = 0, QString name = "", int id = 0);
	virtual ~CorrelationWidget();

public:
	void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setSignal(float* spectrum, float* spectrum_peak_hold);
	bool isGraphicVisible();
	void setDefModulation(QString modulation);
	void setLabelName(QString base, QString second);

	void setDetectedAreasUpdate(const QByteArray& vec){}
	void setZeroFrequency(double val){}

	void onDataArrived(const QString& method, const QVariant& arg);
	void onDataArrived(float* spectrum, float* spectrumPeakHold, int pointCount, double bandwidth, bool isComplex);
	void onDataArrived(float* spectrum, float* spectrumPeakHold);

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
