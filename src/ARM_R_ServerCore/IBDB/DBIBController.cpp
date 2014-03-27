#include "DBIBController.h"

#include <QDebug>

DBIBController* DBIBController::instance = 0;

DBIBController::DBIBController()
{
}

DBIBController::~DBIBController()
{
    if(lib.isLoaded())
    {
        bool err = lib.unload();
        if(!err)
        {
            qDebug() << "Couldn't unload library";
        }
    }
}

bool DBIBController::initDBIB(QString path_to_db)
{
    _path_to_db = path_to_db;
    lib.setFileName("Nodb_api.dll");
    lib.load();
    // If the handle is not valid
    if (lib.isLoaded())
    {
        ProcAddConnect = (NODB_ConnectDatabaseFunc) lib.resolve("NODB_ConnectDatabase");
        ProcAddDisconnect = (NODB_DisconnectDatabaseFunc) lib.resolve("NODB_DisconnectDatabase");
        ProcAddGetError = (NODB_GetErrorStringFunc) lib.resolve("NODB_GetErrorString");
        ProcAddGetDBVer = (NODB_GetDatabaseVersionFunc) lib.resolve("NODB_GetDatabaseVersion");
        ProcAddGetStatistics = (NODB_GetDeliveryStatisticsFunc) lib.resolve("NODB_GetDeliveryStatistics");
        ProcAddConvertTime = (NODB_Time_t_To_TDateTimeFunc) lib.resolve("NODB_Time_t_To_TDateTime");

        ProcAddGetAircraft = (NODB_GetAircraftFunc) lib.resolve("NODB_GetAircraft");
        ProcAddGetBasestation = (NODB_GetBasestationFunc) lib.resolve("NODB_GetBasestation");
        ProcAddGetSatellite = (NODB_GetSatelliteFunc) lib.resolve("NODB_GetSatellite");
        ProcAddGetSource = (NODB_GetSourceFunc) lib.resolve("NODB_GetSource");
        ProcAddGetMeasureBaring = (NODB_GetMeasureBaringFunc) lib.resolve("NODB_GetMeasureBaring");
        ProcAddGetMeasureFixing = (NODB_GetMeasureFixingFunc) lib.resolve("NODB_GetMeasureFixing");
        ProcAddGetDeviceState = (NODB_GetDeviceStateFunc) lib.resolve("NODB_GetDeviceState");
        ProcAddGetDevice = (NODB_GetDeviceFunc) lib.resolve("NODB_GetDevice");

        ProcAddSetAircraft = (NODB_SetAircraftFunc) lib.resolve("NODB_SetAircraft");
        ProcAddSetBasestation = (NODB_SetBasestationFunc) lib.resolve("NODB_SetBasestation");
        ProcAddSetSatellite = (NODB_SetSatelliteFunc) lib.resolve("NODB_SetSatellite");
        ProcAddSetSource = (NODB_SetSourceFunc) lib.resolve("NODB_SetSource");
        ProcAddSetMeasureBaring = (NODB_SetMeasureBaringFunc) lib.resolve("NODB_SetMeasureBaring");
        ProcAddSetMeasureFixing = (NODB_SetMeasureFixingFunc) lib.resolve("NODB_SetMeasureFixing");
        ProcAddSetDeviceState = (NODB_SetDeviceStateFunc) lib.resolve("NODB_SetDeviceState");
        ProcAddSetDevice = (NODB_SetDeviceFunc) lib.resolve("NODB_SetDevice");

        ProcAddDeleteAircraft = (NODB_DeleteAircraftFunc) lib.resolve("NODB_DeleteAircraft");
        ProcAddDeleteBasestation = (NODB_DeleteBasestationFunc) lib.resolve("NODB_DeleteBasestation");
        ProcAddDeleteSatellite = (NODB_DeleteSatelliteFunc) lib.resolve("NODB_DeleteSatellite");
        ProcAddDeleteSource = (NODB_DeleteSourceFunc) lib.resolve("NODB_DeleteSource");
        ProcAddDeleteMeasureBaring = (NODB_DeleteMeasureBaringFunc) lib.resolve("NODB_DeleteMeasureBaring");
        ProcAddDeleteMeasureFixing = (NODB_DeleteMeasureFixingFunc) lib.resolve("NODB_DeleteMeasureFixing");
        ProcAddDeleteDeviceState = (NODB_DeleteDeviceStateFunc) lib.resolve("NODB_DeleteDeviceState");
        ProcAddDeleteDevice = (NODB_DeleteDeviceFunc) lib.resolve("NODB_DeleteDevice");

        ProcAddInsertAircraft = (NODB_InsertAircraftFunc) lib.resolve("NODB_InsertAircraft");
        ProcAddInsertBasestation = (NODB_InsertBasestationFunc) lib.resolve("NODB_InsertBasestation");
        ProcAddInsertSatellite = (NODB_InsertSatelliteFunc) lib.resolve("NODB_InsertSatellite");
        ProcAddInsertSource = (NODB_InsertSourceFunc) lib.resolve("NODB_InsertSource");
        ProcAddInsertMeasure = (NODB_InsertMeasureFunc) lib.resolve("NODB_InsertMeasure");
        ProcAddInsertMeasureBaring = (NODB_InsertMeasureBaringFunc) lib.resolve("NODB_InsertMeasureBaring");
        ProcAddInsertMeasureFixing = (NODB_InsertMeasureFixingFunc) lib.resolve("NODB_InsertMeasureFixing");
        ProcAddInsertDeviceState = (NODB_InsertDeviceStateFunc) lib.resolve("NODB_InsertDeviceState");
        ProcAddInsertDevice = (NODB_InsertDeviceFunc) lib.resolve("NODB_InsertDevice");
        ProcAddInsertReserve = (NODB_InsertReserveFunc) lib.resolve("NODB_InsertReserve");
        ProcAddInsertDictionary = (NODB_InsertDictionaryFunc) lib.resolve("NODB_InsertDictionary");

        ProcAddGetAllAircrafts = (NODB_GetAllAircraftsFunc) lib.resolve("NODB_GetAllAircrafts");
        ProcAddGetAllBasestations = (NODB_GetAllBasestationsFunc) lib.resolve("NODB_GetAllBasestations");
        ProcAddGetAllSatellites = (NODB_GetAllSatellitesFunc) lib.resolve("NODB_GetAllSatellites");
        ProcAddGetAllSources = (NODB_GetAllSourcesFunc) lib.resolve("NODB_GetAllSources");
        ProcAddGetAllMeasuresBaring = (NODB_GetAllMeasuresBaringFunc) lib.resolve("NODB_GetAllMeasuresBaring");
        ProcAddGetAllMeasuresFixing = (NODB_GetAllMeasuresFixingFunc) lib.resolve("NODB_GetAllMeasuresFixing");
        ProcAddGetAllDeviceStates = (NODB_GetAllDeviceStatesFunc) lib.resolve("NODB_GetAllDeviceStates");
        ProcAddGetAllDevices = (NODB_GetAllDevicesFunc) lib.resolve("NODB_GetAllDevices");
        ProcAddGetAllDictionary = (NODB_GetAllDictionaryFunc) lib.resolve("NODB_GetAllDictionary");

        if( ProcAddConnect==NULL|| ProcAddDisconnect==NULL|| ProcAddGetDBVer==NULL|| ProcAddGetError==NULL|| ProcAddGetStatistics==NULL||
                ProcAddConvertTime==NULL|| ProcAddGetAircraft==NULL|| ProcAddGetBasestation==NULL|| ProcAddGetSatellite==NULL|| ProcAddGetSource==NULL||
                ProcAddGetMeasureBaring==NULL|| ProcAddGetMeasureFixing==NULL|| ProcAddGetDeviceState==NULL|| ProcAddGetDevice==NULL||
                ProcAddSetAircraft==NULL|| ProcAddSetBasestation==NULL|| ProcAddSetSatellite==NULL|| ProcAddSetSource==NULL|| ProcAddSetMeasureBaring==NULL||
                ProcAddSetMeasureFixing==NULL|| ProcAddSetDeviceState==NULL|| ProcAddSetDevice==NULL|| ProcAddDeleteAircraft==NULL||
                ProcAddDeleteBasestation==NULL|| ProcAddDeleteSatellite==NULL|| ProcAddDeleteSource==NULL|| ProcAddDeleteMeasureBaring==NULL||
                ProcAddDeleteMeasureFixing==NULL|| ProcAddDeleteDeviceState==NULL|| ProcAddDeleteDevice==NULL|| ProcAddInsertAircraft==NULL||
                ProcAddInsertBasestation==NULL|| ProcAddInsertSatellite==NULL|| ProcAddInsertSource==NULL|| ProcAddInsertMeasure==NULL||
                ProcAddInsertMeasureBaring==NULL|| ProcAddInsertMeasureFixing==NULL|| ProcAddInsertDeviceState==NULL|| ProcAddInsertDevice==NULL||
                ProcAddInsertReserve==NULL|| ProcAddInsertDictionary==NULL|| ProcAddGetAllAircrafts==NULL|| ProcAddGetAllBasestations==NULL||
                ProcAddGetAllSatellites==NULL|| ProcAddGetAllSources==NULL|| ProcAddGetAllMeasuresBaring==NULL|| ProcAddGetAllMeasuresFixing==NULL||
                ProcAddGetAllDeviceStates==NULL|| ProcAddGetAllDevices==NULL|| ProcAddGetAllDictionary==NULL ) {
            return false;
        }else
        {
            return true;
        }
    }
    return false;
}

