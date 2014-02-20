#ifndef ATLANTTABWIDGET
#define ATLANTTABWIDGET

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>

#include <QHostAddress>

#include "Station.h"
#include "RPC/RPCAtlant.h"

#include "ITabAtlant.h"

namespace Ui {
class AtlantTabWidget;
}


class AtlantTabWidget : public QWidget, public ITabAtlant
{
	Q_OBJECT

private:
	Ui::AtlantTabWidget*     ui;

public:
	AtlantTabWidget(QWidget* parent = NULL);
	~AtlantTabWidget();

public:
	virtual void setLog(QByteArray data);

private:
	int                 _id;

	Station*       m_station;
	RPCAtlant*          _rpc_client;



private:
	int createRPC();
	int closeRPC();

private slots:
	void _slotStart();

	void _slot_send();

signals:
	void signalFinishRPC();
	void signalStartRPC();
	void signalStopRPC();

signals:
	void signalAddLog(QString str);

};

#endif // ATLANTTABWIDGET
