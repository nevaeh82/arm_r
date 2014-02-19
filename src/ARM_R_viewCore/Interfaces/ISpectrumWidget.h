#ifndef ISPECTRUMWIDGET_H
#define ISPECTRUMWIDGET_H

class QString;
class ITabSpectrum;
class QWidget;

#include "IGraphicWidget.h"
#include "Interfaces/IDataSourceListener.h"

class ISpectrumWidget: public IGraphicWidget, public IDataSourceListener
{
	public:
		virtual ~ISpectrumWidget(){}

		virtual void setTab(ITabSpectrum*) = 0;
		virtual void setId(const int) = 0;
		virtual void setSpectrumName(const QString&) = 0;
		virtual void setControlPrmState(bool) = 0;

		virtual void setAutoSearch(bool) = 0;
		virtual void setPanorama(bool) = 0;

		virtual void setFFTSetup(float* spectrum, float* spectrum_peak_hold) = 0;

		virtual QString getSpectrumName() const = 0;
		virtual QWidget* getWidget() const = 0;
};

#endif // ISPECTRUMWIDGET_H
