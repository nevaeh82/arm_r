#ifndef COMMONSPECTRUMTABWIDGET_H
#define COMMONSPECTRUMTABWIDGET_H

#include <QWidget>

#include "SpectrumWidget.h"

#include "Abstracts/ICommonComponents.h"
#include "Abstracts/ITabWidget.h"

#include "SettingsTree/TreeModel.h"

namespace Ui {
class CommonSpectrumTabWidget;
}

class CommonSpectrumTabWidget : public QWidget, public ITabWidget
{
	Q_OBJECT

private:
	Ui::CommonSpectrumTabWidget *ui;

	QList<SpectrumWidget*> m_widgetList;

	ICommonComponents* m_correlationComponent;

	TreeModel*          m_treeModel;

	IDbManager* m_dbManager;


public:
	explicit CommonSpectrumTabWidget(IDbManager *dbManager, QWidget *parent = 0);
	~CommonSpectrumTabWidget();

	//void insertSpectrumWidget(GraphicWidget* widget);
	void clearSpectrumWidgetsContainer();

	void setCorrelationComponent(ICommonComponents* correlation);

	virtual void activate();
	virtual void deactivate();

	QWidget* getWidget();
	SpectrumWidget* getSpectrumWidget();

	virtual void insertSpectrumWidget(SpectrumWidget *spectrumWidget);
	virtual TypeTabWidgetEnum getWidgetType() const;

	QLabel* getIndicator();

	virtual int createRPC();
	virtual int closeRPC();
	virtual int createView(QWidget *view);
	virtual int createTree();


};

#endif // COMMONSPECTRUMTABWIDGET_H
