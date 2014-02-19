#ifndef IGRAPHICWIDGET_H
#define IGRAPHICWIDGET_H

#include <QString>
#include <QPointF>

class IGraphicWidget
{
	public:
		virtual ~IGraphicWidget(){}
		virtual bool isGraphicVisible() = 0;

		virtual void setZeroFrequency(double val) = 0;
};

#endif // IGRAPHICWIDGET_H
