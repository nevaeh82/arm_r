#ifndef NETPINGALARM_H
#define NETPINGALARM_H

#include <QWidget>

namespace Ui {
class netpingalarm;
}

class netpingalarm : public QWidget
{
	Q_OBJECT

public:
	explicit netpingalarm(QWidget *parent = 0);
	~netpingalarm();

	void setText(QString);

private:
	Ui::netpingalarm *ui;
};

#endif // NETPINGALARM_H
