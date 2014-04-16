#ifndef ISUBMODULE_H
#define ISUBMODULE_H

class QWidget;
class QLabel;
class SpectrumWidget;
class QString;
class QStringList;
class ISpectrumWidget;
class IDbManager;

#include "DBStation/DBStationController.h"

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

	virtual ISpectrumWidget* getSpectrumWidget() = 0;
	virtual void insertSpectrumWidget(ISpectrumWidget* spectrumWidget) = 0;

	virtual TypeTabWidgetEnum getWidgetType() const = 0;

	virtual void setDbManager(IDbManager*) = 0;
    virtual void setDbStationController(DBStationController* controller) = 0;
	virtual void setStationNamesList(const QStringList& stationsList) = 0;

    virtual void updateListsSelections() = 0;
};

#endif // ISUBMODULE_H
