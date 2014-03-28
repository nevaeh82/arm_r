#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QMutexLocker>

#include <Logger.h>

#include "ProcessStateEnumsToString.h"
#include "CommonCodesEnumsToString.h"
#include "ServiceTerminateCauseEnumsToString.h"

#include "ServiceHandler.h"

#include "ServiceHandler.h"
#include "CommonCodes.h"

#ifndef __linux__
#define SUFFIX  ".exe"
#else
#define SUFFIX
#endif

#ifdef Q_OS_WIN

#ifdef WINVER
#undef WINVER
#define WINVER 0x0501
#else
#define WINVER 0x0501
#endif

#include <windows.h>
#endif


typedef Pw::Common::ServiceControl::ServiceErrorCodes ServiceError;

namespace Pw
{
namespace Common
{
namespace ServiceControl
{

ServiceHandler::ServiceHandler(const QString& servicePath, const QStringList& args, ITerminateRpcController* rpcController, QObject *parent):
	QObject(parent),
	c_terminateTimeout(5000),
	_process(NULL),
	_terminating(false),
	_state(ProcessState::NotRunning),
	_lastServiceError( ServiceError::Success ),
	_syncRoot(QMutex::Recursive)
{

#ifdef Q_OS_WIN

	_childCloseJob = CreateJobObject(NULL, NULL);

#endif

	QMutexLocker locker(&_syncRoot);

	m_agentRpcController = rpcController;

	_servicePath = servicePath;
	_args = args;

	_cmdline = getCommandLine();

	if ( _servicePath.isEmpty()) {
		return;
	}

	_cmdline += " " + _args.join(" ");

	_process = new QProcess(this);
	connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(onProcessDestroyed(int,QProcess::ExitStatus)));
	connect(_process, SIGNAL(readyReadStandardOutput()), SLOT(onReadProcessOutput()));
	connect(_process, SIGNAL(readyReadStandardError()), SLOT(onReadProcessError()));
	connect(_process, SIGNAL(started()), this, SLOT(onProcessStarted()));
	connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onProcessStartFailed(QProcess::ProcessError)));
}

ServiceHandler::~ServiceHandler()
{
	QMutexLocker locker(&_syncRoot);

	if ( !_process ){
		return;
	}

	_process->disconnect(this);

	_process->kill();
	_process->waitForFinished(c_terminateTimeout);

	//delete _process;
	//_process = NULL;

#ifdef Q_OS_WIN
	CloseHandle(_childCloseJob);
#endif
}

QString ServiceHandler::getCommandLine()
{
	return QString("\"%1\"").arg(getServicePath());
}

void ServiceHandler::errorCodesProcessing(int code)
{
	_state = Pw::Common::ServiceControl::ProcessState::Error;
	_lastServiceError = (ServiceError::Enum) code;

	debug(QString("Service %2: %1 (Process State: %3)")
					.arg(ServiceErrorEnumsToString::enumToString(_lastServiceError))
					.arg(serviceName())
					.arg(ProcessStateEnumsToString::enumToString(_state)));
}

void ServiceHandler::onProcessStarted()
{
#ifdef Q_OS_WIN

	if(!_childCloseJob) return;

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli;
	memset(&jeli, 0, sizeof(jeli));
	jeli.BasicLimitInformation.LimitFlags = 0x00002000;//JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE Causes all processes associated with the job to terminate when the last handle to the job is closed.

	if(!SetInformationJobObject( _childCloseJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli))) return;

	AssignProcessToJobObject(_childCloseJob, _process->pid()->hProcess);

#endif

	emit processStartedSignal();
}

void ServiceHandler::onProcessStartFailed(QProcess::ProcessError error)
{
	if (QProcess::FailedToStart != error) {
		return;
	}

	emit processStartFailedSignal();
}

void ServiceHandler::onProcessDestroyed(int code, QProcess::ExitStatus status)
{
	QMutexLocker locker(&_syncRoot);
	debug(QString("onProcessDestroyed. Service %4 destroyed: rc:%1 status:%2 isexpected:%3")
				   .arg(code)
				   .arg((status == QProcess::CrashExit ? "crash" : "normal" ))
				   .arg(( _terminating ? "expected" : "not expected" ))
				   .arg(serviceName()));

	errorCodesProcessing(code);

	if ( status == QProcess::CrashExit || !_terminating ) {
		startService();
	}
}

