#include "TcpPRM300Coder.h"

TcpPRM300Coder::TcpPRM300Coder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_residueLength = 0;
	m_needBytes = false;
}

TcpPRM300Coder::~TcpPRM300Coder()
{
}

MessageSP TcpPRM300Coder::encode(const QByteArray& data)
{
	/// What does the fucking magic happen here?!

	/// TODO: recheck
	MessageSP message;
	m_dataFromTcpSocket.append(data);

	QByteArray inputData = data;

	QVector<quint8> aForCrcCount;
	QDataStream inputDataSream(&inputData, QIODevice::ReadOnly);
	inputDataSream.setVersion(QDataStream::Qt_4_7);
	quint8 aFirstByte, aLastByte, aCrc;
	quint8 aSizeAll = data.size();

	if (!m_needBytes) {
		bool found = findPreamble();
		if (!found) {
			return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, data));
		}
	}

	if (m_dataFromTcpSocket.size() < 3 && m_residueLength == 0) {
		m_needBytes = true;
		return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, data));
	}

	if (m_residueLength == 0) {
		inputDataSream >> aFirstByte;
		if (aFirstByte == 170) {
			inputDataSream >> m_aAddr;
			inputDataSream >> m_aSize;
			m_residueLength = m_aSize;
			inputDataSream >> m_aPacketType;
		} else {
			return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, data));
		}
	}

	if (m_dataFromTcpSocket.size() < m_residueLength) {
		return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, data));
	}

	if (m_aPacketType == (quint8)PRM300External::TypeFrequencyChangedAnswer /*2*/) {
		quint16 aFlag;
		inputDataSream >> aFlag;
        message = prmFrequencyChanged(aFlag);
	}

	if (m_aPacketType == (quint8)PRM300External::TypeAttenuerOneChangedAnswer /*3*/) {
		quint16 aFlag;
		inputDataSream >> aFlag;
	}

	if (m_aPacketType == (quint8)PRM300External::TypeFrequencyAnswer /*4*/) {
		quint16 aFreq;
		quint8 aAt1, aAt2, aAt3, aFltr, aFreqA, aFreqB, aFreqC, aFreqFirst, aFreqLast;
		bool aChangeCRCFlag = false;

		if(aSizeAll != 12) {
			inputDataSream >> aFreq;
		} else {
			inputDataSream >> aFreqA >> aFreqB >> aFreqC;
			if (aFreqA == (0x5A)) {
				if (aFreqB == 0x00) {
					aFreqFirst = 0x5A;
					aFreqLast = aFreqC;
				} else if (aFreqB == 0x01) {
					aFreqFirst = 0x55;
					aFreqLast = aFreqC;
				} else if (aFreqB == 0x02) {
					aFreqFirst = 0xA5;
					aFreqLast = aFreqC;
				} else if (aFreqB == 0x03) {
					aFreqFirst = 0xAA;
					aFreqLast = aFreqC;
				}
			}
			else if (aFreqB == (0x5A)) {
				if (aFreqC == 0x00) {
					aFreqFirst = aFreqA;
					aFreqLast = 0x5A;
				} else if (aFreqC == 0x01) {
					aFreqFirst = aFreqA;
					aFreqLast = 0x55;
				} else if (aFreqC == 0x02) {
					aFreqFirst = aFreqA;
					aFreqLast = 0xA5;
				} else if (aFreqC == 0x03) {
					aFreqFirst = aFreqA;
					aFreqLast = 0xAA;
				}
			}
			else {
				aChangeCRCFlag = true;
			}

			if (!aChangeCRCFlag) {
				aFreq = 256 * aFreqFirst + aFreqLast;
			} else {
				aFreq = 256 * aFreqA + aFreqB;
				aAt1 = aFreqC;
			}
		}

		if (!aChangeCRCFlag) {
			inputDataSream >> aAt1 >> aFltr >> aAt2 >> aAt3 >> aLastByte;
		} else {
			inputDataSream >> aFltr >> aAt2 >> aAt3 >> aFreqA >> aFreqB;
			if (aFreqA == (0x5A)) {
				if (aFreqB == 0x00) {
					aLastByte = 0x5A;
				} else if (aFreqB == 0x01) {
					aLastByte = 0x55;
				} else if (aFreqB == 0x02) {
					aLastByte = 0xA5;
				} else if (aFreqB == 0x03) {
					aLastByte = 0xAA;
				}
			}
		}
		qDebug() << "GOT FREQ = " << aFreq << aFltr << aAt1 << aAt2;

		message = sendPRMStatus(aFreq, aFltr, aAt1, aAt2);

		aForCrcCount.append(m_aAddr);
		aForCrcCount.append(m_aSize);
		aForCrcCount.append(m_aPacketType);
		aForCrcCount.append((aFreq >> 8) & 0xFF); aForCrcCount.append(aFreq & 0xFF);
		aForCrcCount.append(aAt1);
		aForCrcCount.append(aFltr);
		aForCrcCount.append(aAt2);
		aForCrcCount.append(aAt3);

		aCrc = calcCRC(aForCrcCount);

	}

	if (m_aPacketType == (quint8)PRM300External::TypeFilterChangedAnswer /*6*/) {
		quint16 aFlag;
		inputDataSream >> aFlag;
	}

	if (m_aPacketType == (quint8)PRM300External::TypeAttenuerTwoChangedAnswer /*7*/) {
		quint16 aFlag;
		inputDataSream >> aFlag;
	}

	m_dataFromTcpSocket.remove(0, m_residueLength);
	m_residueLength = 0;

	return message;
}

