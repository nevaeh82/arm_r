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
    // 100 - установить центральную частоту в ћ√ц (int),
    // 101 - установить фильтр в к√ц (любое значение (unsigned int) из 100, 300, 500, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000),
    // 102 - установить сдвиг в к√ц относительно центра (float -8000..8000),
    // 103 - начать координатометрию (bool state = true - стартовать, false - остановить)
    unsigned int type;
    //message length
    unsigned int length;
    //reserved data
    //for flakon (1 - —ќ‘№я, 2 - “ќ —ќ¬ќ, 3 - ¬—≈¬ќЋ∆—  - дл€ FFT, 1 - коррел€ци€ —офь€-“оксово, 2 - корррел€ци€ —офь€-¬севолжск)
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
