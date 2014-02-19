#ifndef ICORRELATIONWIDGET_H
#define ICORRELATIONWIDGET_H

class QWidget;

#include "IGraphicWidget.h"

class ICorrelationWidget: public IGraphicWidget
{
	public:
		virtual ~ICorrelationWidget(){}

		virtual void clear() = 0;

		virtual QWidget *getWidget() const = 0;
};

#endif // ICORRELATIONWIDGET_H
