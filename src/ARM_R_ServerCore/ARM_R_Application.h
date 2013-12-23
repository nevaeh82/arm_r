#ifndef ARM_R_APPLICATION_H
#define ARM_R_APPLICATION_H

#include <QtCore/QCoreApplication>
#include <stdexcept>
#include <PwLogger/PwLogger.h>


class ARM_R_Application : public QCoreApplication
{
public:
    ARM_R_Application(int& argc, char** argv);

    bool notify(QObject* receiver, QEvent* event);

private:
    static Pw::Logger::ILogger* m_logger;

};

#endif // ARM_R_APPLICATION_H
