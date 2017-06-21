#ifndef COORDINATECOUNTER_H
#define COORDINATECOUNTER_H

#include <QObject>
#include <QMap>
#include <QPointF>
#include <QDataStream>
#include <QVector>
#include <QPointF>
#include <QTime>
#include <QMetaType>
#include <QThread>
#include <QFile>
#include <QDir>

#include <logManager/LogManager.h>
#include <Templates/BaseSubject.h>
#include <Templates/Message.h>

#include "Interfaces/ICoordinateCounter.h"
#include "../TCP/Interfaces/ITcpListener.h"
#include "TcpDevicesDefines.h"
#include "../TCP/TcpDefines.h"

#include <RadiolocationInterface.h>
#include <Solver.h>
#include <Solver_global.h>
#include <ISolver.h>
#include <ISolverListener.h>

#include "Correction/ZDR.h"

#include "ZVPacket.pb.h"
#include "Server/Structs.h"

#include "Server/ClientServer.h"
#include "ProtoTypes1.pb.h"

// Copy/paste from ARM OD UavDefines.h
//typedef struct UAVPositionDataEnemy
//{
//	UAVPositionDataEnemy()
//	{
//		altitude	= 0.0;
//		altitudeStdDev = 0.0;
//		speed		= 0.0;
//		course		= 0.0;
//		state		= 0;
//		frequency	= -1;
//		time		= QTime::currentTime();
//		latLonStdDev	= QPointF(0.0, 0.0);
//		sourceType  = 0;
//	}

//	UAVPositionDataEnemy(const UAVPositionDataEnemy& object)
//	{
//		altitude	= object.altitude;
//		altitudeStdDev = object.altitudeStdDev;
//		speed		= object.speed;
//		course		= object.course;
//		state		= object.state;
//		frequency	= object.frequency;
//		time		= object.time;
//		latLonStdDev	= object.latLonStdDev;
//		latLon		= object.latLon;
//		sourceType  = object.sourceType;
//	}

//	double		altitude;
//	double		altitudeStdDev;
//	double		speed;
//	double		course;
//	int			state;
//	double		frequency;
//	QTime		time;
//	QPointF		latLonStdDev;
//	QPointF		latLon;
//	uint		sourceType;

//} UAVPositionDataEnemy;

class CoordinateCounter : public QObject, public ITcpListener, public BaseSubject<ITcpListener>, public ICoordinateCounter, public ISolverListener
{
	Q_OBJECT

private:
	QMap<int, QVector<QPointF> > m_map_vec_corr;
	int m_main_point;
	double m_corr_threshold;
	int m_prevStation;

	bool isInit;
	QMap<int, double> m_rangesMap;


	ISolver* m_solver;

	double m_alt;
	DataFromFlaconInt m_aData;

	int countChanNum;

	double m_centerFrequency;
    double m_shift;
    int m_stationsShift;

	QString m_likeADeviceName;

	struct PositionLocal{
		double lat;
		double lon;
		int alt;
	};

//	LogManager* m_logManager;
//	LogManager* m_logManager1;
//	LogManager* m_logManager2;
//	LogManager* m_logManager3;

	QTime m_resTime;

public:
	explicit CoordinateCounter(const QString& deviceName, QObject* parent = NULL);
	virtual ~CoordinateCounter();

	virtual void onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument);

	void onSendDataFromRadioLocation(const SolveResult& result, const DataFromRadioLocation& allData);
	void onSendDataFromRadioLocationManualHeigh(const SolveResult& result, const DataFromRadioLocation& allData);
	void onSendOneDataFromRadioLocation(const SolveResult& result, const OneDataFromRadioLocation& oneData_1, const OneDataFromRadioLocation& oneData_2);
	void onSendHyperbolesFromRadioLocation(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb);
	void onErrorOccured(const ErrorType& error_type, const QString& str);

	void onSolver1ProtoData(const int &result, const QByteArray& data );

    void onSolverBlaData(const QByteArray &data);
    void onSolverWorkData(const QByteArray &data);

	void onSolver1SetupAnswer(const QByteArray &data);

signals:
	void signalGetDataFromRadioLocation(const SolveResult& , const DataFromRadioLocation&);
	void signalGetDataFromRadioLocationManualHeight(const SolveResult& , const DataFromRadioLocation&);
	void signalGetOneDataFromRadioLocation(const SolveResult&, const OneDataFromRadioLocation&, const OneDataFromRadioLocation&);
	void signalGetHyperbolesDataFromRadioLocation(const SolveResult&, const HyperbolesFromRadioLocation&);
	void signalError(int, QString);

	void signal1ProtoData( int, QByteArray data );
    void signal1BlaData( QByteArray data );
	void signal1SetupAnswer( QByteArray data );


	// ICoordinateCounter interface
public:
	virtual void sendData(const MessageSP message);
	virtual QObject* asQObject();
	virtual void setCenterFrequency(const double &frequency);
    void setShift( const double shift );

    void setStationsShift(const int val);

private:
	/// Solver dll
	void setSolverDataSize(int aSize);
	void setSolverAnalyzeSize(int aSize);

	QList<UAVPositionDataEnemy> encodeSolverData(const DataFromRadioLocation& data, bool useCommonAlt = false);
	UAVPositionDataEnemy encodeSolverData(const OneDataFromRadioLocation& data);

	void sendDataToClientTcpServer();
	void sendDataToClientTcpServer1(const SolverProtocol::Packet &data);
	void sendRawDataToClientTcpServer1(const QByteArray &data);

	bool getIsSolverServer();
	uint getSolverVersion();

public slots:
	void initSolver();

private slots:
	void slotCatchDataFromRadioLocationAuto(const SolveResult& result, const DataFromRadioLocation& aData);
	void slotCatchDataFromRadioLocationManual(const SolveResult& result, const DataFromRadioLocation& aData);
	void slotOneCatchDataFromRadioLocationManual(const SolveResult& result, const OneDataFromRadioLocation& aData_1, const OneDataFromRadioLocation& aData_2);
	void slotCatchDataHyperbolesFromRadioLocation(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb);
	void slotErrorOccured(int error_type, QString str);
	void slotSetCenterFrequency(const double& frequency);
    void slotSetShift(const double shift);

	void slotSolver1ProtoData( int result, QByteArray data );
	void slotSolver1SetupAnswer(QByteArray data);
    void slotSolverBlaData( QByteArray data );

signals:
	void signalFinished();
	void signalSetCenterFrequency(const double& frequency);
    void signalSetShift(const double shift);
};

inline QDataStream& operator<<(QDataStream& out, const UAVPositionDataEnemy& object)
{
	return out << object.altitude << object.altitudeStdDev << object.speed
			   << object.course << object.state << object.frequency
			   << object.time << object.latLonStdDev
			   << object.latLon;
}

//inline QDataStream& operator<<(QDataStream& out, const DataFromFlacon& object)
//{
//	return out << object.numOfReferenceDetector_ << object.time_ << object.ranges_;
//}
#endif // COORDINATECOUNTER_H
