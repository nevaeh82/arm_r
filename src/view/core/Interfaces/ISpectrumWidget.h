#ifndef ISPECTRUMWIDGET_H
#define ISPECTRUMWIDGET_H

class QString;
class ITabSpectrum;
class QWidget;

#include "IGraphicWidget.h"
#include "Interfaces/IDataSourceListener.h"
#include "ControlPanel/ControlPanelController.h"
#include "IDbManager.h"
#include "DBStation/DBStationController.h"

class ISpectrumWidget: public IGraphicWidget, public IDataSourceListener
{
public:
	virtual ~ISpectrumWidget(){}

	virtual void setTab(ITabSpectrum*) = 0;
	virtual void setId(const int) = 0;
	virtual void setChannelId(const int) = 0;
	virtual void setPlatformId(const int) = 0;
	virtual void setSpectrumName(const QString&) = 0;
	virtual void setControlPrmState(bool) = 0;

	virtual void setAutoSearch(bool) = 0;
	virtual void setPanorama(bool) = 0;

	virtual void setSelection(double start, double end) = 0;
	virtual void setAnalysisSelection(double start, double end) = 0;

	virtual void updateSelection() = 0;
    virtual double getCenterSelection() = 0;

	virtual void setFFTSetup(float* spectrum, float* spectrum_peak_hold) = 0;

	virtual QString getSpectrumName() const = 0;
	virtual QWidget* getWidget() const = 0;
	virtual void setDbManager(IDbManager* dbManager) = 0;
	virtual void setDbStationController(DBStationController* controller) = 0;
	virtual void setControlPanelController(ControlPanelController* controller) = 0;

	virtual void setReceiverStatus(bool state) = 0;
	virtual void setReceiverSettings(const RdsProtobuf::ReceiverSettings& settings) = 0;

};

#endif // ISPECTRUMWIDGET_H
