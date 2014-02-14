#ifndef TCPDEFINES_H
#define TCPDEFINES_H

#define TCP_EMPTY_MESSAGE	"tcpEmptyMessage"

/// Atlant device internal
#define TCP_ATLANT_ANSWER_DIRECTION			"tcpAtlantAnswerDirection"
#define TCP_ATLANT_ANSWER_POSITION			"tcpAtlantAnswerPosition"
#define TCP_ATLANT_REQUEST_SET_FREQUENCY	"tcpAtlantRequestSetFrequency"

/// PRM300 device internal
#define TCP_PRM300_ANSWER_STATUS				"tcpPRM300AnswerStatus"
#define TCP_PRM300_REQUEST_SET_FREQUENCY		"tcpPRM300RequestSetFrequency"
#define TCP_PRM300_REQUEST_GET_FREQUENCY		"tcpPRM300RequestGetFrequency"
#define TCP_PRM300_REQUEST_SET_ATTENUATOR_1		"tcpPRM300RequestSetAttenuator1"
#define TCP_PRM300_REQUEST_SET_ATTENUATOR_2		"tcpPRM300RequestSetAttenuator2"
#define TCP_PRM300_REQUEST_SET_FILTER			"tcpPRM300RequestSetFilter"

/// Flakon device internal
#define TCP_FLAKON_ANSWER_FFT							"tcpFlakonAnswerFFT"
#define TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH			"tcpFlakonAnswerDetectedBandwidth"
#define TCP_FLAKON_ANSWER_CORRELATION					"tcpFlakonAnswerCorrelation"
#define TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION		"tcpFlakonRequestMainStationCorrelation"
#define TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM				"tcpFlakonRequestAverageSpectrum"

/// Atlant device external
#define Atlant_Direction_MsgA	"DIRECTION_ANSWER_MESSAGE"
#define Atlant_Position_MsgA	"POSITION_ANSWER_MESSAGE"
#define Atlant_Direction_Finding_Request	"DIRECTION_FINDING_REQUEST"

/// PRM300 device external
namespace PRM300External {
typedef enum TypePRM300ExternalEnum {
	TypeFrequencyChangedAnswer			= 2,
	TypeAttenuatorOneChangedAnswer		= 3,
	TypeFrequencyAnswer					= 4,
	TypeFilterChangedAnswer				= 6,
	TypeAttenuatorTwoChangedAnswer		= 7
} TypePRM300ExternalEnum;
}

/// Flakon device external
namespace FlackonExternal {
typedef enum TypeFlaconExternalEnum {
	TypePointsReceivedAnswer		= 1,
	TypeDetectedBandWidthAnswer		= 2,
	TypeCorrelationReceivedAnswer	= 4
} TypeFlaconExternalEnum;
}

#endif // TCPDEFINES_H