bool DBIBController::insertStation(QString name, QString ip)
{
    return _slot_insert_station(name, ip);
}

bool DBIBController::insertStationState(QString ip, quint16 port)
{
    int res1 = _connect_to_db();
    if(res1 != NODB_OK)	{
         _showDllError(res1);
         return false;
    }
//    _waitEmptyQueue();
    qDebug() << _id;

//    if(!_map_stations.key(ip))
//    {
//        return false;
//    }

    t_nodb_device_state *state = new t_nodb_device_state;
    memset(state, 0, sizeof(t_nodb_device_state));

    state->device_id = _map_stations.key(ip);
    state->velocity = port;

    nodb_uint64 mask = nodb_update_mask_object(0,0,0,0,1,0,0,0,0,0,0,0,0);

    nodb_result res = (ProcAddSetDeviceState)(state, mask);
    if(res != NODB_OK)
    {
        _showDllError(res);
         return false;
    }

    delete state;

    bool res2 = _disconnect_from_db();
    if(res2 != NODB_OK)
    {
        _showDllError(res2);

         return false;
    }
    return true;
}

bool DBIBController::getStation(QString ip)
{
    int res1 = _connect_to_db();
    if(res1 != NODB_OK)	{
         _showDllError(res1);
         return false;
    }
    t_nodb_device *device = new t_nodb_device;
    memset(device, 0, sizeof(t_nodb_device));
    device->id = _map_stations.key(ip);

    nodb_result res = (ProcAddGetDevice)(device);
    if(res != NODB_OK)
    {
        _showDllError(res);
         return false;
    }

    qDebug() << device->id << device->name << device->mission ;
    delete device;

    bool res2 = _disconnect_from_db();
    if(res2 != NODB_OK)
    {
        _showDllError(res2);

         return false;
    }
    return true;
}

