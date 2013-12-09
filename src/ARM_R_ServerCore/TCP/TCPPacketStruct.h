#ifndef TCPPACKETSTRUCT_H
#define TCPPACKETSTRUCT_H

#pragma pack(push, 1)

#define PAYLOAD_PREAMBULE       (0xFFEECCFF)
#define PAYLOAD_MAX_DATA_LEN    20480
#define PAYLOAD_HEADER_LENGTH   40

typedef struct  ZaviruhaPayloadPacketHeader
{
    //magic 0xffeeccff
    unsigned int magic;
    //packet number
    unsigned int number;
    //id device
    unsigned int id;
    //flags
    unsigned int flags;
    //timestamp
    long long timestamp;
    //message type
    // 0 - ping, 1 - FFT, 2 - signal, 3 - coordinate, 4 - correlation
    // 100 - ������� ������������ ������, � ������� ���������� ������� (int 0..5), ���� >5 �� ���������� �������������,
    // 101 - ���������� ������ ������� � ��� (float 0..20000),
    // 102 - ���������� ����� � ��� ������������ ������ (float -9000..9000),
    // 103 - ������ ���������������� (bool state = true - ����������, false - ����������)
    // 104 - ���������� ������ (int - ������������� ����������, � �������� ����� ���������� ������).
    // 105 - �������� ���������� ������� (int 0..100, 0 - ��� ����������).
    unsigned int type;
    //message length
    unsigned int length;
    //reserved data
    //for flakon (1 - �����, 2 - �������, 3 - ��������� - ��� FFT, 1 - ���������� �����-�������, 2 - ����������� �����-���������)
    int reserved;
    //message CRC
    unsigned short messageCRC;
    //header CRC
    unsigned short headerCRC;
}ZaviruhaPayloadPacketHeader;

typedef struct ZaviruhaPayloadPacket
{
    ZaviruhaPayloadPacketHeader header;
    unsigned char   data[PAYLOAD_MAX_DATA_LEN];

}ZaviruhaPayloadPacket;

#pragma pack(pop)

#endif // TCPPACKETSTRUCT_H
