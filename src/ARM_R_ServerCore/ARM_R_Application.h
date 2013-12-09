#ifndef ARM_R_APPLICATION_H
#define ARM_R_APPLICATION_H

#include <QtCore/QCoreApplication>
#include <stdexcept>

class ARM_R_Application : public QCoreApplication
{
public:
    ARM_R_Application(int& argc, char** argv);

    bool notify(QObject* receiver, QEvent* event);

};

#endif // ARM_R_APPLICATION_H