bool DBIBController::getAllStations()
{
    int res1 = _connect_to_db();
    if(res1 != NODB_OK)	{
         _showDllError(res1);
         return false;
    }

    int user_data = 0;
    nodb_result res = (ProcAddGetAllDevices)(DEVICE_callback, &user_data);
    if(res != NODB_OK)
    {
        _showDllError(res);
         return false;
    }

    bool res2 = _disconnect_from_db();
    if(res2 != NODB_OK)
    {
        _showDllError(res2);
         return false;
    }
    return true;
}

bool DBIBController::deleteAllStations()
{
    int res1 = _connect_to_db();
    if(res1 != NODB_OK)
    {
         _showDllError(res1);
         return false;
    }
    int user_data = 0;
    nodb_result res = (ProcAddGetAllDevices)(DEVICE_callback, &user_data);
    if(res != NODB_OK)
    {
        _showDllError(res);
         return false;
    }

    qDebug() << "map count = " << _map_stations.count();
    QMap<int, QString>::iterator it;
    for(it = _map_stations.begin(); it != _map_stations.end(); it++)
    {
        qDebug() << it.key();
        res = (ProcAddDeleteDevice)(it.key());
        if(res != NODB_OK)
        {
            _showDllError(res);
             return false;
        }
    }
    _map_stations.clear();

    qDebug() << "map count = " << _map_stations.count();

    bool res2 = _disconnect_from_db();
    if(res2 != NODB_OK)
    {
        _showDllError(res2);

         return false;
    }
    return true;

}

