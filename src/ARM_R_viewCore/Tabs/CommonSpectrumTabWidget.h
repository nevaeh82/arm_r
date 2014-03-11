﻿#ifndef COMMONSPECTRUMTABWIDGET_H
#define COMMONSPECTRUMTABWIDGET_H

#include <QWidget>

#include "SpectrumWidget.h"

#include "Interfaces/ICorrelationControllersContainer.h"
#include "Interfaces/ITabWidget.h"

#include "SettingsTree/TreeModel.h"

namespace Ui {
class CommonSpectrumTabWidget;
}

class CommonSpectrumTabWidget : public QWidget, public ITabWidget
{
	Q_OBJECT

private:
	Ui::CommonSpectrumTabWidget *ui;

	QList<ISpectrumWidget*> m_widgetList;

	ICorrelationControllersContainer* m_correlationControllers;

	TreeModel*          m_treeModel;

	IDbManager* m_dbManager;


public:
	explicit CommonSpectrumTabWidget(QWidget *parent = 0);
	~CommonSpectrumTabWidget();

	//void insertSpectrumWidget(GraphicWidget* widget);
	void clearSpectrumWidgetsContainer();

	void setCorrelationComponent(ICorrelationControllersContainer* correlationControllers);

	virtual void activate();
	virtual void deactivate();

	QWidget* getWidget();
	ISpectrumWidget* getSpectrumWidget();

	virtual void insertSpectrumWidget(ISpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	virtual void setDbManager(IDbManager* dbManager);

	QLabel* getIndicator();

	virtual int createRPC();
	virtual int closeRPC();
	virtual int createView(QWidget *);
	virtual int createTree();


};

#endif // COMMONSPECTRUMTABWIDGET_H