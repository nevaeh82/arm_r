#ifndef TESTDBIB_H
#define TESTDBIB_H



#include <cxxtest/TestSuite.h>
#include <IBDB/DBIBController.h>
#include <QDebug>

class testDBIB : public CxxTest::TestSuite
{
public:
    void test1()
    {
        TS_ASSERT_EQUALS(true, true);
    }

//    void testInitDB()
//    {
//        QString path_to_db = "test/NOC2_TEST.IB";
//        DBIBController *db = DBIBController::getInstance();
//        bool err = db->initDBIB(path_to_db);
//        qDebug() << "Init db  = " << err;

//        TS_ASSERT_EQUALS(true, err);

////        delete db;
//    }

////    void testConnectToDB()
////    {
////        QString path_to_db = "test/NOC2_TEST.IB";
////        DBIBController *db = new DBIBController();
////        bool err = db->initDBIB(path_to_db);
////        qDebug() << "Connect to db  = " << err;


////        err = db->_connect_to_db();
////        TS_ASSERT_EQUALS(true, err);

////        if(err)
////        {
////            qDebug() << db->_disconnect_from_db();
////        }

////        delete db;
////    }

//    void testInsertDeviceToDB()
//    {
//        QString path_to_db = "test/NOC2_TEST.IB";
//        DBIBController *db = DBIBController::getInstance();
//        bool err = db->initDBIB(path_to_db);
//        qDebug() << "Connect to db  = " << err;

//        QString name = "First";
//        QString ip = "127.0.0.1";
//        err = db->insertStation(name, ip);

//        qDebug() << err;

//        TS_ASSERT_EQUALS(true, err);

////        delete db;
//    }

//    void testInsertDeviceStateToDB()
//    {
//        QString path_to_db = "test/NOC2_TEST.IB";
//        DBIBController *db = DBIBController::getInstance();
//        bool err = db->initDBIB(path_to_db);
//        qDebug() << "Connect to db  = " << err;

//        quint16 port = 1024;
//        QString ip = "127.0.0.1";
//        err = db->insertStationState(ip, port);

//        qDebug() << err;

//        TS_ASSERT_EQUALS(true, true);

////        delete db;
//    }

//    void testGetAllDeviceFromDB()
//    {
//        QString path_to_db = "test/NOC2_TEST.IB";
//        DBIBController *db = DBIBController::getInstance();
//        bool err = db->initDBIB(path_to_db);
//        qDebug() << "Connect to db  = " << err;

//        err = db->getAllStations();

//        qDebug() << err;

//        TS_ASSERT_EQUALS(true, err);

////        delete db;

//    }

//    void testGetDeviceFromDB()
//    {
//        QString path_to_db = "test/NOC2_TEST.IB";
//        DBIBController *db = DBIBController::getInstance();
//        bool err = db->initDBIB(path_to_db);
//        qDebug() << "Connect to db  = " << err;

//        err = db->getStation("127.0.0.1");

//        qDebug() << err;

////        TS_ASSERT_EQUALS(true, err);

////        delete db;

//    }



//    void testDeleteAllDevicesFromDB()
//    {
//        QString path_to_db = "test/NOC2_TEST.IB";
//        DBIBController *db = DBIBController::getInstance();
//        bool err = db->initDBIB(path_to_db);
//        qDebug() << "Connect to db  = " << err;

//        err = db->deleteAllStations();

//        qDebug() << "last" << err;

//        TS_ASSERT_EQUALS(true, err);

////        delete db;

//    }

}
;

#endif // TESTDBIB_H
