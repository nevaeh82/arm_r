#pragma once

#include "Tcp/BaseTcpDeviceCoder.h"
#include "Info/StationConfiguration.h"

#include <QDataStream>
#include <QPointF>
#include <QVector>

#include <QDateTime>

#include "RDSExchange.h"

#include "Info/StationConfiguration.h"
#include "TcpDefines.h"
#include "CRCs.h"

#include "Flakon/CoordinateCounter.h"

struct PrmSettings
{
	int att1;
	int att2;
	QString ip;
};

enum InputDataType
{
	spectrum,
	correlation
};

class TcpRdsCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	ZaviruhaPayloadPacketHeader m_header;
	int m_residueLength;
	QByteArray m_dataFromTcpSocket;
	int cnt;

	int m_freq;

	int m_test;
	QString m_sTest;

	QDateTime m_specTime;
	InputDataType m_inputData;

	CoordinateCounter* m_coordCounter;

	//LocSystemConfiguration m_locConf;
	RdsProtobuf::ClientMessage_OneShot_Location m_locationMessage;
	RdsProtobuf::ClientMessage_OneShot_Analysis m_analysisMessage;
	RdsProtobuf::ClientMessage_OneShot_Record m_recordMessage;

	QMap<int, QString> m_mapPrm;

	QMap<int, PrmSettings> m_mapPrmSettings;
	QMap<int, bool> m_mapDevState;

	int m_indexConv1;
	int m_indexConv2;
	int m_indexSpect;

	QMap<int, QDateTime> m_mapSendSpectrumTime;
	QMap<quint64, QDateTime> m_mapSendConvolutionTime;
	QMap<int, QDateTime> m_mapSendDetectorTime;
	QMap<int, QDateTime> m_mapSendAnalysisTime;
	QMap<int, QDateTime> m_mapSendAnalysisSonogramTime;
	QMap<int, QDateTime> m_mapSendAnalysisAbsTime;

	int m_upTime;
	bool m_readyToPushSpectrum;
	bool m_readyToSend;

	QTimer* m_dropFlagTimer;

public:
	explicit TcpRdsCoder(unsigned int zone, unsigned int typeRDS, QObject* parent = NULL);
	virtual ~TcpRdsCoder();

	bool isSend() {return m_readyToSend;}
	void onSetFlag();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();

	void setCoordinatesCounter(CoordinateCounter* obj);

	QByteArray decodeWithCheckLocation(const MessageSP message, bool &isLocation);

	void setReadyToPushSpectrums(bool val = true);
private:
	MessageSP messageFromPreparedData(const QByteArray& data);
	MessageSP pointers(int index, float cf, QVector<QPointF> vec);

	MessageSP correlation(quint32 point1, quint32 point2, float timediff, float veracity, QVector<QPointF> points);
	MessageSP correlationAll(const QByteArray &data);

	void addPreambula(QByteArray& data);
	QByteArray getMessage(const QByteArray& input);

	MessageSP configureLoc(const QByteArray &data);
	MessageSP configureSys(const QByteArray &data);

private slots:
	void onDropFlag();

signals:
	void onChangeDevState(QMap<int, bool>);
	void onDetectSignal(int, QVector<QPointF>);

};
