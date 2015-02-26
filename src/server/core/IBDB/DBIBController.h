#ifndef DBIBCONTROLLER_H
#define DBIBCONTROLLER_H

#include <QObject>
#include <QLibrary>
#include <QMap>
#include "nodb_exports.h"

class DBIBController : public QObject
{
    Q_OBJECT
public:
    static DBIBController* getInstance()
    {
        if(!instance)
        {
            instance = new DBIBController();
        }
        return instance;
    }

protected:
    static DBIBController* instance;

private:
    DBIBController();
    DBIBController(const DBIBController &root);
    DBIBController& operator=(const DBIBController&);

    ~DBIBController();

public:
    bool initDBIB(QString path);
    bool insertStation(QString name, QString ip);
    bool insertStationState(QString ip, quint16 port);
    bool getStation(QString ip);
    bool getAllStations();
    bool deleteAllStations();



private:
    bool _connect_to_db();
    bool _disconnect_from_db();
    void _showDllError(nodb_result res);
    void _set_station(t_nodb_device* device);

    void _waitEmptyQueue();



private slots:
    bool _slot_insert_station(QString name, QString ip);

private:
    QString _path_to_db;
    QMap<int, QString>   _map_stations;
    int _id;



private:
    QLibrary lib;

    NODB_ConnectDatabaseFunc ProcAddConnect;
    NODB_DisconnectDatabaseFunc ProcAddDisconnect;
    NODB_GetDatabaseVersionFunc ProcAddGetDBVer;
    NODB_GetErrorStringFunc ProcAddGetError;
    NODB_GetDeliveryStatisticsFunc ProcAddGetStatistics;
    NODB_Time_t_To_TDateTimeFunc ProcAddConvertTime;

    NODB_GetAircraftFunc ProcAddGetAircraft;
    NODB_GetBasestationFunc ProcAddGetBasestation;
    NODB_GetSatelliteFunc ProcAddGetSatellite;
    NODB_GetSourceFunc ProcAddGetSource;
    NODB_GetMeasureBaringFunc ProcAddGetMeasureBaring;
    NODB_GetMeasureFixingFunc ProcAddGetMeasureFixing;
    NODB_GetDeviceStateFunc ProcAddGetDeviceState;
    NODB_GetDeviceFunc ProcAddGetDevice;

    NODB_SetAircraftFunc  ProcAddSetAircraft;
    NODB_SetBasestationFunc ProcAddSetBasestation;
    NODB_SetSatelliteFunc ProcAddSetSatellite;
    NODB_SetSourceFunc ProcAddSetSource;
    NODB_SetMeasureBaringFunc ProcAddSetMeasureBaring;
    NODB_SetMeasureFixingFunc ProcAddSetMeasureFixing;
    NODB_SetDeviceStateFunc ProcAddSetDeviceState;
    NODB_SetDeviceFunc ProcAddSetDevice;
    NODB_SetReserveFunc ProcAddSetReserve;

    NODB_DeleteAircraftFunc  ProcAddDeleteAircraft;
    NODB_DeleteBasestationFunc ProcAddDeleteBasestation;
    NODB_DeleteSatelliteFunc ProcAddDeleteSatellite;
    NODB_DeleteSourceFunc ProcAddDeleteSource;
    NODB_DeleteMeasureBaringFunc ProcAddDeleteMeasureBaring;
    NODB_DeleteMeasureFixingFunc ProcAddDeleteMeasureFixing;
    NODB_DeleteDeviceStateFunc ProcAddDeleteDeviceState;
    NODB_DeleteDeviceFunc ProcAddDeleteDevice;

    NODB_InsertAircraftFunc  ProcAddInsertAircraft;
    NODB_InsertBasestationFunc ProcAddInsertBasestation;
    NODB_InsertSatelliteFunc ProcAddInsertSatellite;
    NODB_InsertSourceFunc ProcAddInsertSource;
    NODB_InsertMeasureFunc ProcAddInsertMeasure;
    NODB_InsertMeasureBaringFunc ProcAddInsertMeasureBaring;
    NODB_InsertMeasureFixingFunc ProcAddInsertMeasureFixing;
    NODB_InsertDeviceStateFunc ProcAddInsertDeviceState;
    NODB_InsertDeviceFunc ProcAddInsertDevice;
    NODB_InsertReserveFunc ProcAddInsertReserve;
    NODB_InsertDictionaryFunc ProcAddInsertDictionary;

    NODB_GetAllAircraftsFunc ProcAddGetAllAircrafts;
    NODB_GetAllBasestationsFunc ProcAddGetAllBasestations;
    NODB_GetAllSatellitesFunc ProcAddGetAllSatellites;
    NODB_GetAllSourcesFunc ProcAddGetAllSources;
    NODB_GetAllMeasuresBaringFunc ProcAddGetAllMeasuresBaring;
    NODB_GetAllMeasuresFixingFunc ProcAddGetAllMeasuresFixing;
    NODB_GetAllDeviceStatesFunc ProcAddGetAllDeviceStates;
    NODB_GetAllDevicesFunc ProcAddGetAllDevices;
    NODB_GetAllDictionaryFunc ProcAddGetAllDictionary;

    //callback functions
    bool __stdcall Air_callback (t_nodb_aircraft *air, void* user_data)
    {
    return true;
    }

    bool __stdcall BS_callback (t_nodb_basestation *bs, void* user_data)
    {
    return true;
    }

    bool __stdcall SAT_callback (t_nodb_satellite *sat, void* user_data)
    {
    return true;
    }

    bool __stdcall SRC_callback (t_nodb_source *src, void* user_data)
    {
    return true;
    }

    bool __stdcall BAR_callback (t_nodb_measure_baring *baring, void* user_data)
    {
    return true;
    }

    bool __stdcall FIX_callback (t_nodb_measure_fixing *fixing, void* user_data)
    {
    return true;
    }

    bool __stdcall STATE_callback (t_nodb_device_state *state, void* user_data)
    {
    return true;
    }

    static bool __stdcall DEVICE_callback (t_nodb_device *device, void* user_data)
    {
        DBIBController *db = getInstance();
        db->_set_station(device);
    return true;
    }

    bool __stdcall RESERVE_callback (t_nodb_reserved *reserve, void* user_data)
    {
    return true;
    }

    bool __stdcall DICT_callback (t_nodb_dictionary *dict, void* user_data)
    {
    return true;
    }


};

#endif // DBIBCONTROLLER_H
