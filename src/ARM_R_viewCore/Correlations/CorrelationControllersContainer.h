#ifndef CORRELATIONCONTROLLERSCONTAINER_H
#define CORRELATIONCONTROLLERSCONTAINER_H

#include <QString>
#include <QMap>
#include <QMutex>

#include "Interfaces/ICorrelationControllersContainer.h"

#include "Correlations/CorrelationWidget.h"
#include "Correlations/CorrelationWidgetController.h"
#include "IGraphicWidget.h"

class CorrelationControllersContainer : public QObject, public ICorrelationControllersContainer
{
	Q_OBJECT

public:
	CorrelationControllersContainer(QObject* parent = NULL);
	virtual ~CorrelationControllersContainer();

public:
	virtual int init(int count);
	virtual ICorrelationWidget* get(int id);
	virtual int count();

private:
	QMap<int, ICorrelationWidget* >*	m_mapWidgets;
	QMutex	m_mux;
};

#endif // CORRELATIONCONTROLLERSCONTAINER_H
