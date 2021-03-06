#include <QDebug>

#include <Logger/Logger.h>

#include "ARM_R_Application.h"

ARM_R_Application::ARM_R_Application(int& argc, char** argv) :
	QtSingleCoreApplication(argc, argv)
{
}


bool ARM_R_Application::notify(QObject* receiver, QEvent* event)
{
	bool done = true;
	try
	{
		done = QCoreApplication::notify(receiver, event);
	} catch (const std::exception& e)
	{
		qFatal("Error %s sending event %s to object %s (%s)",
			   e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
			   typeid(*receiver).name());

	} catch (...)
	{
		log_info("Terminated by fatal error!");
		//  }
		return done;
	}

	return done;
}
