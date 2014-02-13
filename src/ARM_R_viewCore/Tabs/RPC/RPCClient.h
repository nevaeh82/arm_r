#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include "IRPC.h"
#include "../../Common/IMessage.h"
#include "../../Common/CommandMessage.h"

#include "../TabsProperty.h"
#include "Interfaces/IDBManager.h"
#include "../ITabSpectrum.h"

#include "../../GraphicData.h"

#include "../Controls/IControlPRM.h"

#include "RpcDefines.h"


typedef QVector<QPointF>         rpc_send_points_vector;


class RPCClient : public QObject, public IRPC
{
    Q_OBJECT
public:
	RPCClient(TabsProperty *prop, IDbManager *db_manager, ITabSpectrum *parent_tab, GraphicData *gr_data, IControlPRM* control_prm);
    ~RPCClient();


private slots:
    virtual int start();
    virtual int stop();

public:
    void set_command(IMessage* msg);

private slots:
    void _slotSetCommand(IMessage* msg);

private:
    IControlPRM*        _control_prm;
    QxtRPCPeer*         _rpc_client;
    QString             _ip_RPC;
    quint16             _port_RPC;
    IMessage*           _command_msg;
	IDbManager*         _db_manager;
    TabsProperty*       _tab_property;
    ITabSpectrum*       _parent_tab;
    GraphicData*        _gr_data;

private:
//    int     _init();

	bool readSettings(const QString& settingsFile);
    void    _form_command(IMessage *msg);
    void    _recognize();
    void    _ss_correlation(bool enable);

    void    _prm_set_freq(short freq);
    void    _prm_request_freq();
    void    _prm_set_att1(int att1);
    void    _prm_set_att2(int att2);
    void    _prm_set_filter(int index);

    void    _flakon_set_main_station_cor(int value);
    void    _flakon_set_avarage(int value);

    void    _request_satatus();


private slots:
    void _close();
    void _slotRCPConnetion();
    void _slotRPCDisconnection();
    void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
    void _slotReconnection();

signals:
    void signalSetCommand(IMessage *msg);
    void signalFinished();

    void signalStart();
    void signalStop();
    void signalFinishRPC();

    ///RPC signals
signals:
    void signalSetClientId(int id);
    void signalSetMainStationCor(int id, int station);
    void signalSetBandwidth(int id, float db);
    void signalSetShift(int id, float shift);
    void signalRecognize(int id, int type);
    void signalSSCorrelation(int id, bool enable);
    void signalSetAvarageSpectrum(int id, int avarage);

    void signalPRMSetFreq(int id, short freq);
    void signalPRMRequestFreq(int id);
    void signalPRMSetAtt1(int id, int att1);
    void signalPRMSetAtt2(int id, int att2);
    void signalPRMSetFilter(int id, int index);

    void signalRequestStatus(int id);


    void signalReconnection();

    void signalDataS(float*,float*);
    void signalData(float*,float*);

public slots:
    void slotInit();
    void slotStart();
    void slotStop();
    void slotFinish();

    ///rpc_server
    void rpc_slot_getting_points(rpc_send_points_vector points);
	void rpc_slotGettingDetectedBandwidth(rpc_send_points_vector points);
    void rpc_slot_getting_modulation(QString modulation);
    void rpc_slot_server_send_correlation(int point1, int point2, rpc_send_points_vector points);

    void rpc_slot_server_prm_status(int prm_freq, int prm_filter, int prm_att1, int prm_att2);

    void rpc_slot_server_status(bool state);


private:
    float*                  _spectrum;
    float*                  _spectrum_peak_hold;
    double                  _bandwidth;
    qreal                   _startx;
    bool                    _needSetup;




};

#endif // RPCCLIENT_H
