#ifndef _STORM_EMS_CLIENT_H
#define _STORM_EMS_CLIENT_H

#include "EmsClient.h"
#undef NO_ERROR
#include "storm.pb.h"

namespace EMS
{
	namespace Storm
	{
		class StormEmsClient : public EMS::EmsClient
		{
		/// Public methods
		public:
			/// Constructor
			StormEmsClient();
			
			/// Запрос пеленгования одного источника
			void DirectionFindingRequest(int id, std::vector<double> frequency, double width, std::string description = "");
			/// Запрос остановки
			void DFStopRequest(int id);
			/// Запрос сканирования
			void ScanRequest(int id, double frequency, double width);
			//// Запрос остановки сканирования
			void ScanStopRequest(int id);

		/// Events
		public:			
			std::function<void(::Storm::DirectionAnswerMessage&)> OnDirectionAnswerMessage;
			std::function<void(::Storm::PositionAnswerMessage&)> OnPositionAnswerMessage;
			std::function<void(::Storm::StatusMessage&)> OnStatusMessage;



		/// Raise enevts
		private:
			void RaiseDirectionAnswerMessage(::Storm::DirectionAnswerMessage&);
			void RaisePositionAnswerMessage(::Storm::PositionAnswerMessage&);
			void RaiseStatusMessage(::Storm::StatusMessage&); 

		private:							 
			/// Message type
			const std::string messageTypeDirectionAnswer;
			const std::string messageTypePositionAnswer;
			const std::string messageTypeStatus;

			std::vector<std::string> tag;
		};
	}
}
#endif _STORM_EMS_CLIENT_H
