#ifndef ISMSCOMPORTCONTROLLER_H
#define ISMSCOMPORTCONTROLLER_H

class SMSComPortDialog;

#include "Interfaces/IController.h"

class ISMSComPortController : public IController<SMSComPortDialog>
{
public:

	virtual ~ISMSComPortController(){}

};

#endif // ISMSCOMPORTCONTROLLER_H
