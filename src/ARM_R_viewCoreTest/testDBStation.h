#ifndef TESTDBSTATION_H
#define TESTDBSTATION_H

#include <cxxtest/TestSuite.h>
#include <QtGlobal>

#include "DBStation/DBStationController.h"

class dbStationTest: public CxxTest::TestSuite
{

public:
	void testAddStation()
	{
		DBStationController* stationController = new DBStationController(NULL);
		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "root";
		param.port = 3306;
		param.password = "qwerty12345";
		bool err = stationController->connectToDB(param);
		TS_ASSERT_EQUALS(true, err);
		int lastID = stationController->getLastIndex("station");
		TS_ASSERT_DIFFERS(0, ++lastID);
		qDebug() << lastID;

		QString stationName("Station");
		stationName.append(QString::number(lastID));
		int index = stationController->addStation(stationName, "192.168.1.1");

		qDebug() << index;

		TS_ASSERT_DIFFERS(0, index);

		delete stationController;
	}
	void testAddStationDevice()
	{
		DBStationController* stationController = new DBStationController(NULL);
		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "root";
		param.port = 3306;
		param.password = "qwerty12345";
		bool err = stationController->connectToDB(param);
		TS_ASSERT_EQUALS(true, err);
		int lastID = stationController->getLastIndex("station");
		TS_ASSERT_DIFFERS(0, lastID);
		qDebug() << lastID;

		QString stationName("Station");
		stationName.append(QString::number(lastID));
		int index = stationController->addStationDevice(stationName, 1024);

		qDebug() << index;

		TS_ASSERT_DIFFERS(0, index);

		delete stationController;
	}

	void testAddSignalType()
	{
		DBStationController* stationController = new DBStationController(NULL);
		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "root";
		param.port = 3306;
		param.password = "qwerty12345";
		bool err = stationController->connectToDB(param);
		TS_ASSERT_EQUALS(true, err);
		int lastID = stationController->getLastIndex("signalType");
		TS_ASSERT_DIFFERS(0, ++lastID);
		qDebug() << lastID;

		QString signalName("Signal");
		signalName.append(QString::number(lastID));
		int index = stationController->addSignalType(signalName);

		qDebug() << index;

		TS_ASSERT_DIFFERS(0, index);

		delete stationController;
	}

	void testAddCategory()
	{
		DBStationController* stationController = new DBStationController(NULL);
		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "root";
		param.port = 3306;
		param.password = "qwerty12345";
		bool err = stationController->connectToDB(param);
		TS_ASSERT_EQUALS(true, err);
		int lastID = stationController->getLastIndex("category");

		qDebug() << lastID;

		if(lastID == 2)
		{
			delete stationController;
			return;
		}

		QString categoryName("White");
		int index = stationController->addCategory(categoryName);

		TS_ASSERT_EQUALS(1, index);

		categoryName = "Black";
		index = stationController->addCategory(categoryName);

		qDebug() << index;

		TS_ASSERT_EQUALS(2, index);


		delete stationController;
	}

	void testAddStationData()
	{
		DBStationController* stationController = new DBStationController(NULL);
		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "root";
		param.port = 3306;
		param.password = "qwerty12345";
		bool err = stationController->connectToDB(param);
		TS_ASSERT_EQUALS(true, err);
		int lastID = stationController->getLastIndex("stationData");

		TS_ASSERT_DIFFERS(0, ++lastID);
		StationData data;
		data.stationName = "Station1";
		data.port = 1024;
		data.category = "White";
		data.signalType = "Signal1";
		data.frequency = 1456;
		data.bandwidth= 1000;

		int index = stationController->addStationData(data);

		qDebug() << index;

		TS_ASSERT_DIFFERS(0, index);


		delete stationController;
	}

	void testGetStationInfo()
	{
		DBStationController* stationController = new DBStationController(NULL);
		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "root";
		param.port = 3306;
		param.password = "qwerty12345";
		bool err = stationController->connectToDB(param);
		TS_ASSERT_EQUALS(true, err);

		QList<StationDataFull> list;

		err = stationController->getStationInfo("Station1", list);

		qDebug() << err;

		TS_ASSERT_EQUALS(true, err);
		TS_ASSERT_DIFFERS(0, list.count());



		delete stationController;
	}
};

#endif // TESTDBSTATION_H
