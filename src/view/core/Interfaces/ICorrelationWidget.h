#ifndef ICORRELATIONWIDGET_H
#define ICORRELATIONWIDGET_H

class QWidget;

#include "Interfaces/IDataSourceListener.h"
#include "IGraphicWidget.h"

class ICorrelationWidget: public IGraphicWidget, public IDataSourceListener
{
	public:
		virtual ~ICorrelationWidget(){}

        virtual void setLabels(QString l1, QString l2) = 0;
		virtual void clear() = 0;

		virtual QWidget *getWidget() const = 0;
};

#endif // ICORRELATIONWIDGET_H
