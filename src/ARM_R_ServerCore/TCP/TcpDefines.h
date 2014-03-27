#ifndef TCPDEFINES_H
#define TCPDEFINES_H

#define TCP_ZAVIRUHA_PREAMBULA "ZAV"
#define TCP_ZAVIRUHA_PREAMBULA_LEN 7

#define TCP_EMPTY_MESSAGE	"tcpEmptyMessage"

#define TCP_ARMR_SEND_SOLVER_DATA		"tcpArmrSendSolverData"
#define TCP_ARMR_SEND_SOLVER_CLEAR		"tcpArmrSendSolverClear"

/// COORDINATES
#define ARM_R_SERVER_BPLA_COORDS        5001
#define ARM_R_SERVER_BPLA_COORDS_AUTO   5002

/// ATLANT
#define ARM_R_SERVER_ATLANT_DIRECTION   7001
#define ARM_R_SERVER_ATLANT_POSITION    7002

/// Atlant device internal
#define TCP_ATLANT_ANSWER_DIRECTION			"tcpAtlantAnswerDirection"
#define TCP_ATLANT_ANSWER_POSITION			"tcpAtlantAnswerPosition"
#define TCP_ATLANT_REQUEST_SET_FREQUENCY	"tcpAtlantRequestSetFrequency"

/// PRM300 device internal
#define TCP_PRM300_STATUS						"tcpPRM300Status"
#define TCP_PRM300_ANSWER_STATUS				"tcpPRM300AnswerStatus"
#define TCP_PRM300_REQUEST_SET_FREQUENCY		"tcpPRM300RequestSetFrequency"
#define TCP_PRM300_REQUEST_GET_FREQUENCY		"tcpPRM300RequestGetFrequency"
#define TCP_PRM300_REQUEST_SET_ATTENUER_ONE		"tcpPRM300RequestSetAttenuerOne"
#define TCP_PRM300_REQUEST_SET_ATTENUER_TWO		"tcpPRM300RequestSetAttenuerTwo"
#define TCP_PRM300_REQUEST_SET_FILTER			"tcpPRM300RequestSetFilter"

/// Flakon device internal
#define TCP_FLAKON_ANSWER_FFT							"tcpFlakonAnswerFFT"
#define TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH			"tcpFlakonAnswerDetectedBandwidth"
#define TCP_FLAKON_ANSWER_CORRELATION					"tcpFlakonAnswerCorrelation"
#define TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION		"tcpFlakonRequestMainStationCorrelation"
#define TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM				"tcpFlakonRequestAverageSpectrum"
#define TCP_FLAKON_REQUEST_SET_BANDWIDTH				"tcpFlakonRequestSetBandwidth"
#define TCP_FLAKON_REQUEST_SET_SHIFT					"tcpFlakonRequestSetShift"
#define TCP_FLAKON_REQUEST_RECOGNIZE					"tcpFlakonRequestRecognize"
#define TCP_FLAKON_REQUEST_SS_CORRELATION				"tcpFlakonRequestSSCorrelation"

/// Flakon Coordinates Counter
#define TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA					"tcpFlakonCoordinatesCounterAnswerBPLA"
#define TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO				"tcpFlakonCoordinatesCounterAnswerBPLAAuto"
#define TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER			"tcpFlakonCoordinatesCounterRequestSetSolver"
#define TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER_CLEAR		"tcpFlakonCoordinatesCounterRequestSetSolverClear"

/// Atlant device external
#define Atlant_Direction_MsgA	"DIRECTION_ANSWER_MESSAGE"
#define Atlant_Position_MsgA	"POSITION_ANSWER_MESSAGE"
#define Atlant_Direction_Finding_Request	"DIRECTION_FINDING_REQUEST"

/// PRM300 device external
namespace PRM300External {
typedef enum TypePRM300ExternalEnum {
	TypeFrequencyChangedAnswer			= 2,
	TypeAttenuerOneChangedAnswer		= 3,
	TypeFrequencyAnswer					= 4,
	TypeFilterChangedAnswer				= 6,
	TypeAttenuerTwoChangedAnswer		= 7
} TypePRM300ExternalEnum;
}

/// Flakon device external
namespace FlakonExternal {
typedef enum TypeFlakonExternalEnum {
	// From server to client
	TypePing						= 0,
	TypePointsReceivedAnswer		= 1,
	TypeDetectedBandWidthAnswer		= 2,
	TypeStringAnswer				= 3,
	TypeCorrelationReceivedAnswer	= 4,
	TypeSignalTypeAnswer			= 5,

	// From client to server
	TypeSetMainStationCorrelationRequest	= 100,
	TypeSetBandWidthRequest					= 101,
	TypeSetShiftRequest						= 102,
	TypeStartCorrelationRequest				= 103,
	TypeRecognizeSignalRequest				= 104,
	TypeSetSpectrumAverageParametrRequest	= 105

} TypeFlakonExternalEnum;
}

#define PAYLOAD_PREAMBULE		0xFFEECCFF
#define PAYLOAD_MAX_DATA_LEN	200000
#define PAYLOAD_HEADER_LENGTH	40

#pragma pack(push, 1)
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
	/**!
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
	//! � езервные данные (можно ничего не передавать)
	int reserved;
	//! CRC16 данных
	unsigned short messageCRC;
	//! CRC8 заголовка
	unsigned short headerCRC;
} ZaviruhaPayloadPacketHeader;
#pragma pack(pop)

#endif // TCPDEFINES_H
