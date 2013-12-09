#ifndef QSIGNALCOUNTER_H
#define QSIGNALCOUNTER_H
#include <QThread>
#include <QCoreApplication>
#include <QTime>
#include <QVariant>

class QSignalCounter : public QObject
{
	Q_OBJECT

private:
	class TestThread : public QThread
	{
		public:
			using QThread::msleep;
	};

	int _counter;
	QVariant _argument;

public:

	QSignalCounter(QObject* parent = 0) :
		QObject(parent),
		_counter(0)
	{
	}

	void exec(int sec)
	{
		QTime timer;
		timer.start();

		while(true) {
			QCoreApplication::processEvents();
			TestThread::msleep(50);
			int ms = timer.elapsed();
			if( ms/1000 >= sec) break;
		}
	}

	int getCount()
	{
		return _counter;
	}

	QVariant getArgument()
	{
		return _argument;
	}

public slots:

	void onEvent()
	{
		_counter++;
		_argument = QVariant();
	}

	void onEvent(int argument)
	{
		_counter++;
		_argument = QVariant( argument );
	}

	void onEvent(QVariant argument)
	{
		_counter++;
		_argument = argument;
	}
};

#endif // QSIGNALCOUNTER_H
