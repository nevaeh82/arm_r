#ifndef CRCS_OLD_H
#define CRCS_OLD_H

#include <QVector>

/// all implementaion of count checksum
class CRCsOld
{
public:
	CRCsOld();

public:
        unsigned char crc8(unsigned char *data, int len);
        unsigned short crc16(unsigned char *data, int len);
        quint8 calcCRC(QVector<quint8> aForCrcCount);


private:
        static const char             _magic[];
};

#endif // CRCS_OLD_H
