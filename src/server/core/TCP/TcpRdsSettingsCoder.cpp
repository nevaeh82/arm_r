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

         QSettings settings("./RDS_Settings.ini", QSettings::IniFormat, this);
         settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

         msg.ParseFromArray( msgData.data(), msgData.size() );
         if(msg.has_spectrums())
         {
             bool isSpectrums = msg.spectrums();
             log_debug(QString("Spectrum = %1").arg(isSpectrums));
             settings.setValue("Spectrum", isSpectrums);
         }

         return msgData;
     }
     else {
         return QByteArray();
     }

     return QByteArray();
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
