#pragma once

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>

#include <Interfaces/Tcp/ITcpReceiver.h>
#include <ISolverListener.h>
#include <Templates/BaseSubject.h>
#include <TCP/BaseTcpDeviceCoder.h>


#include "Interfaces/Tcp/ITcpListener.h"

#include "ISolverEncoder.h"

#include "SolverPacket1.pb.h"
#include "Tcp/TcpDefines.h"

class SolverEncoder1 : public BaseTcpDeviceCoder, public ITcpReceiver, public BaseSubject<ISolverListener>
{
	Q_OBJECT

public:
	SolverEncoder1(QObject* parent = 0);
	~SolverEncoder1();

	void onDataReceived( const QVariant& argument );

	MessageSP encode( const QByteArray& data );
	QByteArray decode(const MessageSP message);

	void readProtobuf(const QByteArray& inputData );

private:
	QByteArray m_dataFromTcpSocket;
	uint m_residueLength;
	void addPreambula(QByteArray &data);

	void sendSolverAnswerToGui(const QByteArray& data);
};
