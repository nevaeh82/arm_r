#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QApplication>

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QPoint>
#include <qmath.h>
#include <QMutex>
#include <QMenu>
#include <QAction>
#include <QSpinBox>
#include <QPushButton>

#include <QTimer>

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "Tabs/ITabSpectrum.h"
#include "IGraphicWidget.h"

#include "Common/CommandMessage.h"
#include "SpectrumWidgetController.h"

namespace Ui {
class SpectrumWidget;
}

class SpectrumWidget : public QWidget//, public IGraphicWidget
{
	Q_OBJECT
private:
	Ui::SpectrumWidget*	ui;

	SpectrumWidgetController* m_controller;

public:
	SpectrumWidget(QWidget *parent = 0, Qt::WFlags flags = 0, QString name = "", int id = 0);
	~SpectrumWidget();

	void setSpectrumName(const QString& name);
	QString getSpectrumName() const;

	Q_MG_SpectrumInterface* getGraphicsWidget();

	void setControlPrmState(bool state);

public slots:

private slots:
	//void slotSetEnablePanorama(bool state);
	//void slotAutoSearch(bool state);
	//void slotSelectiontypeChanged(bool);
	//void slotRequestData(bool state);
	void slotEnableKM(bool state);

	/*void slotShowPeaks(bool);

	void slotShowControlPRM(bool state);*/

signals:
	//void selected(double x1, double x2, double y1, double y2);
	//void signalChoosedThreshold(double y);
	//void signalCurSelChanged(int);
	//void signalRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);
	void signalEnableKM(bool state);

	/// id - is id of ilsts (1 - White list, 2 - Black list)
	//void signalAddSelToLists(int id);

	//void signalFinished();
	//void signalNeedToUpdate();

	void setPanoramaSignal(bool);
	void setAutoSearchSignal(bool);
	void selectionTypeChangedSignal(bool);
	void requestDataSignal(bool);

	void setShowPeaksSignal(bool);
	void setShowControlPRM(bool);
};

#endif // GRAPHICWIDGET_H
