#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>

#include "Interfaces/IController.h"
#include "Interfaces/IDbManager.h"
#include "ControlPanelWidget.h"
#include "Interfaces/IControlPanelController.h"
#include "Interfaces/IControlPanelListener.h"

class ControlPanelController : public QObject, public IControlPanelController
{
	Q_OBJECT

private:
	ControlPanelWidget* m_view;

	IDbManager* m_dbManager;

public:
	explicit ControlPanelController(QObject *parent = 0);
	virtual ~ControlPanelController();

	void appendView(ControlPanelWidget* view);

	void setDbManager(IDbManager* dbManager);


signals:

public slots:

private slots:
	void onPanoramaStateChangedSlot(bool isEnabled);
	void onAutoSearchStateChangedSlot(bool isEnabled);

	void onCommonFrequencyChangedSlot(int value);
	void onBandWidthChangedSlot(int start, int end);

};

#endif // CONTROLPANELCONTROLLER_H
