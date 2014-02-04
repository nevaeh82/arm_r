#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QApplication>

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QPoint>
#include <qmath.h>
#include <QMutex>
#include <QMenu>
#include <QAction>
#include <QSpinBox>
#include <QPushButton>


#include <QTimer>

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "Tabs/ITabSpectrum.h"
#include "IGraphicWidget.h"

#include "Common/CommandMessage.h"


namespace Ui {
class GraphicsWidget;
}

class GraphicWidget : public QWidget, public IGraphicWidget
{
    Q_OBJECT
public:
	GraphicWidget(QWidget *parent = 0, Qt::WFlags flags = 0, QString name = "", int id = 0);
    ~GraphicWidget();

	void setTab(ITabSpectrum* tab);

private:
	Ui::GraphicsWidget*     ui;

    int                     _id;
    float                   *_spectrum_peak_hold;
    float                   *_spectrum_peak_hold_corr;

    ITabSpectrum*           _tab;

    double                  _bandwidth;
    int                     _pointCount;
    bool                    _isComplex;

    QMutex                  _mux;
    bool                    _enable_correlation;
	QMenu*                  m_graphicsContextMenu;

    double                  _center_freq_sel_temp;
    double                  _center_freq_def_modulation;

    bool                    _peak_visible;

    double                  _threshold;
    int _rett;

public:
    Q_MG_SpectrumInterface  *get_spectrum_widget();
    virtual void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
    virtual void setSignal(float* spectrum, float* spectrum_peak_hold);
    virtual bool isGraphicVisible();
    virtual void setDefModulation(QString modulation);
    virtual void setLabelName(QString base, QString second);

    void setup();

    void set_coontrolPRM_state(bool state);


public slots:
    void slotSelectionFinished(double x1, double y1, double x2, double y2);
    void slotSelectionFinishedRedLine(double y);
    void slotSetCaption(QString name);
    void slotSetCorrelations(int id, const QVector<QPointF> vecFFT, const bool isComplex);
    void _slotSetFFT(float* spectrum, float* spectrum_peak_hold);
    void _slotSetFFTSetup(float* spectrum, float* spectrum_peak_hold);
    void _slotSetDefModulation(QString modulation);

//    void slotSetFFT2(QVector<QPointF> vecFFT, const bool isComplex);
//    void slotSetParam(int PointCount, double bandwidth, bool isComplex);


private slots:
    void _slotSetEnablePanorama(bool state);
    void _slotAutoSearch(bool state);
    void _slotSelectiontypeChanged(bool);
    void _slotRequestData(bool state);
    void _slotEnableKM(bool state);
    void _slotIsShowContextMenu();
    void _slotCMAddWhiteList();
    void _slotCMAddBlackList();
    void _slotRecognizeSignal();
    void _slotSSCorrelation();
    void _slotClearLabels();

    void _slotShowPeaks(bool);
    void _slotSetLabelName(QString base, QString second);

    void _slotShowControlPRM(bool state);

    void _slot_double_clicked(double, double);

//    void _slotSetFFTSetup(float* spectrum, float* spectrum_peak_hold);
//    void _slotSetFFT(float* spectrum, float* spectrum_peak_hold);

signals:
    void selected(double x1, double x2, double y1, double y2);
    void signalChoosedThreshold(double y);
    void signalCurSelChanged(int);
    void signalRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);
    void signalEnableKM(bool state);

    /// id - is id of ilsts (1 - White list, 2 - Black list)
    void signalAddSelToLists(int id);


    void signalSetFFTSetup(float* spectrum, float* spectrum_peak_hold);
    void signalSetFFT(float* spectrum, float* spectrum_peak_hold);
    void signalSetDefModulation(QString modulation);

    void signalFinished();

    void signalNeedToUpdate();

    void signalSetLabelName(QString, QString);

    void signalSetControlPRMState(bool state);

private:
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );


};

#endif // GRAPHICWIDGET_H
