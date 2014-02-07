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
     2 – обнаруженный сигнал на текущем id пункте (QVector<QPointF>, например (-1.0,5.2) - сигнал с шириной полосы от -1 МГц до 5.2 МГц ),
     3 - текст (QString),
     4 - корреляция (uint номер пункта_A, uint номер пункта_B, QVector<QPointF>),
     5 - тип сигнала (int - Тип сигнала)
     От клиента серверу:
     100 - задание центрального пункта, с которым происходит свертка (int 0..5), если >5 то выбирается автоматически
     101 - установить ширину сигнала в кГц (float 0..20000),
     102 - установить сдвиг в кГц относительно центра (float -9000..9000),
     103 - начать координатометрию (bool true - стартовать, false - остановить).
     104 - распознать сигнал (int - Идентификатор устройства, с которого нужно распознать сигнал).
	 105 - параметр усреднения спектра (int 0..100, 0 - нет усреднения).
    */
    unsigned int type;
    //! Длина сообщения
    unsigned int length;
    //! Резервные данные (можно ничего не передавать)
	int reserved;
    //! CRC16 данных
    unsigned short messageCRC;
    //! CRC8 заголовка
    unsigned short headerCRC;
}ZaviruhaPayloadPacketHeader;

#pragma pack(pop)

#endif // PACKETSTRUCT_H
