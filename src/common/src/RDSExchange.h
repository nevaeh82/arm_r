#pragma once

#include <QByteArray>

#include "RdsPacket.pb.h"

inline void createGetSystemSystemOptions(RdsProtobuf::Packet& msg)
{
	RdsProtobuf::System* sMsg = msg.mutable_from_client()->mutable_get()->mutable_system();

	sMsg->mutable_options()->set_title("");

	//sMsg->mutable_options()->mutable_devices();
}

inline void createGetLocationStatus(RdsProtobuf::Packet& msg, RdsProtobuf::ClientMessage_OneShot_Location& data)
{
	RdsProtobuf::ClientMessage_OneShot_Location* oMsg = msg.mutable_from_client()->mutable_one_shot()->mutable_location();
//	oMsg->CopyFrom( data );
//	oMsg->mutable_range()->CopyFrom( data.range() );


	oMsg->set_duration(10);
	oMsg->set_central_frequency(20);
	RdsProtobuf::Range* range = oMsg->mutable_range();
	range->set_start(10);
	range->set_end(30);
	oMsg->set_convolution(true);
	oMsg->set_doppler(false);
	oMsg->set_convolution_plot(true);
	oMsg->set_hump(false);
	oMsg->set_averaging_frequency_band(50);
}

inline void createGetAnalysisStatus(RdsProtobuf::Packet& msg)
{
//	RdsProtobuf::Analysis_AnalysisOptions* oMsg = msg.mutable_from_client()->mutable_get()->mutable_analysis()->mutable_options();
//	oMsg->set_detector_index(0);
//	oMsg->set_duration(0);
//	oMsg->set_central_frequency(0);
//	oMsg->set_analysis(true);

//	RdsProtobuf::TimeFreqArea* selected = oMsg->mutable_selected_area();
//	selected->set_time_start( 10 );
//	selected->set_time_end( 20 );
//	selected->set_freq_start( 10 );
//	selected->set_freq_end( 20 );

//	RdsProtobuf::TimeFreqArea* zoomed = oMsg->mutable_zoomed_area();
//	zoomed->set_time_start( 10 );
//	zoomed->set_time_end( 20 );
//	zoomed->set_freq_start( 10 );
//	zoomed->set_freq_end( 20 );
}

inline void createSetEnablePlatform(RdsProtobuf::Packet &msg, uint platform,
									bool enable)
{
	msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_device()->set_device_index(platform);
	msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_device()->set_status(enable);
}

inline void createSetEnableReceiver(RdsProtobuf::Packet &msg, uint platform,
									uint receiver, bool enable)
{
	msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver()->set_device_index(platform);
	msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver()->set_channel_index(receiver);
	msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver()->set_status(enable);
}

inline void createChangeMode(RdsProtobuf::Packet &msg, uint mode)
{
	//msg.mutable_from_client()->mutable_set()->mutable_mode()->set_index(mode);
}

inline void createEnableMode(RdsProtobuf::Packet &msg, bool mode)
{
	//msg.mutable_from_client()->mutable_set()->mutable_mode()->set_status(mode);
}

inline void createGetModeState(RdsProtobuf::Packet &msg)
{
	//msg.mutable_from_client()->mutable_get()->mutable_mode()->set_status(true);
}

inline void createGetMode(RdsProtobuf::Packet &msg)
{
	//msg.mutable_from_client()->mutable_get()->mutable_mode()->set_index(0);
}

inline bool isSetLocationStatus(RdsProtobuf::Packet& msg)
{
//	if(msg.has_from_client()) {
//		if(msg.from_client().has_set()) {
//			if(msg.from_client().set().has_location()) {
//				return true;
//			}
//		}
//	}

	return false;
}

inline bool isSetEnableReceiver(RdsProtobuf::Packet &msg)
{
//	if(msg.has_from_client() && msg.from_client().has_set() && msg.from_client().set().has_system()
//			&& msg.from_client().set().system().has_receiver() ) {
//		return true;
//	}

	return false;
}

