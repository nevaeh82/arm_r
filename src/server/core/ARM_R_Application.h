#ifndef ARM_R_APPLICATION_H
#define ARM_R_APPLICATION_H

#include "singleapplication.h"
#include <stdexcept>

class ARM_R_Application : public SingleApplication
{
public:
	ARM_R_Application(int& argc, char** argv);

	bool notify(QObject* receiver, QEvent* event);
};

#endif // ARM_R_APPLICATION_H
