#ifndef SOLVERENCODER_H
#define SOLVERENCODER_H

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>

#include <Interfaces/Tcp/ITcpReceiver.h>
#include <ISolverListener.h>
#include <Templates/BaseSubject.h>

#include "SolverPacket.pb.h"
#include "Tcp/TcpDefines.h"

class SolverEncoder : public QObject, public ITcpReceiver, public BaseSubject<ISolverListener>
{
	Q_OBJECT

public:
	SolverEncoder(QObject* parent = 0);
	~SolverEncoder();

	void onDataReceived( const QVariant& argument );

	QByteArray encode( const QByteArray& data );

private:
	QByteArray m_dataFromTcpSocket;
	uint m_residueLength;
	void readProtobuf(const QByteArray& inputData );
};

#endif; //SOLVERENCODER_H
