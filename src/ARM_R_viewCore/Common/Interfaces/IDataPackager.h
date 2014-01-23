#ifndef IDATAPACKAGER_H
#define IDATAPACKAGER_H

#include <QByteArray>
#include <QSharedMemory>
#include <QMap>
#include <QList>
#include "../TelemetryDefines.h"

class IDataPackager
{
public:
	virtual ~IDataPackager(){}

	virtual QByteArray* createPacket(QByteArray&, FrameHeader&) = 0;
	virtual void createVideoPacket(QSharedMemory*, FrameHeader&, uint&) = 0;

	virtual bool readPacket(QByteArray& inputData, QByteArray& outputData, FrameHeader& outputFrameHeader) = 0;

	virtual bool moveToShared(QMap<QString, const QByteArray*>&) = 0;
	virtual bool readFromShared(const QList<QString>& inputSharedKeys, QMap<QString, const QByteArray*>& outputData) = 0;
};

#endif // IDATAPACKAGER_H
