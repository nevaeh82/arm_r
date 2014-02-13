#ifndef IRPC_H
#define IRPC_H

#include <QtGlobal>

class IClient;

class IRPC
{
	public:
		virtual ~IRPC(){}

		virtual quint64 getClientId(IClient* client)      = 0;
};

#endif // IRPC_H
