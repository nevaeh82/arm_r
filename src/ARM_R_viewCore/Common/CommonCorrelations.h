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
    virtual int init(QString path_to_ini_file);
    virtual int init(int count);
	virtual int set(int id, ICorrelationWidget *widget);
	virtual ICorrelationWidget* get(int id);
    virtual int count(int id);
	virtual QMap<int, ICorrelationWidget* >* get_components();

private:
	QMap<int, ICorrelationWidget* >*      _map_widgets;
    QMutex                           _mux;
};

#endif // COMMONCORRELATIONS_H
