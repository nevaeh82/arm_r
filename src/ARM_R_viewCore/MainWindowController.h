#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>

#include <PwLogger/PwLogger.h>

#include "Interfaces/IController.h"
#include "MainWindow.h"

#include "Common/ServiceHandler/ServiceHandler.h"

class MainWindowController : public QObject, public IController<MainWindow>
{
	Q_OBJECT

private:
	/// logger
	Pw::Logger::ILogger* m_logger;

	MainWindow* m_view;

	Pw::Common::ServiceControl::ServiceHandler* m_serverHandler;

public:
	explicit MainWindowController(QObject *parent = 0);
	virtual ~MainWindowController();

	void appendView(MainWindow *view);

	void startServer();

signals:

private slots:
	void serverFailedToStartSlot();
	void serverStartedSlot();

private:

	void start();
	void stop();

	void init();

};

#endif // MAINWINDOWCONTROLLER_H
