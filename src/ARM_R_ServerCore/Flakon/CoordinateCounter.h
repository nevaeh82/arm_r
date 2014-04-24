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

#include <QFile>

#include "Interfaces/ICoordinateCounter.h"
#include "../TCP/Interfaces/ITcpListener.h"
#include "BaseSubject.h"
#include "TcpDevicesDefines.h"
#include "../TCP/TcpDefines.h"
#include "Message.h"

#include "RadiolocationInterface.h"
#include "Solver.h"
#include "Solver_global.h"
#include "ISolver.h"
#include "ISolverListener.h"

#include "Correction/ZDR.h"

#include "Protobuf/ARMR_OD/ZVPacket.pb.h"


class CoordinateCounter : public QObject, public ITcpListener, public BaseSubject<ITcpListener>, public ICoordinateCounter, public ISolverListener
{
	Q_OBJECT

private:
	QMap<int, QVector<QPointF> > m_map_vec_corr;
	int m_main_point;
	double m_corr_threshold;
	int m_prevStation;
	double m_centerFrequency;

	ISolver* m_solver;

	double m_alt;
	DataFromFlacon m_aData;

	QString m_likeADeviceName;

	struct PositionLocal{
		double lat;
		double lon;
		int alt;
	};

    QFile* fi;
    QFile* fi1;
    QFile* fi2;
    QFile* fi3;

public:
	explicit CoordinateCounter(const QString& deviceName, QObject* parent = NULL);
	virtual ~CoordinateCounter();

	virtual void onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument);

	void onSendDataFromRadioLocation(const SolveResult& result, const DataFromRadioLocation& allData);
	void onSendDataFromRadioLocationManualHeigh(const SolveResult& result, const DataFromRadioLocation& allData);
	void onSendOneDataFromRadioLocation(const SolveResult& result, const OneDataFromRadioLocation& oneData_1, const OneDataFromRadioLocation& oneData_2);
	void onSendHyperbolesFromRadioLocation(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb);
	void onErrorOccured(const ErrorType& error_type, const QString& str);

signals:
	void signalGetDataFromRadioLocation(const SolveResult& , const DataFromRadioLocation&);
	void signalGetDataFromRadioLocationManualHeight(const SolveResult& , const DataFromRadioLocation&);
	void signalGetOneDataFromRadioLocation(const SolveResult&, const OneDataFromRadioLocation&, const OneDataFromRadioLocation&);
	void signalGetHyperbolesDataFromRadioLocation(const SolveResult&, const HyperbolesFromRadioLocation&);
	void signalError(const ErrorType&, const QString&);


	// ICoordinateCounter interface
public:
	virtual void sendData(const MessageSP message);
	virtual QObject* asQObject();
	virtual void setCenterFrequency(const double &frequency);

private:
	/// Solver dll
	void setSolverDataSize(int aSize);
	void setSolverAnalyzeSize(int aSize);

public slots:
	void initSolver();

private slots:
	void slotCatchDataFromRadioLocationAuto(const SolveResult& result, const DataFromRadioLocation& aData);
	void slotCatchDataFromRadioLocationManual(const SolveResult& result, const DataFromRadioLocation& aData);
	void slotOneCatchDataFromRadioLocationManual(const SolveResult& result, const OneDataFromRadioLocation& aData_1, const OneDataFromRadioLocation& aData_2);
	void slotCatchDataHyperbolesFromRadioLocation(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb);
	void slotErrorOccured(const ErrorType& error_type, const QString& str);
	void slotSetCenterFrequency(const double& frequency);

signals:
	void signalFinished();
	void signalSetCenterFrequency(const double& frequency);
};

#endif // COORDINATECOUNTER_H
