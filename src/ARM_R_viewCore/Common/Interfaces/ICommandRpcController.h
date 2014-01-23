#ifndef IRPCCOMMANDCONTROLLER_H
#define IRPCCOMMANDCONTROLLER_H

#include <QByteArray>

class ICommandRpcController
{
public:
	virtual ~ICommandRpcController(){}

	virtual void sendAnswer(int answer) = 0;
	virtual void receiveCommand(QByteArray command) = 0;
};


#endif // IRPCCOMMANDCONTROLLER_H
