#include "Logger/Logger.h"
#include "SprutEncoder.h"

SprutEncoder::SprutEncoder(QObject* parent)
{
}

SprutEncoder::~SprutEncoder()
{
}

void SprutEncoder::onDataReceived(const QVariant& argument)
{
	QByteArray inData = argument.toByteArray();

	emit signalSprutIncome(inData);
}

QByteArray SprutEncoder::encode(const QByteArray &data)
{
	return QByteArray();
}

QByteArray SprutEncoder::decode(const MessageSP message) {
	return QByteArray();
}
