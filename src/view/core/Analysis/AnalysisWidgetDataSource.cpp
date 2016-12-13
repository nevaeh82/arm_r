#include "AnalysisWidgetDataSource.h"

#include "Rpc/RpcDefines.h"

#include "Interfaces/IAnalysisWidget.h"

#include "Logger/Logger.h"

#define TO_KHZ 1000

AnalysisWidgetDataSource::AnalysisWidgetDataSource(IGraphicWidget* correlationWidget, ITabManager* tabManager, int id, QObject *parent)
	: BaseDataSource(parent)
	, m_needSetup(false)
	, m_type(id)
{
	m_correlationWidget = correlationWidget;

	m_tabManager = tabManager;

	m_mapPeaksCorrelation = new float[1];
	m_mapSpectrumCorelation = new float[1];
	m_mapBandwidthCorelation = 0;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));

}

AnalysisWidgetDataSource::~AnalysisWidgetDataSource()
{
	//log_debug("<<<<<<<<<<<<<<");
}

void AnalysisWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	emit onMethodCalledSignal(method, data);
}

void AnalysisWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
	if( method == RPC_METHOD_CONFIG_RDS_ANSWER ) {
		RdsProtobuf::ServerMessage sMsg;
		if( !parseServerMessage(data.toByteArray(), sMsg) ) {
			return;
		}

		if( isServerAnalysisShot(sMsg) ) {
			RdsProtobuf::ServerMessage_OneShotData_AnalysisData adata = getServerAnalysisShot( sMsg );
			QByteArray outData;
			outData.resize(adata.ByteSize());
			adata.SerializeToArray( outData.data(), outData.size() );
			onDataReceived(RPC_SLOT_SERVER_SEND_ANALYSIS, outData);
		}
	}
}

void AnalysisWidgetDataSource::sendCommand(int)
{
}

