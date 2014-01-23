#ifndef PRMCLIENT_H
#define PRMCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QTime>

#include <QtNetwork>

class PrmClient : public QObject
{
    Q_OBJECT
public:
    explicit PrmClient(const QString& strHost, int nPort, QObject* parent = 0);
    
signals:

    //! Отправка текста из клиента на форму
    void sendText(QString aText);

    /*!
        Отправка данных о частоте, аттенюаторах и фильтре, установленных в ПРМ300.
        \param aFreq - текущая частота
        \param aAt1 - аттенюатор 1 (0 = 0 Дб, 1 = 10 Дб)
        \param aAt2 - аттенюатор 2 (0..15 Дб)
        \param aFltr - фильтр (0..3, 2 = 140/20)
    */
    void sendPrmFreq(quint16 aFreq, quint8 aAt1, quint8 aAt2, quint8 aFltr);

    /*!
        Сигнал о подключении к железу.
        \param aState - флаг подключения (true - подключен)
    */
    void deviceConnected(bool aState);

    //! Сигнал об изменении частоты
    void freqChanged();

    //! Сигнал об изменении Аттенюатора1
    void attenuator1Changed();

    //! Сигнал об изменении Аттенюатора2
    void attenuator2Changed();

    //! Сигнал об изменении фильтра
    void filterChanged();
    
public slots:

    //! Слот на запрос частоты
    void getFreq();

    //! Слот на установку частоты
    void setFreq(quint16 aFreq);

    //! Слот на установку Аттенюатора1
    void setAttenuator1(quint8 aValue);

    //! Слот на установку Аттенюатора2
    void setAttenuator2(quint8 aValue);

    //! Слот на установку фильтра
    void setFiltr(quint8 aValue);

private slots:

    //! Слот на чтение
    void slotReadyRead();

    //! Слот на ошибку соединения
    void slotError(QAbstractSocket::SocketError);

    //! Слот на присоединение
    void slotConnected();

private:
    /*!
        Вычисление CRC8 + 1.
        \param aForCrcCount - вектор байт для вычисления CRC8 + 1
        \return результат вычисления CRC8 + 1
    */
    quint8 calcCRC(QVector<quint8> aForCrcCount);

    /*!
        Проверка наличия специальных символов и замена.
        \param lByteA - ссылка на первый байт
        \param lByteB - ссылка на второй байт
        \return произведена ли замена 2х байт на один (если true, то результат замены помещается в lByteA)
    */
    bool specialSymb(quint8 &lByteA, quint8 &lByteB);

private:
    //! Сокет
    QTcpSocket* mTcpSocket;

    //! Новая порция данных
    bool newData;

    //! Для замера времени ответа
    QTime mTimeStart, mTimeStop;
};

#endif // PRMCLIENT_H
