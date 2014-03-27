#ifndef ZCOORD_H
#define ZCOORD_H

#include <QCoreApplication>
#include <QObject>
#include <QTextCodec>
#include <QPointF>
#include <QVector>
#include <QTime>
#include <QDebug>
#include <QSettings>

//#include "ZLogger.h"
#include "ZCorrector.h"
#include "../RadiolocationInterface.h"
#include "../Solver.h"

#include <qmath.h>

/*!
    Класс, который определяет координаты и другие параметры движения И� И
*/
class ZCoord : public QObject
{
    Q_OBJECT
public:
    //! Конструктор (задаются координаты всех пунктов)
    explicit ZCoord(QObject *parent = 0);

    struct DataOfPoints
    {
        //! Кол-во пунктов
        unsigned short aNumPoints;
        //! Координаты пунктов географические.
        QVector<QString> aTitleOfPoint;
        //! Координаты пунктов географические.
        QVector<QPointF> aCoordsOfPoint;
        //! Высота над земной поверхностью антенны приемного пункта.
        QVector<double> aHeight;
        //! � асстояния между 0 и i пунктами (для индексов 0 aDistance = 0).
        QVector<float> aDistance;
    };

    //! Инициальзация SOLVER
    void initSolver();

    /*!
        Перевод географических координат в декартовы (для 1 точки).
        \param aGeogrCoord - географические координаты.
        \param lDecartCoord - ccылка на декартовы координаты.
        \param aCentrCoord - географические координаты центрально точки, которая соответсвует точке в декартовых координатах (0,0).
    */
    void geogrToDecart(QPointF aGeogrCoord,QPointF &lDecartCoord, QPointF aCentrCoord);

    /*!
        Задать кол-во выходных данных Solver (ограничение 2000).
        \param aSize - кол-во выходных данных для Solver.
    */
    void setSolverDataSize(int aSize);

    /*!
        Задать кол-во данных для Solver по которым будет определено стоит источник или двигается.
        \param aSize - кол-во данных для Solver.
    */
    void setSolverAnalyzeSize(int aSize);


    //! Данные о применых пунктах
    DataOfPoints mDataOfPoints;

signals:
    /*!
        Сигнал для отправки одного элемента в Solver.
        \param aItem - Указатель на элемент.
    */
    void signal_sendDataFromFlackon(const DataFromFlacon& aData);

    /*!
        Сигнал для отправки текста на форму.
        \param aText - текст.
    */
    void signal_sendText(QString aText);
    
    /*!
        Сигнал для отправки данных о координатах цели на форму.
        \param allData - данные.
    */
    void signal_sendCoordData(const DataFromRadioLocation& allData);

    /*!
        Сигнал для вектора достоверностей на форму.
        \param allData - данные.
    */
    void signal_sendBenchmark(QVector<float>);

public slots:
    /*!
        Слот для получения данных из Solver.
        \param aData
    */
    void getDataFromSolver(const DataFromRadioLocation& aData);

    /*!
        Достать расстояние между нулевым (на который приходят все сигналы) и aNumOfPoint пунктами.
        \param aNumOfPoint - номер приемного пункта.
        \return - расстояние между основным и aNumOfPoint пунктами.
    */
    float getPointsDistance(int aNumOfPoint);

    /*!
        Достать расстояние между 2мя пунктами.
        \param aNumOfPoint - номер первого пункта.
        \param aNumOfPoint - номер второго пункта.
        \return - расстояние между 2мя пунктами.
    */
    float getPointsDistance(int aNumA, int aNumB);

    /*!
        Перевод декартовых координат в географические (для 1 точки).
        \param aDecartCoord - декартовы координаты.
        \param lGeogrCoord - ccылка на географические координаты.
        \param aCentrCoord - географические координаты центральной точки, которая соответсвует точке в декартовых координатах (0,0).
    */
    void decartToGeogr(QPointF aDecartCoord, QPointF &lGeogrCoord, QPointF aCentrCoord);

//    /*!
//        � асчет декартовых координат для всех пунктов (с учетом основного пункта)
//        \param aNumMainPoint - номер основного приемного пункта.
//    */
//    void countDecartCoord(unsigned int aNumMainPoint);

    /*!
        Проверка корректности разностей расстояний
        \param aNumMain - номер основного пункта.
        \param aDR - верктор разностей расстояний.
        \param aBenchmark - достоверности разностей расстояний.
        \param aThreshold - попрог достоверности.

    */
    void checkData(int aNumMain, QVector<double> &lDR, QVector<float> aBenchmark, float aThreshold);

//    /*!
//        Принять корреляции от флакона
//        \param aNumMain - номер основного пункта.
//        \param aCorrel - верктор графиков корреляции.
//        \param aThreshold - попрог достоверности (если 0 то не фильтруем по-достоверности).

//    */
//    void getFromFlacon(int aNumMain, QVector<QVector<QPointF>> aCorrel, float aThreshold);

