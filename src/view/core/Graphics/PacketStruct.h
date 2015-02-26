#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

#pragma pack(push, 1)

#define PAYLOAD_PREAMBULE       (0xFFEECCFF)
#define PAYLOAD_MAX_DATA_LEN    200000
#define PAYLOAD_HEADER_LENGTH   40

typedef struct ZaviruhaPayloadPacketHeader
{
    //! ���������, ������ 0xFFEECCFF
    unsigned int magic;
    //! ����� ������
    unsigned int number;
    //! ������������� ���������� (0 - Flacon, 1 - C����, 2 - �������, 3 - ���������)
    unsigned int id;
    //! �����, �� ���������
    unsigned int flags;
    //! ����� ������� ���������, �� ���������
    long long timestamp;
    /*!
     ��� ��������
     �� ������� ������:
     0 - ping,
     1 - FFT (QVector<QPointF>),
     2 � ������,
     3 - ����� (QString),
     4 - ���������� (unsigned int ����� ������_A, unsigned int ����� ������_B, QVector<QPointF>),
     5 - ���������� (QPointF),
     �� ������� �������:
     100 - ���������� ����������� ������� � ��� (int),
     101 - ���������� ������ � ��� (����� unsigned int �� 100, 300, 500, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000),
     102 - ���������� ����� � ��� ������������ ������ (float -8000..8000),
     103 - ������ ���������������� (bool true - ����������, false - ����������))
    */
    unsigned int type;
    //! ����� ���������
    unsigned int length;
    //! ��������� ������ (����� ������ �� ����������)
    int reserver;
    //! CRC16 ������
    unsigned short messageCRC;
    //! CRC8 ���������
    unsigned short headerCRC;
}ZaviruhaPayloadPacketHeader;

#pragma pack(pop)

#endif // PACKETSTRUCT_H
