#ifndef ICOMMANDQUEUE_H
#define ICOMMANDQUEUE_H

#include <QObject>

class ICommand;

template <typename T>
class ICommandQueue
{
public:
	virtual ~ICommandQueue(){}
	virtual void addCommand(T* command) = 0;
	virtual void exec() = 0;
	virtual void stop() = 0;
	virtual void clear() = 0;

	virtual QObject* asQObject() =0;

	virtual void setCommandQueueMultithreaded(bool isEnable) = 0;
};

#endif // ICOMMANDQUEUE_H
