#include "TcpServerCoder.h"

TcpServerCoder::TcpServerCoder(QObject *parent) :
	BaseTcpDeviceCoder(parent)
{
}

TcpServerCoder::~TcpServerCoder()
{
}

MessageSP TcpServerCoder::encode(const QByteArray &data)
{
	m_dataFromTcpSocket.append(data);

	if (m_dataFromTcpSocket.size() < TCP_ZAVIRUHA_PREAMBULA_LEN){
		return MessageSP();
	}

	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_ZAVIRUHA_PREAMBULA);
	if (preambulaIndex < 0){
		return MessageSP();
	}

	memcpy(&m_residueLength, m_dataFromTcpSocket.data() + sizeof(TCP_ZAVIRUHA_PREAMBULA), sizeof(int));

	MessageSP dataToSend;

	if (m_dataFromTcpSocket.length() >= m_residueLength) {

		QByteArray data = m_dataFromTcpSocket.mid(preambulaIndex + TCP_ZAVIRUHA_PREAMBULA_LEN, m_residueLength);
		dataToSend = fromProtoBuf(data);

		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - (m_residueLength + TCP_ZAVIRUHA_PREAMBULA_LEN));
		m_residueLength = 0;
	}

	return dataToSend;
}

QByteArray TcpServerCoder::decode(const MessageSP message)
{
	return QByteArray();
}

QObject *TcpServerCoder::asQObject()
{
	return this;
}

MessageSP TcpServerCoder::fromProtoBuf(const QByteArray& data)
{
	Zaviruha::Packet packet;
	if (!packet.ParseFromArray(data, data.size())){
		return MessageSP();
	}

	QString method = "";

	switch (packet.command().action()){
		case Zaviruha::sendSolverClear:
			method = QString(TCP_ARMR_SEND_SOLVER_CLEAR);
			break;
		case Zaviruha::sendSolverData:
			method = QString(TCP_ARMR_SEND_SOLVER_DATA);
			break;
		default:
			break;
	}

	return MessageSP(new Message<QByteArray>(method, data));
}

QByteArray TcpServerCoder::toProtoBuf(const MessageSP& message)
{
	return QByteArray();
}
