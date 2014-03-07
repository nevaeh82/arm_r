#ifndef TESTDB_H
#define TESTDB_H

#include <cxxtest/TestSuite.h>
//#include "../ARM_R_view/Tabs/Tree/DBController.h"

#include <QByteArray>
#include <QThread>
#include <QObject>

#include <QtTest/QTest>


class testPhoto : public CxxTest::TestSuite
{

public:
    void test1()
    {
//        IDBController* db_controller = new DBController();
//        QMap<QString, QVariant>* data = new QMap<QString, QVariant>;
//        data->insert("id", "1");
//        data->insert("pid", "1");
//        data->insert("name", "name");
//        data->insert("value", "2");
//        data->insert("state", 0);
//        data->insert("editable", "true");
//        db_controller->set_data(data);
		TS_ASSERT_EQUALS(1, 1);
    }
};

#endif // TESTDB_H
