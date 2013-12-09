#include "ParserPRM300.h"
#include <QDebug>

ParserPRM300::ParserPRM300(IRouter *router, int id):
    _type(0),
    _restLength(0),
    _needBytes(false)
{

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);


    _id = id;
    _router = router;
    _subscriber = _router->get_subscriber();
}

ParserPRM300::~ParserPRM300()
{
    QTextStream(stdout) << "Terminated parser" << endl;
}

/// self distroy
void ParserPRM300::distruct()
{
    delete this;
}

void ParserPRM300::parsing(QByteArray data)
{
//    QTextStream(stdout) << trUtf8("� ћС‚ � їСЂ� ё� µ� ј� Ѕ� ё� є� ° � їСЂ� ёС€� »� ё � ґ� °� Ѕ� ЅС‹� µ") << endl;

    _msg.append(data);

    QVector<quint8> aForCrcCount; //� ”� »СЏ � їСЂ� ѕ� І� µСЂ� є� ё CRC8
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_7);
    quint8 aFirstByte, aLastByte, aCrc;
    quint8 aSizeAll;

    if(!_needBytes)
    {
        bool found = _find_magic();
        if(!found)
        {
            return;
        }
    }

    if (_msg.size() < 3 && _restLength == 0)
    {
        _needBytes = true;
        return;
    }

    if(_restLength == 0)
    {
        in >> aFirstByte;

        if (aFirstByte == 170)
        {
//            QTextStream(stdout) << "Найден стартовый байт" << endl;

            in >> _aAddr;
//            QTextStream(stdout) << "адрес передачи " << _aAddr << endl;

            in >> _aSize;
//            QTextStream(stdout) << "размер° " << _aSize << endl;

            _restLength = _aSize;

            in >> _aPacketType;
//            QTextStream(stdout) << "время ответа " << _aPacketType << endl;
        }
        else
        {
            return;
        }
    }

    //Проверка пришли ли остальные данные aSize
    if (_msg.size() < _restLength)
    {
        return;
    }

    //Все данные пришли - разбираем
    if (_aPacketType == 2)
    {
        QTextStream(stdout) << "Ответ об изменении частоты" << endl;
        quint16 aFlag;
        in >> aFlag;

//        QTextStream(stdout) << aFlag << endl;
//        emit freqChanged();
    }

    if (_aPacketType == 3)
    {
//        QTextStream(stdout) << "Ответ об изменении аттенюатора1 " << endl;

        quint16 aFlag;
        in >> aFlag;

//        QTextStream(stdout) << aFlag << endl;

//        emit attenuator1Changed();
    }

    if (_aPacketType == 4)
    {
//        QTextStream(stdout) <<  "Ответ о частоте"<< endl;
        quint16 aFreq;
        quint8 aAt1, aAt2, aAt3, aFltr, aFreqA, aFreqB, aFreqC, aFreqFirst, aFreqLast;
        bool aChangeCRCFlag = false; //� ¤� »� °� і � ѕ� ±СЂ� °С‚� Ѕ� ѕ� № � ·� °� ј� µ� ЅС‹ � І CRC

        if(aSizeAll != 12)
        {
            in >> aFreq; //� •СЃ� »� ё aSizeAll=12, С‚� ѕ � і� ґ� µ-С‚� ѕ � ЅСѓ� ¶� Ѕ� ѕ � ґ� µ� »� °С‚СЊ � ѕ� ±СЂ� °С‚� ЅСѓСЋ � ·� °� ј� µ� ЅСѓ СЃ� ї� µС� СЃ� ё� ј� І� ѕ� »� ѕ� І
        }
        else
        {
            //� ћ� ±СЂ� °С‚� Ѕ� °СЏ � ·� °� ј� µ� Ѕ� ° СЃ� ї� µС� СЃ� ё� ј� І� ѕ� »� ѕ� І � І С‡� °СЃС‚� ѕС‚� µ
//            QTextStream(stdout) << "� ћ� ±СЂ� °С‚� Ѕ� °СЏ � ·� °� ј� µ� Ѕ� ° СЃ� ї� µС� � ё� °� »СЊ� ЅС‹С… СЃ� ё� ј� І� ѕ� »� ѕ� І" << endl;

            in >> aFreqA >> aFreqB >> aFreqC;
            if (aFreqA == (0x5A))
            {
                if (aFreqB == 0x00)
                {
                    aFreqFirst = 0x5A;
                    aFreqLast = aFreqC;
                }
                else if (aFreqB == 0x01)
                {
                    aFreqFirst = 0x55;
                    aFreqLast = aFreqC;
                }
                else if (aFreqB == 0x02)
                {
                    aFreqFirst = 0xA5;
                    aFreqLast = aFreqC;
                }
                else if (aFreqB == 0x03)
                {
                    aFreqFirst = 0xAA;
                    aFreqLast = aFreqC;
                }

            } else if (aFreqB == (0x5A))
            {
                if (aFreqC == 0x00)
                {
                    aFreqFirst = aFreqA;
                    aFreqLast = 0x5A;
                }
                else if (aFreqC == 0x01)
                {
                    aFreqFirst = aFreqA;
                    aFreqLast = 0x55;
                }
                else if (aFreqC == 0x02)
                {
                    aFreqFirst = aFreqA;
                    aFreqLast = 0xA5;
                }
                else if (aFreqC == 0x03)
                {
                    aFreqFirst = aFreqA;
                    aFreqLast = 0xAA;
                }
            } else
            {
                aChangeCRCFlag = true;
            }

            //� ї� µСЂ� µ� І� ѕ� ґ 2С… quint8 � І quint16
            if (!aChangeCRCFlag)
            {
                aFreq = 256 * aFreqFirst + aFreqLast;
            }
            else
            {
                aFreq = 256 * aFreqA + aFreqB;
                aAt1 = aFreqC;
            }
        }

        if (!aChangeCRCFlag)
        {
            in >> aAt1 >> aFltr >> aAt2 >> aAt3 >> aLastByte;
        }
        else
        {
            //� ћ� ±СЂ� °С‚� Ѕ� °СЏ � ·� °� ј� µ� Ѕ� ° СЃ� ї� µС� СЃ� ё� ј� І� ѕ� »� ѕ� І � І CRC
            in >> aFltr >> aAt2 >> aAt3 >> aFreqA >> aFreqB;
            if (aFreqA == (0x5A))
            {
                if (aFreqB == 0x00)   aLastByte = 0x5A;
                else if (aFreqB == 0x01)  aLastByte = 0x55;
                else if (aFreqB == 0x02)  aLastByte = 0xA5;
                else if (aFreqB == 0x03)  aLastByte = 0xAA;
            }
        }
        QTextStream(stdout) << "GOT FREQ = " << _id << aFreq << aFltr << aAt1 << aAt2 << endl;

        _send_prm_status(aFreq, aFltr, aAt1, aAt2);


//        QTextStream(stdout) << "Частота " << aFreq << endl;
//        QTextStream(stdout) << "Фильтр " << aFltr << endl;
//        QTextStream(stdout) << "Аттенюатор1 " << aAt1 << endl;
//        QTextStream(stdout) << "Аттенюатор2 " << aAt2 << endl;
//        QTextStream(stdout) << "Аттенюатор3 " << aAt3 << endl;

            //� џСЂ� ѕ� І� µСЂ� є� ° CRC
            aForCrcCount.append(_aAddr);
            aForCrcCount.append(_aSize);
            aForCrcCount.append(_aPacketType);
            aForCrcCount.append((aFreq >> 8) & 0xFF); aForCrcCount.append(aFreq & 0xFF);
            aForCrcCount.append(aAt1);
            aForCrcCount.append(aFltr);
            aForCrcCount.append(aAt2);
            aForCrcCount.append(aAt3);

            CRCs crc;
            aCrc = crc.calcCRC(aForCrcCount);
//            QTextStream(stdout) << "Последний байт " << aLastByte << " CRC8 " << aCrc << endl;

//            emit sendPrmFreq(aFreq, aAt1, aAt2, aFltr);
        }

        if (_aPacketType==6)
        {
//            QTextStream(stdout) << "Ответ об изменении фильтра" << endl;

            quint16 aFlag;
            in >> aFlag;

//            QTextStream(stdout) << aFlag << endl;


//            emit filterChanged();
        }

        if (_aPacketType==7)
        {
//            QTextStream(stdout) << "Ответ об изменении аттенюатора 2" << endl;

            quint16 aFlag;
            in >> aFlag;

//            QTextStream(stdout) << aFlag << endl;

//            emit attenuator2Changed();
        }

        _msg.remove(0, _restLength);
        _restLength = 0;
//    }

}

/// search first ridth byte == 85 (0x55)
bool ParserPRM300::_find_magic()
{
    QDataStream in(&_msg, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_7);
    quint8 byte;
    for(int i = 0; i < _msg.size(); i++)
    {
        in >> byte;

        if(byte == 0xAA)
        {
            _msg.remove(0, i);
            _needBytes = true;
            return true;
        }
    }
    _needBytes = false;
    return false;
}


/// send freq to rpc
void ParserPRM300::_send_prm_status(quint16 freq, quint8 filter, quint8 att1, quint8 att2)
{
//    QTextStream(stdout) << "!!!!!!GOT FREQ = " << freq << filter << att1 << att2 << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << freq;
    ds << filter;
    ds << att1;
    ds << att2;

    QSharedPointer<IMessage> msg(new Message(_id, PRM_STATUS, ba));
    _subscriber->data_ready(PRM_STATUS, msg);
}
