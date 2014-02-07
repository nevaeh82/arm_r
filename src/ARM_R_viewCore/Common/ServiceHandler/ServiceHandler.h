#ifndef SERVICEHANDLER_H
#define SERVICEHANDLER_H

#include <QObject>
#include <QProcess>
#include <QThread>

#include "../Interfaces/ITerminateRpcController.h"

#include <PwLogger/PwLogger.h>

#include "IServiceHandler.h"
#include <QMutex>

class TestServiceHandler;

namespace Pw
{
namespace Common
{
namespace ServiceControl
{

class ServiceHandler : public QObject, public IServiceHandler
{
	Q_OBJECT

	friend class ::TestServiceHandler;


private:
	class SharedThread : public QThread
	{
	public:
		using QThread::msleep;
	};

#ifdef Q_OS_WIN

	void* _childCloseJob;

#endif
public:

protected:
	const int c_terminateTimeout;
	QProcess* _process;
	QString _servicePath;
	QStringList _args;
	QString _cmdline;
	bool _terminating;
	ProcessState::Enum _state;
	Pw::Common::ServiceControl::ServiceErrorCodes::Enum _lastServiceError;

	Pw::Logger::ILogger* _logger;
	QMutex _syncRoot;

	ITerminateRpcController* m_agentRpcController;

	void startService();

public:

	/// @brief Initializes a new instance of ServiceHasndler for using with concrete service.
	ServiceHandler(const QString& servicePath, const QStringList& args, ITerminateRpcController* rpcController, QObject* parent=NULL);

protected:
	QString getCommandLine();

	virtual void errorCodesProcessing(int code);

public:
	void kill();

	/// @brief Gets service path.
	///
	QString getServicePath();


	/// @brief Descructor. The object terminates ran service on destory.
	virtual ~ServiceHandler();

	/// @brief  Get current state
	/// @returns  state of service process
	ProcessState::Enum  getState(){ return _state;}

	/**
	 * @brief Get service last error.
	 * @return error code.
	 */
	virtual Pw::Common::ServiceControl::ServiceErrorCodes::Enum getLastError() { return _lastServiceError; }

	/// @brief Indicates whether service available
	///
	bool isServiceAvailable();

	void start();

signals:
	void processStartedSignal();
	void processStartFailedSignal();

public slots:

	/// @brief Terminate service.
	///
	int terminate();

	/** Method returns opened map ID. (aka Process ID)

	  @return  process ID which provides map.
	  */
	int serviceHandle();

	/** Indicates whether service started .
	  @return  true is service started.
	  */
	bool isServiceStarted();


	/** Gets service filename without path;
	  */
	virtual QString serviceName();

	/// @brief Wait service started.
	/// @return true if service started successfully
	/// @param timeout   timeout in seconds.
	bool waitService(int timeout);

	void waitForFinished();

	/**
	 * @brief Add argument to command line
	 * @param arg  command line argument.
	 */
	void addArgument(QString arg);

protected slots:
	void onProcessDestroyed(int, QProcess::ExitStatus);
	void onReadProcessOutput();
	void onReadProcessError();
	void onProcessStarted();
	void onProcessStartFailed(QProcess::ProcessError);
};

} // namespace ServiceControl
} // namespace Common
} // namespace Pw

#endif // SERVICEHANDLER_H
