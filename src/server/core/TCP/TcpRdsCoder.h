#pragma once

#include "Tcp/BaseTcpDeviceCoder.h"
#include "Info/StationConfiguration.h"

#include <QDataStream>
#include <QPointF>
#include <QVector>

#include <QDateTime>

#include "RdsPacket.pb.h"

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

//	int m_chanNum;
//	int m_mode;
//	int m_baseIndex;
//	bool conv;
//	int spectMean;
//	int tunMode;
//	int duration;
//	int cfreq;

	LocSystemConfiguration m_locConf;

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

	int m_analysisIndex;
	int m_analysisCfreq;

public:
    explicit TcpRdsCoder(unsigned int zone, unsigned int typeRDS, QObject* parent = NULL);
	virtual ~TcpRdsCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();

    void setCoordinatesCounter(CoordinateCounter* obj);

private:
	MessageSP messageFromPreparedData(const QByteArray& data);
	MessageSP pointers(int index, float cf, QVector<QPointF> vec);
	MessageSP configure(const QList<StationConfiguration>& lst);

	MessageSP correlation(quint32 point1, quint32 point2, float timediff, float veracity, QVector<QPointF> points);
	MessageSP correlationAll(const QByteArray &data);

	MessageSP detectedBandwidth(int index, QVector<QPointF> vec);

	void addPreambula(QByteArray& data);
	QByteArray getMessage(const QByteArray& input);
	MessageSP configureLoc(const QByteArray &data);

signals:
	void onChangeDevState(QMap<int, bool>);
	void onDetectSignal(int, QVector<QPointF>);
};
