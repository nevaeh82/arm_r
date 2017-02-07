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

#include "ISolverEncoder.h"

#include "SolverPacket1.pb.h"
#include "Tcp/TcpDefines.h"

class SolverEncoder : public QObject, public ITcpReceiver, public ISolverEncoder, public BaseSubject<ISolverListener>
{
	Q_OBJECT

public:
	SolverEncoder(QObject* parent = 0);
	~SolverEncoder();

	void onDataReceived( const QVariant& argument );

	QByteArray encode( const QByteArray& data );
	QByteArray decode( const MessageSP message );

	QByteArray decodeRegister();

private:
	QByteArray m_dataFromTcpSocket;
	uint m_residueLength;

	void readProtobuf(const QByteArray& inputData );
    void addPreambula(QByteArray& data);
    quint8 crc(const QByteArray &data);

signals:
    void nippDataIncome(bool);
};

#endif; //SOLVERENCODER_H
