#ifndef ZDR_H
#define ZDR_H


#include <QObject>
#include <QPointF>
#include <QVector>
#include <QDebug>

#include "ZCoord.h"
#include "ZCorrector.h"

/*!
    Класс, который обрабатывает корреляции от флакона и выдает данные для расчета координат
*/
class ZDR : public QObject
{
    Q_OBJECT
public:
    explicit ZDR(QObject *parent = 0);
    
signals:
    //! Сигнал для отправки откорректированных разностей
    void signal_sendCorrectDR(int, QVector<double>);
    
public slots:

    //! Получение графиков корреляции от флакона
    void getDataFromFlackon(int aNumMain, QVector<QVector<QPointF> > aCorrel, float aThreshold, QVector<double> &lDR);

private slots:

private:
    //! Класс для расчета координат
    ZCoord *mCoord2;

    //! Для корректировки систематических ошибок и инверсии знака
    ZCorrector* mCorrector2;

};

#endif // ZDR_H