void ServiceHandler::onReadProcessOutput()
{
	debug(QString("Service %2:%1").arg(QString(_process->readAllStandardOutput())).arg(serviceName()));
}

void ServiceHandler::onReadProcessError()
{
	debug(QString("Service %2:%1").arg(QString(_process->readAllStandardError())).arg(serviceName()));
}

void ServiceHandler::startService()
{
	_state = ProcessState::IsRunnung;
	//_process->startDetached(_cmdline);
	_process->start( _cmdline, QProcess::ReadOnly);
}

QString ServiceHandler::getServicePath()
{
	if ( !_servicePath.isEmpty()) {
		return _servicePath;
	}
	QStringList names;

#ifdef DEBUG
	names.append( serviceName() + "d" SUFFIX);
#endif
	names.append( serviceName() + "" SUFFIX);

	QStringList paths;
	paths.append(QDir::currentPath() );
	paths.append(QCoreApplication::applicationDirPath());

	foreach( QString path, paths ){
		foreach( QString name, names ) {
			QString fullname = QDir::toNativeSeparators(path + QDir::separator() + name);
			debug( QString("Checking %1").arg(fullname) );

			if ( QFile(fullname ).exists()) {
				_servicePath = fullname ;
				return _servicePath;
			}
		}
	}

	return _servicePath;
}

int ServiceHandler::serviceHandle()
{
	QMutexLocker locker(&_syncRoot);
	if ( !_process ) {
		return 0;
	}
	return (int)_process->pid();
}

bool ServiceHandler::isServiceStarted()
{
	QMutexLocker locker(&_syncRoot);
	if ( !_process ) {
		return false;
	}
	return _process->state() == QProcess::Running;
}

QString ServiceHandler::serviceName()
{
	return _servicePath;
}

int ServiceHandler::terminate()
{
	debug(" terminate >>>");
	QMutexLocker locker(&_syncRoot);

	_terminating = true;
	if ( !_process ) {
		return ServiceTerminateCause::NotRunned;
	}

	_process->disconnect(this);

	//Try shutdown correctly.
	if (m_agentRpcController == NULL) {
		kill();
		return ServiceTerminateCause::ByKill;
	}

	m_agentRpcController->sendTerminate(getServicePath()+ "" SUFFIX);
	qApp->processEvents();

	if ( _process->waitForFinished(c_terminateTimeout)) {
		debug(" waitForFinished = true");
		//_process->deleteLater();
		//_process = NULL;
		return ServiceTerminateCause::ByCommand;
	}
/*
	_process->terminate();
	if ( _process->waitForFinished(c_terminateTimeout) ) {
		_process->deleteLater();
		_process = NULL;
		return ServiceTerminateCause::ByTerminate;
	}*/

	kill();
	return ServiceTerminateCause::ByKill;
}
void ServiceHandler::kill()
{
	debug(" kill >>");
	QMutexLocker locker(&_syncRoot);

	_terminating = true;
	if ( !_process ) {
		return;
	}
	_process->disconnect(this);

#ifdef Q_OS_WIN
	_process->kill();
	_process->close();
	delete _process;
	_process = NULL;
#else
	_process->terminate();
	_process->waitForFinished(c_terminateTimeout);
#endif


	//_process->deleteLater();
	//_process = NULL;
}

bool ServiceHandler::isServiceAvailable()
{
	return _process ? _process->isOpen() : false;
}

void ServiceHandler::start()
{
	startService();
}

bool ServiceHandler::waitService(int timeout)
{
	QMutexLocker locker(&_syncRoot);

	QDateTime expireTime = QDateTime::currentDateTime().addSecs(timeout);

	while( !isServiceAvailable() && QDateTime::currentDateTime() < expireTime && _state == ProcessState::IsRunnung )
	{
		QCoreApplication::processEvents();
		SharedThread::msleep(100);
	}
	return isServiceAvailable();
}

void ServiceHandler::waitForFinished()
{
	if (_process == NULL){
		return;
	}

	_process->waitForFinished(c_terminateTimeout);

}

void ServiceHandler::addArgument(QString arg)
{
	_args.append(arg);
}

} // namespace ServiceControl
} // namespace Common
} // namespace Pw
