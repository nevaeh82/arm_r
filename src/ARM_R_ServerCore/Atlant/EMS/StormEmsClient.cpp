#include "pch.h"
#include "StormEmsClient.h"
using namespace EMS::Storm;

StormEmsClient::StormEmsClient() :
	messageTypeDirectionAnswer("DIRECTION_ANSWER_MESSAGE")
	, messageTypePositionAnswer("POSITION_ANSWER_MESSAGE")
	, messageTypeStatus("STATUS_MESSAGE")
{
	this->RegisterHandler<::Storm::DirectionAnswerMessage>(messageTypeDirectionAnswer, std::bind(&StormEmsClient::RaiseDirectionAnswerMessage, this, std::placeholders::_1));
	this->RegisterHandler<::Storm::PositionAnswerMessage>(messageTypePositionAnswer, std::bind(&StormEmsClient::RaisePositionAnswerMessage, this, std::placeholders::_1));
	this->RegisterHandler<::Storm::StatusMessage>(messageTypeStatus, std::bind(&StormEmsClient::RaiseStatusMessage, this, std::placeholders::_1));

	tag.push_back("Detector");
}

/// Запрос пеленгования одного источника
void StormEmsClient::DirectionFindingRequest(int id, std::vector<double> frequency, double width, std::string description)
{
	::Storm::DirectionFindingRequest request;
	request.set_id(id);
	for(int i = 0; i < frequency.size(); ++i)
	{
		request.add_frequency(frequency[i]);
	}
	request.set_width(width);
	request.set_description(description);
	Send(request, "DIRECTION_FINDING_REQUEST", tag);
}
/// Запрос остановки
void StormEmsClient::DFStopRequest(int id)
{
	::Storm::DFStopRequest request;
	request.set_id(id);
	Send(request, "DF_STOP_REQUEST", tag);
}
/// Запрос сканирования
void StormEmsClient::ScanRequest(int id, double frequency, double width)
{
	::Storm::ScanRequest request;
	request.set_id(id);
	request.set_frequency(frequency);
	request.set_width(width);
	Send(request, "SCAN_REQUEST", tag);

}
void StormEmsClient::ScanStopRequest(int id)
{
	::Storm::ScanStopRequest request;
	request.set_id(id);
	Send(request, "SCAN_STOP_REQUEST", tag);
}
void StormEmsClient::RaiseDirectionAnswerMessage(::Storm::DirectionAnswerMessage& message)
{
	OnDirectionAnswerMessage(message);
}
void StormEmsClient::RaisePositionAnswerMessage(::Storm::PositionAnswerMessage& message)
{
	OnPositionAnswerMessage(message);
}
void StormEmsClient::RaiseStatusMessage(::Storm::StatusMessage& message)
{
	OnStatusMessage(message);
}
