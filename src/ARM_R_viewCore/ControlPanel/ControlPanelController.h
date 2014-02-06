#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>

#include "Interfaces/IController.h"
#include "ControlPanelWidget.h"

class ControlPanelController : public QObject, public IController<ControlPanelWidget>
{
	Q_OBJECT

private:
	ControlPanelWidget* m_view;

public:
	explicit ControlPanelController(QObject *parent = 0);
	virtual ~ControlPanelController();

	void appendView(ControlPanelWidget* view);


signals:

public slots:

};

#endif // CONTROLPANELCONTROLLER_H
