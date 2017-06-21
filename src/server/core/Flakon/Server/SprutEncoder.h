#pragma once

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>

#include <Interfaces/Tcp/ITcpReceiver.h>
#include <ISolverListener.h>
#include <Templates/BaseSubject.h>

#include <MessageSP.h>

#include "Tcp/TcpDefines.h"

class SprutEncoder : public QObject, public ITcpReceiver
{
	Q_OBJECT

public:
	SprutEncoder(QObject* parent = 0);
	~SprutEncoder();

	void onDataReceived( const QVariant& argument );

	QByteArray encode( const QByteArray& data );
	QByteArray decode( const MessageSP message );

signals:
	void signalSprutIncome(QByteArray);
};

