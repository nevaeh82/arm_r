#include "pch.h"

#include "EmsClient.h"
#include "TcpClientConnection.h"
#include "IConnectionBuilder.h"
#include "TcpClientConnectionBuilder.h"
#include "EmsConnectionController.h"
using namespace EMS;

struct EmsClient::Pimpl
{
	EmsConnectionController connectionController;
	/// Handlers
	boost::unordered_map<std::string, std::shared_ptr<IHandlerWrapper> > handlers;
};



EmsClient::EmsClient() : 
		reconnect(true)
		, firstConnect(true)
{
	pimpl = new Pimpl();
	pimpl->connectionController.MessageReceivedSetHandler(boost::bind(&EmsClient::ReceivedMessage, this, _1));
	std::string s("SERVICE_MESSAGE");
	this->RegisterHandler<ServiceMessage>(s, boost::bind(&EmsClient::ServiceMessageHandler, this, _1, _2, _3, _4, _5));
}

void EmsClient::RegisterHandler(const std::string &type, std::shared_ptr<IHandlerWrapper> handler)
{
	pimpl->handlers[type] = handler;
}
void EmsClient::SendData(std::vector<char> &data, size_t size)
{
	pimpl->connectionController.SendData(&data[0], size);
}

bool EmsClient::Connect(const std::string &ip, const unsigned short port, const std::string &name)
{
	this->name = name;
	this->ip = ip;
	this->port = port;

	try
	{
		boost::shared_ptr<EMS::IConnectionBuilder> builder(
			(new EMS::TcpClientConnectionBuilder())->SetHost(ip)->SetPort(port));
		pimpl->connectionController.SetConnectionParams(builder);
		pimpl->connectionController.Start();
		SendConnectMessage();
	}
	catch(std::exception &ex)
	{
		std::string s(ex.what());
		Log(s);
		return false;
	}
	reconnect = true;
	return true;
}

void EmsClient::SubscribeByLabel(std::string &label)
{
	std::vector<std::string> v;
	v.push_back(label);
	SendSubscribeLabelsMessage(v);
	labels.push_back(v);
}

void EmsClient::SubscribeByLabels(std::vector<std::string> &labels)
{
	SendSubscribeLabelsMessage(labels);
	this->labels.push_back(labels);
}

void EmsClient::SubscribeByName(std::string &name)
{
	SendSubscribeNameMessage(name);
	std::vector<std::string> v;
	v.push_back(name);
	this->labels.push_back(v);
}

void EmsClient::ReceivedMessage( std::vector<char> &message )
{
	EagleMessage eagleMessage;
	bool res = eagleMessage.ParseFromArray(&message[0], message.size());
	if(!res)
	{
		std::string s("Error parse eagle message");
		Log(s);
		return;
	}
	boost::unordered_map<std::string, std::shared_ptr<IHandlerWrapper> >::iterator it = pimpl->handlers.find(eagleMessage.type());
	if(it != pimpl->handlers.end())
	{
		//(const void* data, const size_t length, long long time, std::string& senderName, std::vector<std::string>& labels, std::string& recipientName)
		std::vector<std::string> v;
		size_t size = eagleMessage.label_size();
		for(size_t i = 0; i < size; i++)
			v.push_back(eagleMessage.label(i));
		it->second->Handle(eagleMessage.innermessage().c_str(), eagleMessage.innermessage().size(),
			eagleMessage.datetime(), eagleMessage.sendername(), v, eagleMessage.innermessage());
	}
	else
	{
		Debug(std::string("Message handler not found"));
	}
}

void EmsClient::SendConnectMessage()
{
	try
	{
		ServiceMessage serviceMessage;
		serviceMessage.set_type(ServiceMessage_MessageType_CONNECT_MESSAGE);
		serviceMessage.mutable_connectmessage()->set_clientname(name);
		SendServiceMessage(serviceMessage);
	}
	catch(std::exception &ex)
	{
		Log(ex.what());
	}
}

void EmsClient::SendServiceMessage(ServiceMessage &serviceMessage)
{
	size_t size = serviceMessage.ByteSize();
	std::vector<char> data(size);
	serviceMessage.SerializeToArray(&data[0], size);

	EagleMessage eagleMessage;
	eagleMessage.set_type("SERVICE_MESSAGE");
	
	eagleMessage.set_datetime(GetTimeNow());
	eagleMessage.set_sendername(name);
	eagleMessage.set_innermessage(&data[0], data.size());
	size_t len = eagleMessage.ByteSize();
	std::vector<char> buf(len);
	eagleMessage.SerializeToArray(&buf[0], len);

	pimpl->connectionController.SendData(&buf[0], len);
}

