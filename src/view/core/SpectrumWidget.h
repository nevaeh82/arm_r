#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QApplication>

#include <QWidget>

#include "Common/CommandMessage.h"
#include "SpectrumWidgetController.h"

#include "Prm300ControlWidget.h"

namespace Ui {
class SpectrumWidget;
}

class SpectrumWidget : public QWidget
{
	Q_OBJECT
private:
	Ui::SpectrumWidget*	ui;

	SpectrumWidgetController* m_controller;

public:
	SpectrumWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SpectrumWidget();

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setSpectrumName(const QString& name);
	QString getSpectrumName() const;

	Q_MG_SpectrumInterface* getGraphicsWidget();

	void setControlPrmState(bool state);

	Prm300ControlWidget* getPrm300Widget();



public slots:
	void slotSetEnableSpactrum(bool state);

public slots:
	void slotEnableKM(bool state);

signals:
	void signalEnableKM(bool state);

	void setPanoramaSignal(bool);
	void setAutoSearchSignal(bool);
	void selectionTypeChangedSignal(bool);
	void requestDataSignal(bool);

	void setShowPeaksSignal(bool);
	void setShowControlPRM(bool);
};

#endif // GRAPHICWIDGET_H
