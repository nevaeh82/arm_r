#ifndef TESTSTREAMING_H
#define TESTSTREAMING_H
#include <cxxtest/TestSuite.h>
#include <QtGlobal>
//#include "../ARM_R_view/Tabs/Tree/DBController.h"
//#include "../ARM_R_view/Tabs/Tree/DBController.cpp"

#include "SpectrumWidgetDataSource.h"

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
		SpectrumWidgetDataSource* grData = new SpectrumWidgetDataSource(NULL, NULL);
		grData->m_listStartx.append(1.23);
		grData->m_listStartx.append(8.97);

		qDebug() << grData->m_listStartx.size() << grData->m_listStartx.at(1);

		TS_ASSERT_EQUALS(1, grData->findIndex(8.97));
		TS_ASSERT_EQUALS(1, grData->findIndex(8));
		TS_ASSERT_EQUALS(0, grData->findIndex(0.99));

		delete grData;
	}

	void testGraphicsIncomeData()
	{
		SpectrumWidgetDataSource* grData = new SpectrumWidgetDataSource(NULL, NULL);

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


		grData->dataProccess(vecFFT1, isComplex);

		grData->setPanorama(true, 0, 40);

		grData->dataProccess(vecFFT1, isComplex);


		QVector<QPointF> vecFFT2;
		for(int i = 21; i < 41; i++)
		{
			QPointF point;
			point.setX(i*1000);
			qreal val =  (qrand() % N - K);
			point.setY(val);
			vecFFT2.append(point);
		}

		grData->dataProccess(vecFFT2, isComplex);

		for(int i = 0; i < 40; i++)
		{
			if(i < 20)
			{
				qDebug() << i << vecFFT1.at(i).y() << grData->m_spectrum[i];
				TS_ASSERT_EQUALS(vecFFT1.at(i).y(), grData->m_spectrum[i]);
			}
			else
			{
				qDebug() << i << vecFFT2.at(i-20).y() << grData->m_spectrum[i];
				TS_ASSERT_EQUALS(vecFFT2.at(i-20).y(), grData->m_spectrum[i]);
			}

		}

		delete grData;
	}
};

#endif // TESTSTREAMING_H
