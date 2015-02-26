#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

#pragma pack(push, 1)

#define PAYLOAD_PREAMBULE       (0xFFEECCFF)
#define PAYLOAD_MAX_DATA_LEN    200000
#define PAYLOAD_HEADER_LENGTH   40

typedef struct ZaviruhaPayloadPacketHeader
{
    //! Преамбула, всегда 0xFFEECCFF
    unsigned int magic;
    //! Номер пакета
    unsigned int number;
    //! Идентификатор устройства (0 - Flacon, 1 - Cофья, 2 - Токсово, 3 - Всеволжск)
    unsigned int id;
    //! Флаги, не использую
    unsigned int flags;
    //! Всемя посылки сообщения, не использую
    long long timestamp;
    /*!
     Тип собщения
     От сервера кленту:
     0 - ping,
     1 - FFT (QVector<QPointF>),
     2 – сигнал,
     3 - текст (QString),
     4 - корреляция (unsigned int номер пункта_A, unsigned int номер пункта_B, QVector<QPointF>),
     5 - координаты (QPointF),
     От клиента серверу:
     100 - установить центральную частоту в МГц (int),
     101 - установить фильтр в кГц (любое unsigned int из 100, 300, 500, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000),
     102 - установить сдвиг в кГц относительно центра (float -8000..8000),
     103 - начать координатометрию (bool true - стартовать, false - остановить))
    */
    unsigned int type;
    //! Длина сообщения
    unsigned int length;
    //! Резервные данные (можно ничего не передавать)
    int reserver;
    //! CRC16 данных
    unsigned short messageCRC;
    //! CRC8 заголовка
    unsigned short headerCRC;
}ZaviruhaPayloadPacketHeader;

#pragma pack(pop)

#endif // PACKETSTRUCT_H
