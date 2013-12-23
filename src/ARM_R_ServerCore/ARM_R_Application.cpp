#include "ARM_R_Application.h"

#include <QDebug>

Pw::Logger::ILogger* ARM_R_Application::m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(ARM_R_Server));

ARM_R_Application::ARM_R_Application(int& argc, char** argv) :
    QCoreApplication(argc, argv)
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
      m_logger->fatal(QString("Terminated by fatal error!"));

  }
  return done;
}
