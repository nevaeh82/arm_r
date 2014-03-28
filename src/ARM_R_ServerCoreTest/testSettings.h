#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <cxxtest/TestSuite.h>
#include <TCP/TcpSettingsManager.h>
//#include <TCP/TcpPRM300Controller.h>
#include <QDebug>

class testSettings : public CxxTest::TestSuite
{
public:
	void testGetAllInfo()
	{
		TcpSettingsManager settingsManager;
		settingsManager.setIniFile("./TCP/coders.ini");

		QMap<QString, int> mapInfo = settingsManager.getAllInfo();

        TS_ASSERT_EQUALS(4, mapInfo.count());
	}

//	void testInit()
//	{
//		TcpPRM300Controller *controller = new TcpPRM300Controller("Гал");
//		bool err = controller->init();

//		TS_ASSERT_EQUALS(true, err);
//		delete controller;
//	}

};

#endif // TESTSETTINGS_H
