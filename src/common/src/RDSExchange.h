#pragma once

#include <QByteArray>
#include <QString>
#include <QObject>

#include "RdsPacket.pb.h"

inline void createGetSystemSystemOptions(RdsProtobuf::Packet& msg)
{
	RdsProtobuf::System* sMsg = msg.mutable_from_client()->mutable_get()->mutable_system();

	sMsg->mutable_options()->set_title("");

	//sMsg->mutable_options()->mutable_devices();
}

inline void createSetSystemSystemOptions(RdsProtobuf::Packet& msg, const RdsProtobuf::System_SystemOptions& sysOptions)
{
	msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_options()->CopyFrom(sysOptions);
}

inline void createGetLocationStatus(RdsProtobuf::Packet& msg, RdsProtobuf::ClientMessage_OneShot_Location& data)
{
	RdsProtobuf::ClientMessage_OneShot_Location* oMsg = msg.mutable_from_client()->mutable_one_shot()->mutable_location();
	oMsg->CopyFrom( data );
	oMsg->mutable_range()->CopyFrom( data.range() );
}

inline void createGetAnalysisStatus(RdsProtobuf::Packet& msg, RdsProtobuf::ClientMessage_OneShot_Analysis& data)
{
	RdsProtobuf::ClientMessage_OneShot_Analysis* aMsg = msg.mutable_from_client()->mutable_one_shot()->mutable_analysis();
	aMsg->CopyFrom( data );
}

inline void createTmpChannelOptions(RdsProtobuf::ChannelOptions* opt)
{
	opt->set_title(QString(QObject::tr("Channel")).toStdString());

	RdsProtobuf::ReceiverOptions* rec = opt->mutable_receiver();
	{
		rec->set_title(QString(QObject::tr("Receiver")).toStdString());
		rec->set_status(true);
		rec->set_ip(QString("127.0.0.1").toStdString());
		RdsProtobuf::ReceiverSettings* set = rec->mutable_settings();
		{
			set->set_frequency(20);
			set->set_attenuator1(0);
			set->set_attenuator2(0);
			set->set_filter(0);
		}
	}

	opt->set_inversion(false);
	RdsProtobuf::ChannelOptions_Coordinates* coord = opt->mutable_coordinates();
	coord->set_altitude(50);
	coord->set_latitude(60);
	coord->set_longitude(30);
}

inline void createTmpDeviceOptions(RdsProtobuf::DeviceOptions* opt)
{
	opt->set_title(QString(QObject::tr("Device")).toStdString());
	opt->set_status(true);
	opt->set_ip(QString("127.0.0.1").toStdString());
	opt->set_sync(0);
	opt->set_enabled(true);
	RdsProtobuf::ChannelOptions* channel = opt->add_channels();
	createTmpChannelOptions(channel);
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

// Server - Server Analysis
inline bool isServerAnalysisShot( const RdsProtobuf::ServerMessage& msg )
{
	if( msg.has_one_shot_data() && msg.one_shot_data().has_analysis_data() ) {
		return true;
	}

	return false;
}

inline RdsProtobuf::ServerMessage_OneShotData_AnalysisData getServerAnalysisShot( const RdsProtobuf::ServerMessage& msg )
{
	return msg.one_shot_data().analysis_data();
}

// --------------------------------------------------------------------------------

// =====================================================================================

inline bool parseServerMessage(const QByteArray& bData, RdsProtobuf::ServerMessage& sMsg)
{
	RdsProtobuf::Packet pkt;
	pkt.ParseFromArray( bData.data(), bData.size() );

	if(!pkt.has_from_server()) {
		return false;
	}

	sMsg = pkt.from_server();
	return true;
}

inline QByteArray pack(RdsProtobuf::Packet pkt)
{
	QByteArray data;
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
