#ifndef _EMS_CLIENT_H_
#define _EMS_CLIENT_H_

#include <memory>
#include <functional>
#include <list>
#include <string>
#include <vector>

#include "proto/EagleMessageProto.pb.h"
#include "proto/ServiceMessageProto.pb.h"

namespace EMS
{
	/// Interface for functor wrapper. Is used to abstract away from knowledge of concrete type
	/// of protobuf message.
	class IHandlerWrapper
	{
	public:
		/// Implementation parses protobuf message to concrete type and calls stored message handler.
		virtual void Handle(const void* data, const size_t length, long long time, const std::string& senderName, 
			const std::vector<std::string>& labels, const std::string& recipientName) = 0;
	};

	/// Functor wrapper which allows to store handler for protobuf message of concrete type, and calls this 
	/// handler after message is parsed
	template <typename T>
	class HandlerWrapper : public IHandlerWrapper
	{
	public:
		/// Concrete message type handler
		typedef std::function<void (T&, long long, const std::string&, const std::vector<std::string>&, const std::string&)> Handler;

		/// Constructor by handler
		HandlerWrapper(const Handler &handler_)	
			: handler(handler_)
		{
		}

		/// Parses protobuf message to concrete type and calls stored message handler.
		virtual void Handle(const void* data, const size_t length, long long time, const std::string& senderName, 
			const std::vector<std::string>& labels, const std::string& recipientName)
		{
			T t;
			bool result = t.ParseFromArray(data, length);
			if(!result)
			{
//				_err << "Error parse inner message" << endl_;
			}
			handler(t, time, senderName, labels, recipientName);
		};
	private:
		Handler handler;
	};

	class EmsClient
	{
	/// AEmsClient interface
	public:
		/// Connect to EMS server
		bool Connect(const std::string &ip, const unsigned short port, const std::string &name);
		/// Subscribe by label
		void SubscribeByLabel(std::string &label);
		/// Subscribe by labels
		void SubscribeByLabels(std::vector<std::string> &labels);
		/// Subscribe by name
		void SubscribeByName(std::string &name);
		/// Describe from subscription by labels
		void DescribeByLabels(std::vector<std::string> &labels );
		/// Handler of received message
		void ReceivedMessage(std::vector<char> &message);
	public:
		/// Transport connect state changed
		virtual void TransportConnectedChanged(bool connected);
		/// Public methods
	public:
		/// Constructor
		EmsClient();
		///
		~EmsClient(){};
		/// Register event handler
		template <typename T>
		void RegisterHandler(const std::string &type, 
			const typename HandlerWrapper<T>::Handler &func);

	private:
		void RegisterHandler(const std::string &type, std::shared_ptr<IHandlerWrapper> handler);
		void SendData(std::vector<char> &data, size_t size);
	protected:
		/// Send message
		template<typename T> void Send(const T &message, const std::string &type, const std::vector<std::string> &labels);

	/// Private methods
	private:
		/// Reconnect
		void Reconnect();
		/// Incoming service message handler
		void ServiceMessageHandler(ServiceMessage &message, long long time, 
			const std::string& senderName, const std::vector<std::string>& labels, const std::string& recipientName);
		/// Send message with type "SERVICE_MESSAGE"
		void SendServiceMessage(ServiceMessage &serviceMessage);
		/// Send connect message
		void SendConnectMessage();
		/// Send subscribe by name message
		void SendSubscribeNameMessage(const std::string &senderName, bool subscribe = true);
		/// Send subscribe labels message
		void SendSubscribeLabelsMessage(const std::vector<std::string> &labels, bool subscribe = true);
		/// Debug message
		void Debug(std::string &text);
		/// Log message
		void Log(std::string &text);
		/// Log message
		void Log(const char *text);
		/// Get time now
		long long GetTimeNow();
	/// Fields
	private:
		/// Connection controller
		//EmsConnectionController connectionController;
		struct Pimpl;
		Pimpl *pimpl;
		
		/// Client name
		std::string name;
		/// Ip address
		std::string ip;
		/// Port
		unsigned short port;
		/// Subscribed labels
		std::list<std::vector<std::string> > labels;
		/// Make reconnect
		bool reconnect;
		/// Is first connect to server? (for resubscribe. First one needn't subscribe)
		bool firstConnect;
	};

	template <typename T>
	void EMS::EmsClient::RegisterHandler(const std::string &type, const typename HandlerWrapper<T>::Handler &func)
	{
		RegisterHandler(type, std::shared_ptr<IHandlerWrapper>(new HandlerWrapper<T>(func)));
	}

	template<typename T>
	void EMS::EmsClient::Send(const T &message, const std::string &type, const std::vector<std::string> &labels/*, std::string &recipientName*/)
	{
		size_t size = message.ByteSize();
		std::vector<char> data(size);
		message.SerializeToArray(&data[0], size);

		EagleMessage eagleMessage;
		eagleMessage.set_type(type);
		eagleMessage.set_datetime(GetTimeNow());
		eagleMessage.set_sendername(name);
		int labelsCount = labels.size();
		for(size_t i = 0; i < labelsCount; i++)
		{
			std::string* s = eagleMessage.add_label();
			*s = labels[i];
		}
		//eagleMessage.set_recipientname(recipientName);
		eagleMessage.set_innermessage(&data[0], data.size());

		size_t len = eagleMessage.ByteSize();
		std::vector<char> buf(len);
		eagleMessage.SerializeToArray(&buf[0], len);

		SendData(buf, len);
	}

}

#endif // _EMS_CLIENT_H_
