#ifndef ISUBMODULE_H
#define ISUBMODULE_H

class QWidget;
class QLabel;
class SpectrumWidget;
class QString;

typedef enum TypeTabWidgetEnum
{
	TypeSignleSpectrum = 0,
	TypeCommonSpectrum = 1

}TypeTabWidgetEnum;

class ITabWidget
{
public:
	virtual ~ITabWidget(){}

	virtual void activate()   = 0;
	virtual void deactivate() = 0;

	virtual QWidget* getWidget() = 0;

	virtual QLabel* getIndicator() = 0;

	virtual SpectrumWidget* getSpectrumWidget() = 0;
	virtual void insertSpectrumWidget(SpectrumWidget* spectrumWidget) = 0;

	virtual TypeTabWidgetEnum getWidgetType() const = 0;
};

#endif // ISUBMODULE_H
