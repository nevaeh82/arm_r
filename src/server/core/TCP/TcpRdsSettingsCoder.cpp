#include "TcpRdsSettingsCoder.h"

TcpRdsSettingsCoder::TcpRdsSettingsCoder(QObject* parent) :
     BaseTcpDeviceCoder(parent)
{
}

TcpRdsSettingsCoder::~TcpRdsSettingsCoder()
{

}

//Decode to send Message to RDS
QByteArray TcpRdsSettingsCoder::decode(const MessageSP message)
{
	QString messageType = message->type();
	QByteArray dataToSend;

	QByteArray msgData = message->data();
	QDataStream stream(&msgData, QIODevice::ReadOnly);

	CommonParams::Parameters msg;

	if( message->type() == TCP_CCPACKET_SEND_PROTO ) {

		QSettings settings("./RDS/RDS_Settings.ini", QSettings::IniFormat, this);
		settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

		msg.ParseFromArray( msgData.data(), msgData.size() );

		int serverId = msg.serverid();

		settings.beginGroup(QString::number(serverId));

		if(msg.has_convolution())
		{
			settings.setValue("Convolution", msg.convolution());
		}
		if(msg.has_doppler())
		{
			settings.setValue("Doppler", msg.doppler());
		}
		if(msg.has_humps())
		{
			settings.setValue("Humps", msg.humps());
		}
		if(msg.has_frequency())
		{
			settings.setValue("Frequency", msg.frequency());
		}
		if(msg.has_startfreq())
		{
			settings.setValue("StartFrequency", msg.startfreq());
		}
		if(msg.has_finishfreq())
		{
			settings.setValue("FinishFrequency", msg.finishfreq());
		}
		if(msg.has_mergesysctrl())
		{
			settings.setValue("MergeSysCtrl", msg.mergesysctrl());
		}
		if(msg.has_mode())
		{
			settings.setValue("Mode", msg.mode());
		}
		if(msg.has_panorama())
		{
			settings.setValue("Panorama", msg.panorama());
		}
		if(msg.has_sleep())
		{
			settings.setValue("Sleep", msg.sleep());
		}
		if(msg.has_spectrums())
		{
			settings.setValue("Spectrum", msg.spectrums());
		}
		settings.endGroup();

		return msgData;
	}
	else {
		return QByteArray();
	}

	return QByteArray();
}

QByteArray TcpRdsSettingsCoder::encode(int id)
{
	QSettings settings("./RDS/RDS_Settings.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	CommonParams::Parameters params;
	QByteArray outData;

	QStringList childGroups = settings.childGroups();
	foreach (const QString &childKey, childGroups)
	{
		if(childKey.toInt() == id)
		{
			settings.beginGroup(childKey);
			params.set_serverid(id);
			params.set_convolution(settings.value("Convolution", "true").toBool());
			params.set_doppler(settings.value("Doppler", "false").toBool());
			params.set_humps(settings.value("Humps", "false").toBool());
			params.set_frequency(settings.value("Frequency", 20).toInt());
			params.set_startfreq(settings.value("StartFrequency", 20).toInt());
			params.set_finishfreq(settings.value("FinishFrequency", 300).toInt());
			params.set_mergesysctrl(settings.value("MergeSysCtrl", "false").toBool());
			params.set_mode(settings.value("Mode", 0).toInt());
			params.set_panorama(settings.value("Panorama", "false").toBool());
			params.set_sleep(settings.value("Sleep", "true").toBool());
			params.set_spectrums(settings.value("Spectrum", "true").toBool());
			settings.endGroup();

			outData.resize( params.ByteSize() );
			params.SerializeToArray( outData.data(), outData.size() );
		}
	}
	return outData;
}

MessageSP TcpRdsSettingsCoder::encode(const QByteArray& data)
{
     MessageSP message;

    //    QByteArray inData = getMessage(data);

    //    if(inData.isEmpty()) {
    //        return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, inData));
    //    }

    //    if(m_dataFromTcpSocket.length() >= m_residueLength) {
    //        message = messageFromPreparedData(inData);
    //    }

     return message;
}
