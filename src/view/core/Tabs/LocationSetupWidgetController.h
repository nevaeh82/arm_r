#pragma once

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "LocationSetupWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

#include "RdsPacket.pb.h"
#include "SolverPacket1.pb.h"

class LocationSetupWidgetController : public QObject, public IController<LocationSetupWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit LocationSetupWidgetController(QObject *parent = 0);
	virtual ~LocationSetupWidgetController();

	void setLocationSetup(const RdsProtobuf::Location& data);
	void setDetectorSetup(const RdsProtobuf::Detector& data);
	void setCorrectionSetup(const RdsProtobuf::Correction &data);

private:
	LocationSetupWidget* m_view;

public:
	void appendView(LocationSetupWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
	LocationSetupWidget *getView();

public slots:
	void slotShowWidget();


signals:
	void onMethodCalledSignal(QString, QVariant);

	void sendRdsData(QByteArray);
	void onSignalUpdate();

private slots:
	void onMethodCalledSlot(QString method, QVariant data);
	void slotSendSettings();

	void slotOnUpdate();
	void slotOnSet();

	void slotOnUpdateDet();
	void slotOnSetDet();

	void slotOnUpdateCor();
	void slotOnSetCor();
};

