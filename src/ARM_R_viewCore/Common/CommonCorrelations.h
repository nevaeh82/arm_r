#ifndef COMMONCORRELATIONS_H
#define COMMONCORRELATIONS_H

#include <QString>
#include <QMap>
#include <QMutex>

#include "../Abstracts/ICommonComponents.h"

#include "Correlations/CorrelationWidget.h"
#include "Correlations/CorrelationWidgetController.h"
#include "../IGraphicWidget.h"

class CommonCorrelations : public ICommonComponents
{
public:
    CommonCorrelations();
    ~CommonCorrelations();

public:
	virtual int init(int count);
	virtual ICorrelationWidget* get(int id);
	virtual int count();

private:
	QMap<int, ICorrelationWidget* >*	m_mapWidgets;
	QMutex	m_mux;
};

#endif // COMMONCORRELATIONS_H