QByteArray TcpPRM300Coder::decode(const MessageSP message)
{
	QByteArray inputData = message->data();
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

	QByteArray dataToSend;

	int int_value = 0;
	if (message->type() == TCP_PRM300_REQUEST_SET_FREQUENCY) {
		QString name;
		unsigned short avalue;
		inputDataStream >> name;
		inputDataStream >> avalue;
		dataToSend = prmSetFrequency(avalue);
	} else if (message->type() == TCP_PRM300_REQUEST_GET_FREQUENCY) {
		dataToSend = prmGetFrequency();
	} else if (message->type() == TCP_PRM300_REQUEST_SET_ATTENUER_ONE) {
		inputDataStream >> int_value;
		dataToSend = prmSetAttenuerOne(int_value);
	} else if (message->type() == TCP_PRM300_REQUEST_SET_ATTENUER_TWO) {
		inputDataStream >> int_value;
		dataToSend = prmSetAttenuerTwo(int_value);
	} else if (message->type() == TCP_PRM300_REQUEST_SET_FILTER) {
		inputDataStream >> int_value;
		dataToSend = prmSetFilter(int_value);
	}

	return dataToSend;
}

QObject* TcpPRM300Coder::asQObject()
{
	return this;
}

/// search first ridth byte == 85 (0x55)
bool TcpPRM300Coder::findPreamble()
{
	QDataStream inputDataStream(&m_dataFromTcpSocket, QIODevice::ReadOnly);
	inputDataStream.setVersion(QDataStream::Qt_4_7);
	quint8 byte;
	for(int i = 0; i < m_dataFromTcpSocket.size(); i++) {
		inputDataStream >> byte;
		if (byte == 0xAA) {
			m_dataFromTcpSocket.remove(0, i);
			m_needBytes = true;
			return true;
		}
	}
	m_needBytes = false;
	return false;
}

/// send freq to rpc
MessageSP TcpPRM300Coder::sendPRMStatus(quint16 freq, quint8 filter, quint8 att1, quint8 att2)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << freq;
	dataStream << filter;
	dataStream << att1;
	dataStream << att2;

	return MessageSP(new Message<QByteArray>(TCP_PRM300_ANSWER_STATUS, byteArray));
}

MessageSP TcpPRM300Coder::sendPRMStatus(int status)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << status;

	return MessageSP(new Message<QByteArray>(TCP_PRM300_STATUS, byteArray));
}

MessageSP TcpPRM300Coder::prmFrequencyChanged(unsigned short aFreq)
{
    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
    dataStream << aFreq;

    return MessageSP(new Message<QByteArray>(TCP_PRM300_FREQUENCY_CHANGED, byteArray));
}

QByteArray TcpPRM300Coder::prmSetFrequency(unsigned short aFreq)
{
	/// WTF?! Magic again

	QByteArray ba;
	QDataStream streamWrite(&ba, QIODevice::WriteOnly);
	quint8 aStart, aAddr, aSize, aBodyType, aCrc, aFreqFirst, aFreqLast, aTemp;
	quint8 aSpecialB;
	aTemp = 0;

	aStart = 170;			//0xAA
	aAddr = 255;			//0xFF
	aSize = 1 + 2;
	aBodyType = 2;

	QVector<quint8> aForCrcCount;
	aForCrcCount.append(aAddr);
	aForCrcCount.append(aSize);
	aForCrcCount.append(aBodyType);

	aFreqFirst = ((aFreq >> 8) & 0xFF);
	aFreqLast = (aFreq & 0xFF);

	aForCrcCount.append(aFreqFirst);
	aForCrcCount.append(aFreqLast);

	aCrc = calcCRC(aForCrcCount);

	streamWrite << aStart << aAddr << aSize << aBodyType;

	if (specialSymb(aFreqFirst, aSpecialB)) {
		streamWrite << aFreqFirst << aSpecialB;
	} else {
		streamWrite << aFreqFirst;
	}

	if (specialSymb(aFreqLast, aSpecialB)) {
		streamWrite << aFreqLast << aSpecialB;
	} else {
		streamWrite << aFreqLast;
	}

	if (specialSymb(aCrc, aSpecialB)) {
		streamWrite << aCrc << aSpecialB;
	} else {
		streamWrite << aCrc;
	}

    return ba;
}

