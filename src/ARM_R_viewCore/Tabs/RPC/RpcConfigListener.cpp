#include "RpcConfigListener.h"

RpcConfigListener::RpcConfigListener(QObject* parent) :
	QObject(parent)
{
}


void RpcConfigListener::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();
	if (method == RPC_METHOD_CONFIG_ANSWER_STATION_LIST) {
		//	QDataStream dataStream(&data, QIODevice::ReadOnly);
		//	QList<StationConfiguration> stationList;
		//	dataStream >> stationList;
	} else if (method == RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION) {
		//	QDataStream dataStream(&data, QIODevice::ReadOnly);
		//	AtlantConfiguration atlantConfiguration;
		//	dataStream >> atlantConfiguration;
	}
}
