#ifndef TESTDBSTATION_H
#define TESTDBSTATION_H

#include <cxxtest/TestSuite.h>
#include <QtGlobal>

#include "DBStation/DBStationController.h"

class DBStationControllerTest: public CxxTest::TestSuite
{

	DBStationController* stationController;
	bool isConnected;

public:
	void setUp()
	{
		stationController = new DBStationController( NULL );

		DBConnectionStruct param;
		param.dbName = "testStations";
		param.host = "127.0.0.1";
		param.login = "uavtest";
		param.port = 3306;
		param.password = "uavtest";

		isConnected = stationController->connectToDB( param );

		stationController->m_db.transaction();
	}

	void tearDown()
	{
		stationController->m_db.rollback();
		delete stationController;
	}

	void testAddStation()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		int index = stationController->addStation( "Test station", "192.168.1.1" );

		TS_ASSERT_DIFFERS( 0, index );
	}

	void testAddStationDevice()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		stationController->addStation( "Test station", "192.168.1.1" );
		int index = stationController->addStationDevice( "Test station", 1024 );

		TS_ASSERT_DIFFERS( 0, index );
	}

	void testAddSignalType()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		int index = stationController->addSignalType( "Test signal" );

		TS_ASSERT_DIFFERS( 0, index );
	}

	void testAddCategory()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		int index = stationController->addCategory( "Test category" );

		TS_ASSERT_DIFFERS( 0, index );
	}

	void testAddStationData()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		stationController->addStation( "Test station", "192.168.1.1" );
		stationController->addStationDevice( "Test station", 1024 );
		stationController->addSignalType( "Test signal" );
		stationController->addCategory( "Test category" );

		StationData data;
		data.stationName = "Test station";
		data.port = 1024;
		data.category = "Test category";
		data.signalType = "Test signal";
		data.frequency = 1456;
		data.bandwidth= 1000;

		int index = stationController->addStationData( data );

		TS_ASSERT_DIFFERS( 0, index );
	}

	void testGetStationInfo()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		QList<StationDataFull> list;
		bool isGetOk;

		list.clear();
		isGetOk = stationController->getStationInfo( "Test station", list );
		TS_ASSERT_EQUALS( true, isGetOk );
		TS_ASSERT_EQUALS( 0, list.count() );

		stationController->addStation( "Test station", "192.168.1.1" );
		stationController->addStationDevice( "Test station", 1024 );
		stationController->addSignalType( "Test signal" );
		stationController->addCategory( "Test category" );

		list.clear();
		isGetOk = stationController->getStationInfo( "Test station", list );
		TS_ASSERT_EQUALS( true, isGetOk );
		TS_ASSERT_EQUALS( 0, list.count() );

		StationData data;
		data.stationName = "Test station";
		data.port = 1024;
		data.category = "Test category";
		data.signalType = "Test signal";

		data.frequency = 1456;
		data.bandwidth= 1000;
		stationController->addStationData( data );

		data.frequency = 734;
		data.bandwidth= 200;
		stationController->addStationData( data );

		list.clear();
		isGetOk = stationController->getStationInfo( "Test station", list );
		TS_ASSERT_EQUALS( true, isGetOk );
		TS_ASSERT_EQUALS( 2, list.count() );
	}

	void testGetFrequencyAndBandwidth()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		QList<StationsFrequencyAndBandwith> list;
		bool isGetOk;

		list.clear();
		isGetOk = stationController->getFrequencyAndBandwidthByCategory( "Test category", list );
		TS_ASSERT_EQUALS( true, isGetOk );
		TS_ASSERT_EQUALS( 0, list.count() );

		stationController->addStation( "Test station", "192.168.1.1" );
		stationController->addStationDevice( "Test station", 1024 );
		stationController->addSignalType( "Test signal" );
		stationController->addCategory( "Test category" );

		list.clear();
		isGetOk = stationController->getFrequencyAndBandwidthByCategory( "Test category", list );
		TS_ASSERT_EQUALS( true, isGetOk );
		TS_ASSERT_EQUALS( 0, list.count() );

		StationData data;
		data.stationName = "Test station";
		data.port = 1024;
		data.category = "Test category";
		data.signalType = "Test signal";

		data.frequency = 1456;
		data.bandwidth= 1000;
		stationController->addStationData( data );

		data.frequency = 734;
		data.bandwidth= 200;
		stationController->addStationData( data );

		data.frequency = 158;
		data.bandwidth= 100;
		data.category = "Test category 2";
		stationController->addStationData( data );

		list.clear();
		isGetOk = stationController->getFrequencyAndBandwidthByCategory( "Test category", list );
		TS_ASSERT_EQUALS( true, isGetOk );
		TS_ASSERT_EQUALS( 2, list.count() );
	}
};

#endif // TESTDBSTATION_H
