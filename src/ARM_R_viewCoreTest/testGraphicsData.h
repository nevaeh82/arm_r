#ifndef TESTSTREAMING_H
#define TESTSTREAMING_H
#include <cxxtest/TestSuite.h>
#include <QtGlobal>
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

	void testGraphicsIncomeData()
	{
		GraphicData* grData = new GraphicData(0,0,0,0);

		QVector<QPointF> vecFFT1;
		bool isComplex = true;
		int N = 100;
		int K = 99;
		for(int i = 0; i < 21; i++)
		{
			QPointF point;
			point.setX(i*1000);
			qreal val =  (qrand() % N - K);
			qDebug() << val;
			point.setY(val);
			vecFFT1.append(point);
			qDebug() << point.x() << point.y();
		}


		grData->m_dataProccess(vecFFT1, isComplex);

		grData->m_slotPanoramaStart(0, 40);

		grData->m_dataProccess(vecFFT1, isComplex);


		QVector<QPointF> vecFFT2;
		for(int i = 21; i < 41; i++)
		{
			QPointF point;
			point.setX(i*1000);
			qreal val =  (qrand() % N - K);
			point.setY(val);
			vecFFT2.append(point);
		}

		grData->m_dataProccess(vecFFT2, isComplex);

		for(int i = 0; i < 40; i++)
		{
			if(i < 20)
			{
				qDebug() << i << vecFFT1.at(i).y() << grData->_spectrum[i];
				TS_ASSERT_EQUALS(vecFFT1.at(i).y(), grData->_spectrum[i]);
			}
			else
			{
				qDebug() << i << vecFFT2.at(i-20).y() << grData->_spectrum[i];
				TS_ASSERT_EQUALS(vecFFT2.at(i-20).y(), grData->_spectrum[i]);
			}

		}

		TS_ASSERT_EQUALS(0, 0);
		delete grData;

	}
};

#endif // TESTSTREAMING_H
