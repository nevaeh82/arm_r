#ifndef IRPC_H
#define IRPC_H

class QString;
class QByteArray;

class IRPC
{
public:
	virtual ~IRPC(){}

	virtual void sendDataByRpc(const QString& signalType, const QString& deviceName, const QByteArray& data) = 0;
};

#endif // IRPC_H
