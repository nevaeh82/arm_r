#ifndef RPCDEFINES_H
#define RPCDEFINES_H

#define RPC_SLOT_SET_CLIENT_ID                    "rpc_slot_set_client_id"
#define RPC_METHOD_SET_MAIN_STATION_CORRELATION   "rpc_slot_set_main_station_cor"
#define RPC_METHOD_SET_BANDWIDTH                  "rpc_slot_set_bandwidth"
#define RPC_METHOD_SET_SHIFT                      "rpc_slot_set_shift"
#define RPC_METHOD_SET_CENTER                     "rpc_slot_set_center"
#define RPC_METHOD_RECOGNIZE                      "rpc_slot_recognize"
#define RPC_METHOD_SS_CORRELATION                 "rpc_slot_ss_correlation"
#define RPC_METHOD_SS_CORRELATION1                "rpc_slot_ss_correlation1"

#define RPC_METHOD_AVARAGE_SPECTRUM       "rpc_slot_avarage_spectrum"
#define RPC_METHOD_ENABLE_RECEIVER        "rpc_slot_enable_receiver"

#define RPC_METHOD_SOLVER_SETTINGS_SETUP        "rpc_slot_solver_settings_setup"

#define RPC_METHOD_WORK_MODE       "rpc_slot_work_mode"
#define RPC_METHOD_WORK_MODE_M       "rpc_slot_work_mode_m"
#define RPC_METHOD_WORK_MODE_ON       "rpc_slot_work_mode_on"

///PRM300 from rpc client
#define RPC_METHOD_PRM_SET_FREQUENCY       "rpc_slot_prm_set_freq"
#define RPC_METHOD_PRM_REQUEST_FREQUENCY   "rpc_slot_prm_request_freq"
#define RPC_METHOD_PRM_SET_ATT1            "rpc_slot_prm_set_att1"
#define RPC_METHOD_PRM_SET_ATT2            "rpc_slot_prm_set_att2"
#define RPC_METHOD_PRM_SET_FILTER          "rpc_slot_prm_set_filter"

/// rpc config client
#define RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST				"rpcMethodConfigRequestGetStationList"
#define RPC_METHOD_CONFIG_ANSWER_STATION_LIST					"rpcMethodConfigAnswerStationList"
#define RPC_METHOD_CONFIG_ANSWER_LOCSYSTEM					"rpcMethodConfigAnswerLocSystem"
#define RPC_METHOD_CONFIG_REQUEST_GET_ATLANT_CONFIGURATION		"rpcMethodConfigRequsetGetAtlantConfiguration"
#define RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION			"rpcMethodConfigAnswerAtlantConfiguration"
#define RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION			"rpcMethodConfigRequsetGetDBConfiguration"
#define RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION				"rpcMethodConfigAnswerDBConfiguration"

///server
#define RPC_SLOT_SERVER_SEND_POINTS                 "rpc_slot_server_send_points"
#define RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH		"rpc_slot_server_send_detected_bandwidth"
#define RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION    "rpc_slot_server_send_response_modulation"
#define RPC_SLOT_SERVER_SEND_CORRELATION            "rpc_slot_server_send_correlation"
#define RPC_SLOT_SERVER_SEND_BPLA_RESULT			"rpc_slot_server_send_solver_result"
#define RPC_SLOT_SERVER_SEND_BPLA_RESULT_1			"rpc_slot_server_send_solver_result_1"
#define RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1		"rpc_slot_server_send_answer_result_1"
#define RPC_SLOT_SERVER_SEND_HYPERBOLA				"rpc_slot_server_send_hyperbola"
#define RPC_SLOT_SERVER_SEND_QUALITY_STATUS			"rpc_slot_server_send_quality_status"
#define RPC_SLOT_SERVER_SEND_SOLVER_ERRORS			"rpc_slot_server_send_solver_errors"

///PRM to RPC client
#define RPC_SLOT_SERVER_PRM_STATUS                   "rpc_slot_server_prm_status"
#define RPC_PRM_FREQUENCY_CHANGED                    "rpc_prm_frequency_changed"

///ATLANT
#define RPC_SLOT_SERVER_ATLANT_DIRECTION            "rpc_slot_server_atlant_direction"
#define RPC_SLOT_SERVER_ATLANT_POSITION             "rpc_slot_server_atlant_position"

/// ATLANT from client
#define RPC_METHOD_SET_ATLANT_FREQUENCY             "rpc_slot_set_atlant_frequency"

/// BPLA
#define RPC_SLOT_SERVER_SEND_BPLA_DEF               "rpc_slot_server_send_bpla_def"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_1               "rpc_slot_server_send_bpla_def_1"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO          "rpc_slot_server_send_bpla_def_auto"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_SINGLE        "rpc_slot_server_send_bpla_def_single"

/// connectiont status
#define RPC_SLOT_SERVER_STATUS                      "rpc_slot_server_status"
#define RPC_SLOT_FLAKON_STATUS						"rpc_slot_flakon_status"
#define RPC_SLOT_FLAKON_DEV_STATE						"rpc_slot_flakon_dev_state"
#define RPC_METHOD_REQUEST_STATUS                   "rpc_slot_request_status"
#define RPC_METHOD_FLAKON_REQUEST_STATUS            "rpc_slot_flakon_request_status"
#define RPC_SLOT_PRM300_FREQUENCY_CHANGED           "rpc_slot_prm300_frequency_changed"

/// to solver
#define RPC_METHOD_SET_DATA_TO_SOLVER               "rpc_slot_set_data_to_solver"
#define RPC_METHOD_SET_CLEAR_TO_SOLVER              "rpc_slot_set_clear_to_solver"

#include <QString>

typedef struct A_Dir_Ans_msg
{
	//id запроса
	int requestId;
	//id источника, при ответе на DirectionFindingRequest совпадает с айди запроса
	//              при ответе на ScanRequest назначается пеленгатором
	int sourceId;
	//время засечки
	//DateTime.Utc.Now - время в 100наносекундных интервалах начиная с 1 января 1 года
	quint64 dateTime;

	//имя поста
	QString post;
	double postLatitude;     //GPS Geo Coords
	double postLongitude;    //GPS Geo Coords
	double postHeight;       //meters

	//центральная частота
	double frequency;
	//ширина пика
	double widht;

	//пеленг
	double direction;
	//угол места
	double angle;
	//уровень
	double level;
	//качество
	double quality;

	int motionType;
	double motionConfidence;
}A_Dir_Ans_msg;

// POSITION_ANSWER_MESSAGE
typedef struct A_Pos_Ans_msg
{
	//id запроса
	int requestId;
	//id источника, при ответе на DirectionFindingRequest совпадает с айди запроса
	//              при ответе на ScanRequest назначается пеленгатором
	int sourceId;
	//время засечки
	quint64 dateTime;

	 //Geo Coords
	double longitude;
	 //Geo Coords
	double latitude;
	 //качество
	double quality;
}A_Pos_Ans_msg;

#endif // RPCDEFINES_H
