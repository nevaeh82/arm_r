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

#include "Interfaces/ICoordinateCounter.h"
#include "../TCP/Interfaces/ITcpListener.h"
#include "BaseSubject.h"
#include "TcpDevicesDefines.h"
#include "../TCP/TcpDefines.h"
#include "Message.h"

#include "RadiolocationInterface.h"
#include "Solver.h"
#include "Solver_global.h"

#include "Correction/ZDR.h"

#include "Protobuf/ARMR_OD/ZVPacket.pb.h"

class CoordinateCounter : public QObject, public ITcpListener, public BaseSubject<ITcpListener>, public ICoordinateCounter
{
	Q_OBJECT

private:
	QMap<int, QVector<QPointF> > m_map_vec_corr;
	int m_main_point;
	double m_corr_threshold;
	int m_prevStation;

	Solver* m_solver;

	double m_alt;
	DataFromFlacon m_aData;

	QString m_likeADeviceName;

public:
	explicit CoordinateCounter(const QString& deviceName, QObject* parent = NULL);
	virtual ~CoordinateCounter();

	// ITcpListener interface
public:
	virtual void onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument);

	// ICoordinateCounter interface
public:
	virtual void sendData(const MessageSP message);
	virtual QObject* asQObject();

private:
	/// Solver dll
	void setSolverDataSize(int aSize);
	void setSolverAnalyzeSize(int aSize);

public slots:
	void initSolver();

private slots:
	void slotCatchDataFromRadioLocationAuto(const DataFromRadioLocation&);
    void slotCatchDataFromRadioLocationManual(const DataFromRadioLocation&);
    void slotOneCatchDataFromRadioLocationManual(const OneDataFromRadioLocation&);


signals:
	void signalFinished();
};

#endif // COORDINATECOUNTER_H
