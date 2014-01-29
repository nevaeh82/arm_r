#ifndef TESTSTREAMING_H
#define TESTSTREAMING_H
#include <cxxtest/TestSuite.h>
//#include "../ARM_R_view/Tabs/Tree/DBController.h"
//#include "../ARM_R_view/Tabs/Tree/DBController.cpp"

#include "GraphicData.h"

class GraphicsDataTest: public CxxTest::TestSuite
{

public:
    void test1()
    {
//        DBController* db_controller = new DBController();
//        QMap<QString, QVariant>* data = new QMap<QString, QVariant>;
//        data->insert("id", "1");
//        data->insert("pid", "1");
//        data->insert("name", "name");
//        data->insert("value", "2");
//        data->insert("state", 0);
//        data->insert("editable", "true");
//        db_controller->set_data(data);
        TS_ASSERT_EQUALS(1, 1);/* expected, receiver.data);*/
    }

//	GraphicsDataTest()
//	{
//	}

	void testGraphicsList()
	{
		GraphicData* grData = new GraphicData(0,0,0,0);
		grData->_list_startx.append(1.23);
		grData->_list_startx.append(8.97);

		qDebug() << grData->_list_startx.size() << grData->_list_startx.at(1);

		TS_ASSERT_EQUALS(1, grData->_find_index(8.97));

		TS_ASSERT_EQUALS(1, grData->_find_index(8));

		TS_ASSERT_EQUALS(0, grData->_find_index(0.99));
		delete grData;
	}


};

#endif // TESTSTREAMING_H
