#include "DBIBController.h"

#include <QDebug>

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

bool DBIBController::initDBIB()
{
    lib.setFileName("Nodb_api.dll");
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
        }else{
            return true;
        }
    }
    return false;
}

bool DBIBController::_connect_to_db()
{
    bool updateConnection = true;
    int res = (ProcAddConnect)(L"", L"D:\\DB\\NOC2_EMPTY.IB", L"SYSDBA", L"masterkey" , L"C:\\123", updateConnection);
    if(res == NODB_OK)	{
         return true;
    }
    _showDllError(res);
    return false;
}

bool DBIBController::_disconnect_from_db()
{
    int res = (ProcAddDisconnect) ();
    if(res != NODB_OK)
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
    return;
}
