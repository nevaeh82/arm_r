#include "SpectrumWidgetDataSource.h"
#include "Rpc/RpcDefines.h"

SpectrumWidgetDataSource::SpectrumWidgetDataSource(RPCClient* rpcClient, QObject *parent) :
	BaseDataSource(parent)
{
	m_rpcController = rpcClient;
	m_rpcController->registerReceiver(this);
}

void SpectrumWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	if (RPC_SLOT_SERVER_SEND_POINTS == method) {
		//set_data(arg.toByteArray(), true); //spectrum
	} else if(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH == method) {
		//setDetectedAreas(arg.toByteArray());
	} else if(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION == method) {
		//set_def_modulation(arg.toString()); //spectrum
	} else if (RPC_SLOT_SERVER_SEND_CORRELATION == method){
		//correlation
		//TODO: point2 from rpc
		int point2 = 0;
		//set_data(point2, arg.toByteArray(), true);
		onDataReceived(method, data);
	}
}

void SpectrumWidgetDataSource::sendCommand(int)
{
}
