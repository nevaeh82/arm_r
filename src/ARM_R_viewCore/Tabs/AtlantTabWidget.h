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

#include <QProcess>

#include "Station.h"
#include "RPC/RpcAtlantClient.h"

#include "ITabAtlant.h"

#include "Info/AtlantConfiguraton.h"

namespace Ui {
class AtlantTabWidget;
}


class AtlantTabWidget : public QWidget, public ITabAtlant
{
	Q_OBJECT

private:
	Ui::AtlantTabWidget*     ui;

	int m_id;
	RpcAtlantClient *m_rpcClient;

public:
	AtlantTabWidget(QWidget* parent = NULL);
	~AtlantTabWidget();

	virtual void setLog(QByteArray data);
	void setConfig(const AtlantConfiguration& config);

private slots:
	void _slot_send();
    void slotOpenRDP();

signals:
	void signalAddLog(QString str);

private:
    int openRDP();
};

#endif // ATLANTTABWIDGET