bool TcpPRM300Coder::specialSymb(quint8 &aByteA, quint8 &aByteB)
{
	/// Magic again
	bool aTemp=false;

	if (aByteA == 0x5A) {
		aByteA = 0x5A;
		aByteB = 0x00;
		aTemp = true;
	} else if (aByteA == 0x55) {
		aByteA = 0x5A;
		aByteB = 0x01;
		aTemp = true;
	} else if (aByteA == 0xA5) {
		aByteA = 0x5A;
		aByteB = 0x02;
		aTemp = true;
	} else if (aByteA == 0xAA) {
		aByteA = 0x5A;
		aByteB = 0x03;
		aTemp = true;
	}

	return aTemp;
}

QByteArray TcpPRM300Coder::prmGetFrequency()
{
	/// Magic again

	QByteArray ba;
	QDataStream streamWrite(&ba, QIODevice::WriteOnly);

	quint8 aStart, aAddr, aSize, aBodyType, aCrc;

	// Запрос о текущей частоте
	aStart = 170;		//0xAA
	aAddr = 255;		//0xFF
	aSize = 1;			//0x01
	aBodyType = 4;

	QVector<quint8> aForCrcCount;
	aForCrcCount.append(aAddr);
	aForCrcCount.append(aSize);
	aForCrcCount.append(aBodyType);

	aCrc = calcCRC(aForCrcCount);

	streamWrite << aStart << aAddr << aSize << aBodyType << aCrc;

	return ba;
}

QByteArray TcpPRM300Coder::prmSetAttenuerOne(int value)
{
	/// Magic again

	quint8 aValue = static_cast<quint8>(value);
	QByteArray ba;
	QDataStream streamWrite(&ba, QIODevice::WriteOnly);
	quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
	quint8 aSpecialB;

	// Установка аттенюатора 1
	aStart = 170;		//0xAA
	aAddr = 255;		//0xFF
	aSize = 1 + 1;
	aBodyType = 3;
	aBodyData = aValue;

	QVector<quint8> aForCrcCount;
	aForCrcCount.append(aAddr);
	aForCrcCount.append(aSize);
	aForCrcCount.append(aBodyType);
	aForCrcCount.append(aBodyData);

	aCrc = calcCRC(aForCrcCount);

	streamWrite << aStart << aAddr << aSize << aBodyType << aBodyData;

	// Проверка специальных символов
	if (specialSymb(aCrc, aSpecialB)) {
		streamWrite << aCrc << aSpecialB;
	} else {
		streamWrite << aCrc;
	}

	return ba;
}

QByteArray TcpPRM300Coder::prmSetAttenuerTwo(int value)
{
	/// Magic again

	quint8 aValue = static_cast<quint8>(value);
	QByteArray ba;
	QDataStream streamWrite(&ba, QIODevice::WriteOnly);

	quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
	quint8 aSpecialB;

	// Установка аттенюатора 2
	aStart = 170;		//0xAA
	aAddr = 255;		//0xFF
	aSize = 1 + 1;
	aBodyType = 7;
	aBodyData = aValue;

	QVector<quint8> aForCrcCount;
	aForCrcCount.append(aAddr);
	aForCrcCount.append(aSize);
	aForCrcCount.append(aBodyType);
	aForCrcCount.append(aBodyData);

	aCrc = calcCRC(aForCrcCount);

	streamWrite << aStart << aAddr << aSize << aBodyType << aBodyData;

	// Проверка специальных символов
	if (specialSymb(aCrc, aSpecialB)) {
		streamWrite << aCrc << aSpecialB;
	} else {
		streamWrite << aCrc;
	}

	return ba;
}

QByteArray TcpPRM300Coder::prmSetFilter(int value)
{
	/// Magic again

	quint8 aValue = static_cast<quint8>(value);
	QByteArray ba;
	QDataStream streamWrite(&ba, QIODevice::WriteOnly);

	quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
	quint8 aSpecialB;

	// Установка фильтра
	aStart = 170;		//0xAA
	aAddr = 255;		//0xFF
	aSize = 1 + 1;
	aBodyType = 6;
	aBodyData = aValue;

	QVector<quint8> aForCrcCount;
	aForCrcCount.append(aAddr);
	aForCrcCount.append(aSize);
	aForCrcCount.append(aBodyType);
	aForCrcCount.append(aBodyData);

	aCrc = calcCRC(aForCrcCount);

	streamWrite << aStart << aAddr << aSize << aBodyType << aBodyData;

	// Проверка специальных символов
	if (specialSymb(aCrc, aSpecialB)) {
		streamWrite << aCrc << aSpecialB;
	} else {
		streamWrite << aCrc;
	}

	return ba;
}

quint8 TcpPRM300Coder::calcCRC(QVector<quint8> aForCrcCount)
{
	if (aForCrcCount.size() > 12) {
		return 0;
	}
	quint8 c = 0;
	foreach (quint8 item, aForCrcCount) {
		c += item;
	}
	return c + 1;
}
