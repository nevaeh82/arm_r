#pragma once

class QWidget;

#include "Interfaces/IDataSourceListener.h"
#include "IGraphicWidget.h"

class IAnalysisWidget: public IGraphicWidget, public IDataSourceListener
{
	public:
		virtual ~IAnalysisWidget(){}

		virtual void clear() = 0;

		virtual QWidget *getWidget() const = 0;

		virtual int getType() const = 0;
		virtual int getAnalysisWorkId() const = 0;
};
