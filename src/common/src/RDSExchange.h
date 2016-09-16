#include <QByteArray>

#include "RdsPacket.pb.h"

inline void createGetLocationStatus(RdsProtobuf::Packet& msg)
{
	RdsProtobuf::Location_LocationOptions* oMsg = msg.mutable_from_client()->mutable_get()->mutable_location()->mutable_options();
	oMsg->set_duration(0);
	oMsg->set_central_frequency(0);
	oMsg->set_convolution(0);
	oMsg->set_doppler(0);
	oMsg->set_averaging_frequency_band(0);
	oMsg->set_frequency_tuning_mode(2);
	oMsg->mutable_filter()->set_range(0);
	oMsg->mutable_filter()->set_shift(0);
}

inline void createSetLocationStatus(RdsProtobuf::Packet& msg, RdsProtobuf::Location& loc)
{
	msg.mutable_from_client()->mutable_set()->mutable_location()->CopyFrom( loc );
}

inline void createGetDetectorOptions(RdsProtobuf::Packet& msg, RdsProtobuf::Detector& det)
{
	msg.mutable_from_client()->mutable_get()->mutable_detector()->CopyFrom(det);
}

inline void createSetDetectorOptions(RdsProtobuf::Packet& msg, RdsProtobuf::Detector& det)
{
	msg.mutable_from_client()->mutable_set()->mutable_detector()->CopyFrom(det);
}

inline void createGetCorrectionOptions(RdsProtobuf::Packet& msg, RdsProtobuf::Correction& cor)
{
	msg.mutable_from_client()->mutable_get()->mutable_correction()->CopyFrom(cor);
}

inline void createSetCorrectionOptions(RdsProtobuf::Packet& msg, RdsProtobuf::Correction& cor)
{
	msg.mutable_from_client()->mutable_set()->mutable_correction()->CopyFrom(cor);
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

inline bool isSetEnableReceiver(RdsProtobuf::Packet &msg)
{
	if(msg.has_from_client() && msg.from_client().has_set() && msg.from_client().set().has_system()
			&& msg.from_client().set().system().has_receiver() ) {
		return true;
	}

	return false;
}

inline bool isAnalysisAbsPhaseFreq(RdsProtobuf::Packet &msg)
{
	if( msg.has_from_server() && msg.from_server().has_data() &&
		msg.from_server().data().has_analysis_abs_phase_freq() ) {
		return true;
	}

	return false;
}

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
