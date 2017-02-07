#ifndef CORRELATIONCONTROLLERSCONTAINER_H
#define CORRELATIONCONTROLLERSCONTAINER_H

#include <QString>
#include <QMap>
#include <QMutex>

#include "Interfaces/ICorrelationControllersContainer.h"

#include "Correlations/CorrelationWidget.h"
#include "Correlations/CorrelationIndicatorWidget.h"
#include "Correlations/CorrelationWidgetController.h"

#include "IGraphicWidget.h"

class CorrelationControllersContainer : public QObject, public ICorrelationControllersContainer
{
	Q_OBJECT

public:
	CorrelationControllersContainer(QObject* parent = NULL);
	virtual ~CorrelationControllersContainer();

public:
    virtual int init(int count, int type = 0);
	virtual ICorrelationWidget* get(int id);
	virtual int count();

	void setLocationController(LocationSetupWidgetController* controller);
    void setControlPanelController(ControlPanelController*);

private:
	QMap<int, ICorrelationWidget* > m_mapWidgets;
	QMutex	m_mux;
    LocationSetupWidgetController* m_locationController;
    ControlPanelController* m_controlPanel;

signals:
    void signalExpand();
};

#endif // CORRELATIONCONTROLLERSCONTAINER_H
