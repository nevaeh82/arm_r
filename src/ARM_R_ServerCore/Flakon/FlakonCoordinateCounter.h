#ifndef FLAKONCOORDINATECOUNTER_H
#define FLAKONCOORDINATECOUNTER_H

#include <QObject>
#include <QMap>
#include <QPointF>
#include <QDataStream>
#include <QVector>
#include <QPointF>
#include <QTime>
#include <QMetaType>
#include "../common/IClient.h"
#include "../Common/IMessage.h"
#include "../Common/ISubscriber.h"

#include "../Common/IRouter.h"

#include "../RPC/Message.h"

#include "StationProperty.h"

#include "RadiolocationInterface.h"
#include "Solver.h"
#include "Solver_global.h"

#include "Correction/ZDR.h"

//typedef DataFromRadioLocation    DataFromRadioLocation;
//typedef DataFromFlacon    DataFromFlacon;

typedef QSharedPointer<IMessage> rpc_flakon_msg;


class FlakonCoordinateCounter : public QObject, public IClient
{
    Q_OBJECT
public:
    FlakonCoordinateCounter(IRouter* router);
    ~FlakonCoordinateCounter();

public:
    virtual void set_id(int id);
    virtual int get_id();
    virtual void set_type(int type);
    virtual int get_type();
    virtual void send_data(QSharedPointer<IMessage> msg_ptr);

private slots:
    void _slotGetData(rpc_flakon_msg msg_ptr);

    /// Solver
//    void _slotGetDataFromSolver(const DataFromRadioLocation&);
    void _slotCatchDataFromRadioLocationAuto(const DataFromRadioLocation&);
    void _slotCatchDataFromRadioLocationManual(const DataFromRadioLocation&);

private:
    void _find_max(int main_point, int second_point);
    void _checkData();

    /// Solver dll
    void _setSolverDataSize(int aSize);
    void _setSolverAnalyzeSize(int aSize);

public slots:
    void initSolver();


private:
    int                     _id;
    int                     _type;
    IRouter*                _router;
    ISubscriber*            _subscriber;
    QMap<int, double>       _map_maxs;
    QMap<int, QVector<QPointF> > _map_vec_corr;
    QMap<int, double>       _map_benchmarks;
    QMap<int, int>          _map_main_stations;
    int                     _main_point;
    double                  _corr_threshold;
    int                     _prev_station;

    Solver*                 _solver;
    int                     _count_corr;

    double                  _alt;

    DataFromFlacon _aData;

signals:
    void signalSendDataFromFlackon(const DataFromFlacon&);

    void signalData(rpc_flakon_msg msg_ptr);

    void signalFinished();
};

Q_DECLARE_METATYPE(DataFromFlacon)

//Q_DECLARE_METATYPE(DataFromRadioLocation)


#endif // FLAKONCOORDINATECOUNTER_H
