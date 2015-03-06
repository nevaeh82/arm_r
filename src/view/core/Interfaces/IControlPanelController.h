#ifndef ICONTROLPANELCONTROLLER_H
#define ICONTROLPANELCONTROLLER_H

class IControlPanelListener;
class ControlPanelWidget;

#include <Templates/BaseSubject.h>
#include "Interfaces/IController.h"

class IControlPanelController : public BaseSubject<IControlPanelListener>, public IController<ControlPanelWidget>
{
public:

	virtual ~IControlPanelController(){}

};

#endif // ICONTROLPANELCONTROLLER_H
