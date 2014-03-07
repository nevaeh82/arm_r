#ifndef ARM_OD_APPLICATION_H
#define ARM_OD_APPLICATION_H

#include <QtCore/QCoreApplication>
#include <stdexcept>

class ARM_OD_Application : public QCoreApplication
{
public:
    ARM_OD_Application(int& argc, char** argv);

    bool notify(QObject* receiver, QEvent* event);

};

#endif // ARM_OD_APPLICATION_H