bool DBIBController::_connect_to_db()
{
    bool updateConnection = true;
    int res = (ProcAddConnect)(L"", _path_to_db.toStdWString().c_str(), L"SYSDBA", L"masterkey" , L"C:\\123", updateConnection);
    if(res == NODB_OK)
    {
         return true;
    }
    _showDllError(res);
    return false;
}

bool DBIBController::_disconnect_from_db()
{
    int res = (ProcAddDisconnect) ();
    if(res == NODB_OK)
    {
        return true;
    }
    _showDllError(res);

    return false;
}

void DBIBController::_showDllError(nodb_result res)
{
    if (NULL == ProcAddGetError)
    {
        qDebug() << "Unknown Error";
        return;
    }
    nodb_pvarchar error = (ProcAddGetError) (res);
    QString ss;
    ss = QString::fromStdWString(error);
    qDebug() << ss;
    bool res2 = _disconnect_from_db();

    return;
}

void DBIBController::_set_station(t_nodb_device *device)
{
    _map_stations.insert(device->id, QString::fromWCharArray(device->name));
    qDebug() << "Here" << device->id << QString::fromWCharArray(device->name);
}

bool  DBIBController::_slot_insert_station(QString name, QString ip)
{
    int res1 = _connect_to_db();
    if(res1 != NODB_OK)	{
         _showDllError(res1);
         return false;
    }
    t_nodb_device *device = new t_nodb_device;
    memset(device, 0, sizeof(t_nodb_device));

//    nodb_int *id = new nodb_int;
    nodb_int id = -8;
//    device->id = 1;
    wchar_t str[] = L"127.0.0.1";
    memcpy(device->mission, str, device->COMMENTS_LEN);
    memcpy(device->name, str, device->NAME_LEN);

    nodb_result res = (ProcAddInsertDevice)(device, &_id);
    if(res != NODB_OK)
    {
        _showDllError(res);
         return false;
    }




    delete device;
//    delete id;

    bool res2 = _disconnect_from_db();
    if(res2 != NODB_OK)
    {
        _showDllError(res2);

         return false;
    }
    _waitEmptyQueue();

    return true;

}

void  DBIBController::_waitEmptyQueue()
{
    //подготавливаем структуру
        t_nodb_delivery_system ds;
        int Inserted = 1;
        int Loaded = 0;
        int errors = 0;
        QString stat = "";
        while (Inserted != Loaded+errors) {
            memset(&ds, 0, sizeof(t_nodb_delivery_system));	//очищаем структуру
            //получаем статистику системы доставки
            int res = (ProcAddGetStatistics) (&ds);
//            Check(res == NODB_OK, "Ошибка при получении статистики системы доставки");
            Inserted = ds.Inserted;
            Loaded = ds.Loaded;
            errors = ds.Errors;
            qDebug() << "not ready!";
//            Sleep(100);
        }

        qDebug() << _id;
//        Status("Файлов записано на диск: " + IntToStr(ds.Inserted) + "   Записей добавлено в БД: " + IntToStr(ds.Loaded));
//        Status("Записей добавлено в БД: " + IntToStr(ds.Loaded));
//        Status("Ошибок: " + IntToStr(ds.Errors));
//        stat = stat.sprintf(L"%d", ds.LoadSpeed);
//        Status("Скорость занесения данных в БД: " + stat);
//        stat = stat.sprintf(L"%d", ds.InsertSpeed);
//        Status("Скорость записи файлов на диск: " + stat);
}
