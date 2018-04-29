#include "NetPingDetector.h"

#include "Logger/Logger.h"
#include <QDateTime>

NetPingDetector::NetPingDetector(QObject *parent)
{
	indexNo = -2;
	indexYes = -2;

	alarmW = new netpingalarm(0);
	alarmW->close();

	m_readTimer = new QTimer(this);
	connect(m_readTimer, SIGNAL(timeout()), this, SLOT(searchForAlarm()));

	m_readTimer->start(30000);
}

NetPingDetector::~NetPingDetector()
{
	m_readTimer->stop();
}

void NetPingDetector::searchForAlarm()
{
	QDir directory(PATH + QString("%1/")
				   .arg(QDateTime::currentDateTime().toString("yyyyMMdd"))
				   );
	QStringList names;
	names.append("*.evl");
	QStringList lst = directory.entryList(names, QDir::NoFilter, QDir::Name);

	if(lst.isEmpty()) {
		return;
	}

	if(nameLast.isEmpty()) {
		nameLast = lst.last();
	}

	QFile f(QString(directory.absolutePath() + QString("/%1")).arg(lst.last()));
	bool tmp = f.open(QIODevice::ReadOnly);
	QByteArray data = f.readAll();
	int tmp2 = data.size();
	bool tmp3 = directory.exists();

	const char noVst[] = {0xEF, 0xF0, 0xEE, 0xEF, 0xE0, 0xEB, 0xEE, 0x20, 0xED, 0xE0,
						   0xEF, 0xF0, 0xFF, 0xE6, 0xE5, 0xED, 0xE8, 0xE5};
	const char yaVst[] = {0xEF, 0xEE, 0xFF, 0xE2, 0xE8, 0xEB, 0xEE, 0xF1, 0xFC, 0x20, 0xED, 0xE0, 0xEF, 0xF0,
		0xFF, 0xE6, 0xE5, 0xED, 0xE8, 0xE5};

	QByteArray byaVst(yaVst, 18);
	QByteArray bnoVst(noVst, 18);

	int noVstind = data.lastIndexOf(bnoVst);
	int yaVstind = data.lastIndexOf(byaVst);

	bool ret = false;

	if(indexNo == -2) {
		indexNo = noVstind;
		ret = true;
	}

	if(indexYes == -2) {
		indexYes = yaVstind;
		ret = true;
	}

	if(ret) {
		return;
	}

	if(noVstind >=0 && noVstind != indexNo) {
		QString txt = QString(data.mid(noVstind-20, 20)) + QString(" Power failed!!!");
		emit onSendAlarm(txt);
		alarmW->setText(txt);
		alarmW->show();
		//QMessageBox::critical(0, "Alarm", QString(data.mid(noVstind-20, 20)) + QString(" Power failed!!!"), QMessageBox::Yes);
	} else if(yaVstind >=0 && yaVstind != indexYes) {
		QString txt = QString(data.mid(yaVstind-20, 20)) + QString(" Power OK");
		 emit onSendAlarm(txt);
		alarmW->setText(txt);
		alarmW->show();
		//QMessageBox::critical(0, "Alarm", QString(data.mid(yaVstind-20, 20)) + QString(" Power OK"), QMessageBox::Yes);
	 }

	indexNo = noVstind;
	indexYes = yaVstind;

	 if(nameLast != lst.last()) {
		 nameLast = lst.last();
		 indexYes = -1;
		 indexNo = -1;
	 }
}


