#ifndef ICORRELATIONWIDGET_H
#define ICORRELATIONWIDGET_H

#include "IGraphicWidget.h"

class ICorrelationWidget: public IGraphicWidget
{
	public:
		virtual ~ICorrelationWidget(){}

		virtual void clear() = 0;
};

#endif // ICORRELATIONWIDGET_H