    /*!
     � асстояние между 2мя точками (геогр. координаты)
     */
    double distanceAB(QPointF aCoordA, double aHgtA, QPointF aCoordB, double aHgtB);

    /*!
        Достать название � ДС пункта по индексу
    */
    QString getTitle(int aIndex);

    /*!
        Достать координаты � ДС пункта по индексу
    */
    QPointF getCoord(int aIndex);

    /*!
        Достать кол-во � ДС пунктов
    */
    int getSize(void);

private:
    /*!
        Задание данных о приемных пунктах (в коде).
        \param lNumPoints - ccылка на кол-во приемных пунктов.
        \param lCoordsOfPoint - ccылка на вектор, содержащий широту и долготу приемного пункта.
        \param lHeight - ccылка на вектор высот антенны приемного пункта.
    */
    void dataPoint(unsigned short &lNumPoints, QVector<QString> &lTitleOfPoint, QVector<QPointF> &lCoordsOfPoint, QVector<double> &lHeight);

    /*!
        Запись данных о приемных пунктах в глобальную структуру mDataOfPoints
        \param aNumPoints - кол-во приемных пунктов.
        \param aCoordsOfPoint - вектор, содержащий широту и долготу приемного пункта.
        \param aHeight - высота антенны приемного пункта.
        \param lDataOfPoints - ссылка на структуру с данными о применых пунктах.
    */
    void setDataPoint(unsigned short aNumPoints, QVector<QString> aTitleOfPoint, QVector<QPointF> aCoordsOfPoint, QVector<double> aHeight, DataOfPoints &lDataOfPoints);

    /*!
        � ассчет расстояния между нулевым (на который приходят все сигналы) и i приемным пунктом
        \param lDataOfPoints - ссылка на структуру с данными о применых пунктах.
    */
    void countDistanceOfPoints(DataOfPoints &lDataOfPoints);

    /*!
        Перевод географических координат в георцентрические (для определения расстояний)
        \param aCoordsOfPoint - географические координаты (lat, lon).
        \param aHeight - высота, м.
        \param lX - ссылка на X геоцентрических координат, м.
        \param lY - ссылка на Y геоцентрических координат, м.
        \param lZ - ссылка на Z геоцентрических координат, м.
    */
    void convertGeogrToGeocentric(QPointF aCoordsOfPoint, float aHeight, float &lX, float &lY, float &lZ);

    /*!
        � ассчет расстояния между 2 точками
        \param aX1 - X 1й точки, м.
        \param aY1 - Y 1й точки, м.
        \param aZ1 - Z 1й точки, м.
        \param aX2 - X 2й точки, м.
        \param aY2 - Y 2й точки, м.
        \param aZ2 - Z 2й точки, м.
        \param lDistance - ссылка на расстояние между точками в метрах.
    */
    void countDistanceOfPoints(float aX1, float aY1, float aZ1, float aX2, float aY2, float aZ2, float &lDistance);




//    /*!
//        � асчет декартовых координат для всех пунктов (с учетом основного пункта)
//        \param aCoordsOfPoint - вектор географических координат (lat, lon) всех пунктов.
//        \param aNumMainPoint - номер основного приемного пункта.
//        \param lDecartCoordsOfPoint - ссылка на вектор декартовых координат всех пунктов.
//    */
//    void countDecartCoord(QVector<QPointF> aCoordsOfPoint, unsigned int aNumMainPoint, QVector<QPointF> &lDecartCoordsOfPoint);

    //! � асчет расстояния ретрансляции САБЕ� ИО - ИНГУ�  - ТАГЛАН - ГАЛ
    double retransmissionSaberio();







//    //! Порядок обработки пунктов
//    QVector<unsigned short> mOrderProcessing;

//    //! Декартовый координаты приемных пунктов (изменяются в зависимости от выбора основного пункта mOrderProcessing)
//    QVector<QPointF> mDecartCoords;

    //! Номер основного пункта
    int mNumOfMainPoint;

    //! Для корректировки систематических ошибок и инверсии знака
    ZCorrector* mCorrector;

    //! Для определения координат
    Solver* mSolver;

    //! Класс для записи логов
//    ZLogger *mLogger;
    QString mCurrentLogName;
    
};

#endif // ZCOORD_H