void EmsClient::TransportConnectedChanged(bool state)
{
	if(state)
	{
		if(firstConnect)
		{
			firstConnect = false;
			return;
		}
		if(reconnect)
		{
			Reconnect();
		}
	}
	else
	{
		if(!reconnect)
		{
			pimpl->connectionController.Stop();
		}
	}
}

void EmsClient::SendSubscribeNameMessage(const std::string &senderName, bool subscribe)
{
	try
	{
		ServiceMessage serviceMessage;
		serviceMessage.set_type(ServiceMessage_MessageType_SUBSCRIBE_NAME_MESSAGE);
		serviceMessage.mutable_subscribenamemessage()->set_sendername(senderName);
		serviceMessage.mutable_subscribenamemessage()->set_subscribe(subscribe);
		SendServiceMessage(serviceMessage);
	}
	catch(std::exception &ex)
	{
		std::stringstream ss;
		ss << "Error serialization: " << ex.what();
		Log(ss.str());
	}
}
// T&, long long, const std::string&, const std::vector<std::string>&, const std::string&
void EmsClient::ServiceMessageHandler(ServiceMessage &message, long long time, 
				const std::string& senderName, const std::vector<std::string>& labels, const std::string& recipientName)
{
	switch(message.type())
	{
	case ServiceMessage_MessageType_CONNECT_MESSAGE:
		{
			std::string s("Connected to server");
			Debug(s);
			//Connected();
			break;
		}
	case ServiceMessage_MessageType_ERROR_MESSAGE:
		{
			std::stringstream ss;
			ss << "Error connected to server. Error code " << message.mutable_errormessage()->type() <<
				" : " + message.mutable_errormessage()->description();
			Log(ss.str());

			if(message.errormessage().noreconnect())
			{

			}

			break;
		}

	default:
		{
			
			Log("Unknown service message type");
			break;
		}
	}
}

void EmsClient::Debug(std::string &text)
{
	std::cout << text << std::endl;
}

void EmsClient::Log(std::string &text)
{
	std::cout << text << std::endl;
}

void EmsClient::Log(const char *text)
{
	Log(std::string(text));
}

void EmsClient::SendSubscribeLabelsMessage(const std::vector<std::string> &labels, bool subscribe)
{
	try
	{
		ServiceMessage serviceMessage;
		serviceMessage.set_type(ServiceMessage_MessageType_SUBSCRIBE_LABEL_MESSAGE);
		size_t size = labels.size();
		for(size_t i = 0; i < size; i ++)
		{
			std::string *s = serviceMessage.mutable_subscribelabelmessage()->add_labelname();
			*s = labels[i];
		}
		serviceMessage.mutable_subscribelabelmessage()->set_subscribe(subscribe);
		SendServiceMessage(serviceMessage);
	}
	catch(std::exception &ex)
	{
		std::stringstream ss;
		ss << "Error serialization: " << ex.what();
	}
}

void EMS::EmsClient::DescribeByLabels(std::vector<std::string> &labels_)
{	
	SendSubscribeLabelsMessage(labels_, false);
	struct F 
	{
		F(std::vector<std::string> &labels_) :
			labels(labels_)
		{
			this->labels = labels_;
		}
		bool operator ()(std::vector<std::string> it)
		{
			if(it.size() != labels.size())
				return false;
			for(size_t i = 0; i < it.size(); i++)
				if(it[i] != labels[i])
					return false;
			return true;
		}
		std::vector<std::string> &labels;
	};
	F f(labels_);
	labels.remove_if(f);
}

void EMS::EmsClient::Reconnect()
{
	SendConnectMessage();
	BOOST_FOREACH(std::vector<std::string> item, labels)
	{
		SendSubscribeLabelsMessage(item, true);
	}
}


long long EMS::EmsClient::GetTimeNow()
{
	static const long long lixuxEpoch = 621355968000000000; // ticks from 1.1.1 to 1970.1.1

	boost::posix_time::ptime myEpoch(boost::gregorian::date(1970,boost::gregorian::date::month_type(1), 1));
	auto now = boost::posix_time::second_clock::local_time();
	long long ticks = (now - myEpoch).total_microseconds() * 10 + lixuxEpoch;
	return ticks;
};