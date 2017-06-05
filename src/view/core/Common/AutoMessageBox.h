#include <QMessageBox>

class AutoMessageBox : public QMessageBox
{
	int timeoutClose;
	bool autoClose;
	int currentTime;

public:
	AutoMessageBox(QWidget* parent = 0) :
		QMessageBox(parent),
		timeoutClose(5),
		autoClose(true),
		currentTime(0) {}

	void setTimeoutClose(int val) {timeoutClose = val;}

protected:
	void showEvent(QShowEvent * event);
	void timerEvent(QTimerEvent * event);

};

void AutoMessageBox ::showEvent ( QShowEvent * event ) {
	currentTime = 0;
	if (autoClose) {
		this->startTimer(1000);
	}
}

void AutoMessageBox ::timerEvent(QTimerEvent *event)
{
	currentTime++;
	if (currentTime>=timeoutClose) {
		this->done(0);
	}
}
