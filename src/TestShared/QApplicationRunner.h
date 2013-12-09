#ifndef QAPPLICATIONRUNNER_H
#define QAPPLICATIONRUNNER_H
#include <QThread>
#include <QCoreApplication>
#include <QTimer>

class QApplicationRunner : public QObject
{
	Q_OBJECT

private:
	class TestThread : public QThread
		{
		public:
			using QThread::msleep;
	};

	bool finished;

public:

	QApplicationRunner(): finished(false)
	{
	}

	virtual ~QApplicationRunner()
	{
	}

	void exec()
	{
		exec(20*1000);
	}


	void exec(int sec)
	{
		QTimer timer;
		timer.setSingleShot(true);
		connect(&timer, SIGNAL(timeout()), SLOT(onEvent()));
		timer.start(sec);

		while(!finished) {
			QCoreApplication::processEvents();
			TestThread::msleep(10);
		}
		finished = false;
	}

public slots:

	void onEvent()
	{
		finished = true;
	}
};

#endif // QAPPLICATIONRUNNER_H