inline bool isAnalysisAbsPhaseFreq(RdsProtobuf::Packet &msg)
{
//	if( msg.has_from_server() && msg.from_server().has_data() &&
//			msg.from_server().data().has_analysis_abs_phase_freq() ) {
//		return true;
//	}

	return false;
}

inline bool isAnalysisDetected(RdsProtobuf::Packet &msg)
{
//	if( msg.has_from_server() && msg.from_server().has_data() &&
//			msg.from_server().data().has_analysis_detected() ) {
//		return true;
//	}

	return false;
}

inline bool isAnalysisSpectrogram(RdsProtobuf::Packet &msg)
{
//	if( msg.has_from_server() && msg.from_server().has_data() &&
//			msg.from_server().data().has_analysis_spectrogram() ) {
//		return true;
//	}

	return false;
}

// =========================================  SYSTEM MESSAGES  ============================================

// --------------------------------------------------------------------------------
// System - System Options
inline bool isSystemSystemOptions( const RdsProtobuf::ServerMessage& msg )
{
	if( msg.has_current() && msg.current().has_system() && msg.current().system().has_options() ) {
		return true;
	}

	return false;
}

inline RdsProtobuf::System_SystemOptions getSystemSystemOptions( const RdsProtobuf::ServerMessage& msg )
{
	return msg.current().system().options();
}

// --------------------------------------------------------------------------------
// System - Device Options
inline bool isSystemDeviceOptions( const RdsProtobuf::ServerMessage& msg )
{
	if( msg.has_current() && msg.current().has_system() && msg.current().system().has_device() ) {
		return true;
	}

	return false;
}

inline RdsProtobuf::System_Device getSystemDeviceOptions( const RdsProtobuf::ServerMessage& msg )
{
	return msg.current().system().device();
}

// --------------------------------------------------------------------------------
// System - Receiver Options
inline bool isSystemReceiverOptions( const RdsProtobuf::ServerMessage& msg )
{
	if( msg.has_current() && msg.current().has_system() && msg.current().system().has_receiver() &&
		msg.current().system().receiver().has_settings() ) {
		return true;
	}

	return false;
}

// System - Receiver Status
inline bool isSystemReceiverStatus( const RdsProtobuf::ServerMessage& msg )
{
	if( msg.has_current() && msg.current().has_system() && msg.current().system().has_receiver() &&
		msg.current().system().receiver().has_status() ) {
		return true;
	}

	return false;
}

inline RdsProtobuf::System_Receiver getSystemReceiver( const RdsProtobuf::ServerMessage& msg )
{
	return msg.current().system().receiver();
}

// =======================================  SERVER MESSAGES   ==============================================

// --------------------------------------------------------------------------------
// Server - Server Location
inline bool isServerLocationShot( const RdsProtobuf::ServerMessage& msg )
{
	if( msg.has_one_shot_data() && msg.one_shot_data().has_location_data() ) {
		return true;
	}

	return false;
}

inline RdsProtobuf::ServerMessage_OneShotData_LocationData getServerLocationShot( const RdsProtobuf::ServerMessage& msg )
{
	return msg.one_shot_data().location_data();
}

// --------------------------------------------------------------------------------

// =====================================================================================

inline QByteArray pack(RdsProtobuf::Packet pkt)
{
	QByteArray data;
	int t = pkt.ByteSize();
	data.resize( pkt.ByteSize() );
	uint size = data.size();

	pkt.SerializeToArray( data.data(), size );

	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend("RdsPacket");

	return data;
}

inline RdsProtobuf::Packet unpack(const QByteArray& data)
{
	RdsProtobuf::Packet pkt;
	QByteArray inData = data;
	inData = inData.right( inData.size() - 13 ); //13 is RdsPacket+int

	pkt.ParseFromArray( inData.data(), inData.size() );

	return pkt;
}
