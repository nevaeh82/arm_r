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
#include <RadiolocationInterface.h>
#include <Solver.h>

#include <qmath.h>

/*!
    РљР»Р°СЃСЃ, РєРѕС‚РѕСЂС‹Р№ РѕРїСЂРµРґРµР»СЏРµС‚ РєРѕРѕСЂРґРёРЅР°С‚С‹ Рё РґСЂСѓРіРёРµ РїР°СЂР°РјРµС‚СЂС‹ РґРІРёР¶РµРЅРёСЏ РР Р
*/
class ZCoord : public QObject
{
    Q_OBJECT
public:
    //! РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ (Р·Р°РґР°СЋС‚СЃСЏ РєРѕРѕСЂРґРёРЅР°С‚С‹ РІСЃРµС… РїСѓРЅРєС‚РѕРІ)
    explicit ZCoord(QObject *parent = 0);

    struct DataOfPoints
    {
        //! РљРѕР»-РІРѕ РїСѓРЅРєС‚РѕРІ
        unsigned short aNumPoints;
        //! РљРѕРѕСЂРґРёРЅР°С‚С‹ РїСѓРЅРєС‚РѕРІ РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ.
        QVector<QString> aTitleOfPoint;
        //! РљРѕРѕСЂРґРёРЅР°С‚С‹ РїСѓРЅРєС‚РѕРІ РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ.
        QVector<QPointF> aCoordsOfPoint;
        //! Р’С‹СЃРѕС‚Р° РЅР°Рґ Р·РµРјРЅРѕР№ РїРѕРІРµСЂС…РЅРѕСЃС‚СЊСЋ Р°РЅС‚РµРЅРЅС‹ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
        QVector<double> aHeight;
        //! Р Р°СЃСЃС‚РѕСЏРЅРёСЏ РјРµР¶РґСѓ 0 Рё i РїСѓРЅРєС‚Р°РјРё (РґР»СЏ РёРЅРґРµРєСЃРѕРІ 0 aDistance = 0).
        QVector<float> aDistance;
    };

    //! РРЅРёС†РёР°Р»СЊР·Р°С†РёСЏ SOLVER
    void initSolver();

    /*!
        РџРµСЂРµРІРѕРґ РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёС… РєРѕРѕСЂРґРёРЅР°С‚ РІ РґРµРєР°СЂС‚РѕРІС‹ (РґР»СЏ 1 С‚РѕС‡РєРё).
        \param aGeogrCoord - РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ РєРѕРѕСЂРґРёРЅР°С‚С‹.
        \param lDecartCoord - ccС‹Р»РєР° РЅР° РґРµРєР°СЂС‚РѕРІС‹ РєРѕРѕСЂРґРёРЅР°С‚С‹.
        \param aCentrCoord - РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ РєРѕРѕСЂРґРёРЅР°С‚С‹ С†РµРЅС‚СЂР°Р»СЊРЅРѕ С‚РѕС‡РєРё, РєРѕС‚РѕСЂР°СЏ СЃРѕРѕС‚РІРµС‚СЃРІСѓРµС‚ С‚РѕС‡РєРµ РІ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚Р°С… (0,0).
    */
    void geogrToDecart(QPointF aGeogrCoord,QPointF &lDecartCoord, QPointF aCentrCoord);

    /*!
        Р—Р°РґР°С‚СЊ РєРѕР»-РІРѕ РІС‹С…РѕРґРЅС‹С… РґР°РЅРЅС‹С… Solver (РѕРіСЂР°РЅРёС‡РµРЅРёРµ 2000).
        \param aSize - РєРѕР»-РІРѕ РІС‹С…РѕРґРЅС‹С… РґР°РЅРЅС‹С… РґР»СЏ Solver.
    */
    void setSolverDataSize(int aSize);

    /*!
        Р—Р°РґР°С‚СЊ РєРѕР»-РІРѕ РґР°РЅРЅС‹С… РґР»СЏ Solver РїРѕ РєРѕС‚РѕСЂС‹Рј Р±СѓРґРµС‚ РѕРїСЂРµРґРµР»РµРЅРѕ СЃС‚РѕРёС‚ РёСЃС‚РѕС‡РЅРёРє РёР»Рё РґРІРёРіР°РµС‚СЃСЏ.
        \param aSize - РєРѕР»-РІРѕ РґР°РЅРЅС‹С… РґР»СЏ Solver.
    */
    void setSolverAnalyzeSize(int aSize);


    //! Р”Р°РЅРЅС‹Рµ Рѕ РїСЂРёРјРµРЅС‹С… РїСѓРЅРєС‚Р°С…
    DataOfPoints mDataOfPoints;

signals:
    /*!
        РЎРёРіРЅР°Р» РґР»СЏ РѕС‚РїСЂР°РІРєРё РѕРґРЅРѕРіРѕ СЌР»РµРјРµРЅС‚Р° РІ Solver.
        \param aItem - РЈРєР°Р·Р°С‚РµР»СЊ РЅР° СЌР»РµРјРµРЅС‚.
    */
    void signal_sendDataFromFlackon(const DataFromFlacon& aData);

    /*!
        РЎРёРіРЅР°Р» РґР»СЏ РѕС‚РїСЂР°РІРєРё С‚РµРєСЃС‚Р° РЅР° С„РѕСЂРјСѓ.
        \param aText - С‚РµРєСЃС‚.
    */
    void signal_sendText(QString aText);
    
    /*!
        РЎРёРіРЅР°Р» РґР»СЏ РѕС‚РїСЂР°РІРєРё РґР°РЅРЅС‹С… Рѕ РєРѕРѕСЂРґРёРЅР°С‚Р°С… С†РµР»Рё РЅР° С„РѕСЂРјСѓ.
        \param allData - РґР°РЅРЅС‹Рµ.
    */
    void signal_sendCoordData(const DataFromRadioLocation& allData);

    /*!
        РЎРёРіРЅР°Р» РґР»СЏ РІРµРєС‚РѕСЂР° РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚РµР№ РЅР° С„РѕСЂРјСѓ.
        \param allData - РґР°РЅРЅС‹Рµ.
    */
    void signal_sendBenchmark(QVector<float>);

public slots:
    /*!
        РЎР»РѕС‚ РґР»СЏ РїРѕР»СѓС‡РµРЅРёСЏ РґР°РЅРЅС‹С… РёР· Solver.
        \param aData
    */
    void getDataFromSolver(const DataFromRadioLocation& aData);

    /*!
        Р”РѕСЃС‚Р°С‚СЊ СЂР°СЃСЃС‚РѕСЏРЅРёРµ РјРµР¶РґСѓ РЅСѓР»РµРІС‹Рј (РЅР° РєРѕС‚РѕСЂС‹Р№ РїСЂРёС…РѕРґСЏС‚ РІСЃРµ СЃРёРіРЅР°Р»С‹) Рё aNumOfPoint РїСѓРЅРєС‚Р°РјРё.
        \param aNumOfPoint - РЅРѕРјРµСЂ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
        \return - СЂР°СЃСЃС‚РѕСЏРЅРёРµ РјРµР¶РґСѓ РѕСЃРЅРѕРІРЅС‹Рј Рё aNumOfPoint РїСѓРЅРєС‚Р°РјРё.
    */
    float getPointsDistance(int aNumOfPoint);

    /*!
        Р”РѕСЃС‚Р°С‚СЊ СЂР°СЃСЃС‚РѕСЏРЅРёРµ РјРµР¶РґСѓ 2РјСЏ РїСѓРЅРєС‚Р°РјРё.
        \param aNumOfPoint - РЅРѕРјРµСЂ РїРµСЂРІРѕРіРѕ РїСѓРЅРєС‚Р°.
        \param aNumOfPoint - РЅРѕРјРµСЂ РІС‚РѕСЂРѕРіРѕ РїСѓРЅРєС‚Р°.
        \return - СЂР°СЃСЃС‚РѕСЏРЅРёРµ РјРµР¶РґСѓ 2РјСЏ РїСѓРЅРєС‚Р°РјРё.
    */
    float getPointsDistance(int aNumA, int aNumB);

    /*!
        РџРµСЂРµРІРѕРґ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚ РІ РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ (РґР»СЏ 1 С‚РѕС‡РєРё).
        \param aDecartCoord - РґРµРєР°СЂС‚РѕРІС‹ РєРѕРѕСЂРґРёРЅР°С‚С‹.
        \param lGeogrCoord - ccС‹Р»РєР° РЅР° РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ РєРѕРѕСЂРґРёРЅР°С‚С‹.
        \param aCentrCoord - РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ РєРѕРѕСЂРґРёРЅР°С‚С‹ С†РµРЅС‚СЂР°Р»СЊРЅРѕР№ С‚РѕС‡РєРё, РєРѕС‚РѕСЂР°СЏ СЃРѕРѕС‚РІРµС‚СЃРІСѓРµС‚ С‚РѕС‡РєРµ РІ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚Р°С… (0,0).
    */
    void decartToGeogr(QPointF aDecartCoord, QPointF &lGeogrCoord, QPointF aCentrCoord);

//    /*!
//        Р Р°СЃС‡РµС‚ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚ РґР»СЏ РІСЃРµС… РїСѓРЅРєС‚РѕРІ (СЃ СѓС‡РµС‚РѕРј РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р°)
//        \param aNumMainPoint - РЅРѕРјРµСЂ РѕСЃРЅРѕРІРЅРѕРіРѕ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
//    */
//    void countDecartCoord(unsigned int aNumMainPoint);

    /*!
        РџСЂРѕРІРµСЂРєР° РєРѕСЂСЂРµРєС‚РЅРѕСЃС‚Рё СЂР°Р·РЅРѕСЃС‚РµР№ СЂР°СЃСЃС‚РѕСЏРЅРёР№
        \param aNumMain - РЅРѕРјРµСЂ РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
        \param aDR - РІРµСЂРєС‚РѕСЂ СЂР°Р·РЅРѕСЃС‚РµР№ СЂР°СЃСЃС‚РѕСЏРЅРёР№.
        \param aBenchmark - РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё СЂР°Р·РЅРѕСЃС‚РµР№ СЂР°СЃСЃС‚РѕСЏРЅРёР№.
        \param aThreshold - РїРѕРїСЂРѕРі РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё.

    */
    void checkData(int aNumMain, QVector<double> &lDR, QVector<float> aBenchmark, float aThreshold);

//    /*!
//        РџСЂРёРЅСЏС‚СЊ РєРѕСЂСЂРµР»СЏС†РёРё РѕС‚ С„Р»Р°РєРѕРЅР°
//        \param aNumMain - РЅРѕРјРµСЂ РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
//        \param aCorrel - РІРµСЂРєС‚РѕСЂ РіСЂР°С„РёРєРѕРІ РєРѕСЂСЂРµР»СЏС†РёРё.
//        \param aThreshold - РїРѕРїСЂРѕРі РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё (РµСЃР»Рё 0 С‚Рѕ РЅРµ С„РёР»СЊС‚СЂСѓРµРј РїРѕ-РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё).

//    */
//    void getFromFlacon(int aNumMain, QVector<QVector<QPointF>> aCorrel, float aThreshold);

    /*!
     Р Р°СЃСЃС‚РѕСЏРЅРёРµ РјРµР¶РґСѓ 2РјСЏ С‚РѕС‡РєР°РјРё (РіРµРѕРіСЂ. РєРѕРѕСЂРґРёРЅР°С‚С‹)
     */
	double distanceAB(const QPointF& aCoordA, double aHgtA, const QPointF& aCoordB, double aHgtB);

    /*!
        Р”РѕСЃС‚Р°С‚СЊ РЅР°Р·РІР°РЅРёРµ Р Р”РЎ РїСѓРЅРєС‚Р° РїРѕ РёРЅРґРµРєСЃСѓ
    */
    QString getTitle(int aIndex);

    /*!
        Р”РѕСЃС‚Р°С‚СЊ РєРѕРѕСЂРґРёРЅР°С‚С‹ Р Р”РЎ РїСѓРЅРєС‚Р° РїРѕ РёРЅРґРµРєСЃСѓ
    */
    QPointF getCoord(int aIndex);

    /*!
        Р”РѕСЃС‚Р°С‚СЊ РєРѕР»-РІРѕ Р Р”РЎ РїСѓРЅРєС‚РѕРІ
    */
    int getSize(void);

private:
    /*!
        Р—Р°РґР°РЅРёРµ РґР°РЅРЅС‹С… Рѕ РїСЂРёРµРјРЅС‹С… РїСѓРЅРєС‚Р°С… (РІ РєРѕРґРµ).
        \param lNumPoints - ccС‹Р»РєР° РЅР° РєРѕР»-РІРѕ РїСЂРёРµРјРЅС‹С… РїСѓРЅРєС‚РѕРІ.
        \param lCoordsOfPoint - ccС‹Р»РєР° РЅР° РІРµРєС‚РѕСЂ, СЃРѕРґРµСЂР¶Р°С‰РёР№ С€РёСЂРѕС‚Сѓ Рё РґРѕР»РіРѕС‚Сѓ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
        \param lHeight - ccС‹Р»РєР° РЅР° РІРµРєС‚РѕСЂ РІС‹СЃРѕС‚ Р°РЅС‚РµРЅРЅС‹ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
    */
    void dataPoint(unsigned short &lNumPoints, QVector<QString> &lTitleOfPoint, QVector<QPointF> &lCoordsOfPoint, QVector<double> &lHeight);

    /*!
        Р—Р°РїРёСЃСЊ РґР°РЅРЅС‹С… Рѕ РїСЂРёРµРјРЅС‹С… РїСѓРЅРєС‚Р°С… РІ РіР»РѕР±Р°Р»СЊРЅСѓСЋ СЃС‚СЂСѓРєС‚СѓСЂСѓ mDataOfPoints
        \param aNumPoints - РєРѕР»-РІРѕ РїСЂРёРµРјРЅС‹С… РїСѓРЅРєС‚РѕРІ.
        \param aCoordsOfPoint - РІРµРєС‚РѕСЂ, СЃРѕРґРµСЂР¶Р°С‰РёР№ С€РёСЂРѕС‚Сѓ Рё РґРѕР»РіРѕС‚Сѓ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
        \param aHeight - РІС‹СЃРѕС‚Р° Р°РЅС‚РµРЅРЅС‹ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
        \param lDataOfPoints - СЃСЃС‹Р»РєР° РЅР° СЃС‚СЂСѓРєС‚СѓСЂСѓ СЃ РґР°РЅРЅС‹РјРё Рѕ РїСЂРёРјРµРЅС‹С… РїСѓРЅРєС‚Р°С….
    */
    void setDataPoint(unsigned short aNumPoints, QVector<QString> aTitleOfPoint, QVector<QPointF> aCoordsOfPoint, QVector<double> aHeight, DataOfPoints &lDataOfPoints);

    /*!
        Р Р°СЃСЃС‡РµС‚ СЂР°СЃСЃС‚РѕСЏРЅРёСЏ РјРµР¶РґСѓ РЅСѓР»РµРІС‹Рј (РЅР° РєРѕС‚РѕСЂС‹Р№ РїСЂРёС…РѕРґСЏС‚ РІСЃРµ СЃРёРіРЅР°Р»С‹) Рё i РїСЂРёРµРјРЅС‹Рј РїСѓРЅРєС‚РѕРј
        \param lDataOfPoints - СЃСЃС‹Р»РєР° РЅР° СЃС‚СЂСѓРєС‚СѓСЂСѓ СЃ РґР°РЅРЅС‹РјРё Рѕ РїСЂРёРјРµРЅС‹С… РїСѓРЅРєС‚Р°С….
    */
    void countDistanceOfPoints(DataOfPoints &lDataOfPoints);

    /*!
        РџРµСЂРµРІРѕРґ РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёС… РєРѕРѕСЂРґРёРЅР°С‚ РІ РіРµРѕСЂС†РµРЅС‚СЂРёС‡РµСЃРєРёРµ (РґР»СЏ РѕРїСЂРµРґРµР»РµРЅРёСЏ СЂР°СЃСЃС‚РѕСЏРЅРёР№)
        \param aCoordsOfPoint - РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёРµ РєРѕРѕСЂРґРёРЅР°С‚С‹ (lat, lon).
        \param aHeight - РІС‹СЃРѕС‚Р°, Рј.
        \param lX - СЃСЃС‹Р»РєР° РЅР° X РіРµРѕС†РµРЅС‚СЂРёС‡РµСЃРєРёС… РєРѕРѕСЂРґРёРЅР°С‚, Рј.
        \param lY - СЃСЃС‹Р»РєР° РЅР° Y РіРµРѕС†РµРЅС‚СЂРёС‡РµСЃРєРёС… РєРѕРѕСЂРґРёРЅР°С‚, Рј.
        \param lZ - СЃСЃС‹Р»РєР° РЅР° Z РіРµРѕС†РµРЅС‚СЂРёС‡РµСЃРєРёС… РєРѕРѕСЂРґРёРЅР°С‚, Рј.
    */
    void convertGeogrToGeocentric(QPointF aCoordsOfPoint, float aHeight, float &lX, float &lY, float &lZ);

    /*!
        Р Р°СЃСЃС‡РµС‚ СЂР°СЃСЃС‚РѕСЏРЅРёСЏ РјРµР¶РґСѓ 2 С‚РѕС‡РєР°РјРё
        \param aX1 - X 1Р№ С‚РѕС‡РєРё, Рј.
        \param aY1 - Y 1Р№ С‚РѕС‡РєРё, Рј.
        \param aZ1 - Z 1Р№ С‚РѕС‡РєРё, Рј.
        \param aX2 - X 2Р№ С‚РѕС‡РєРё, Рј.
        \param aY2 - Y 2Р№ С‚РѕС‡РєРё, Рј.
        \param aZ2 - Z 2Р№ С‚РѕС‡РєРё, Рј.
        \param lDistance - СЃСЃС‹Р»РєР° РЅР° СЂР°СЃСЃС‚РѕСЏРЅРёРµ РјРµР¶РґСѓ С‚РѕС‡РєР°РјРё РІ РјРµС‚СЂР°С….
    */
    void countDistanceOfPoints(float aX1, float aY1, float aZ1, float aX2, float aY2, float aZ2, float &lDistance);




//    /*!
//        Р Р°СЃС‡РµС‚ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚ РґР»СЏ РІСЃРµС… РїСѓРЅРєС‚РѕРІ (СЃ СѓС‡РµС‚РѕРј РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р°)
//        \param aCoordsOfPoint - РІРµРєС‚РѕСЂ РіРµРѕРіСЂР°С„РёС‡РµСЃРєРёС… РєРѕРѕСЂРґРёРЅР°С‚ (lat, lon) РІСЃРµС… РїСѓРЅРєС‚РѕРІ.
//        \param aNumMainPoint - РЅРѕРјРµСЂ РѕСЃРЅРѕРІРЅРѕРіРѕ РїСЂРёРµРјРЅРѕРіРѕ РїСѓРЅРєС‚Р°.
//        \param lDecartCoordsOfPoint - СЃСЃС‹Р»РєР° РЅР° РІРµРєС‚РѕСЂ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚ РІСЃРµС… РїСѓРЅРєС‚РѕРІ.
//    */
//    void countDecartCoord(QVector<QPointF> aCoordsOfPoint, unsigned int aNumMainPoint, QVector<QPointF> &lDecartCoordsOfPoint);

    //! Р Р°СЃС‡РµС‚ СЂР°СЃСЃС‚РѕСЏРЅРёСЏ СЂРµС‚СЂР°РЅСЃР»СЏС†РёРё РЎРђР‘Р•Р РРћ - РРќР“РЈР  - РўРђР“Р›РђРќ - Р“РђР›
    double retransmissionSaberio();







//    //! РџРѕСЂСЏРґРѕРє РѕР±СЂР°Р±РѕС‚РєРё РїСѓРЅРєС‚РѕРІ
//    QVector<unsigned short> mOrderProcessing;

//    //! Р”РµРєР°СЂС‚РѕРІС‹Р№ РєРѕРѕСЂРґРёРЅР°С‚С‹ РїСЂРёРµРјРЅС‹С… РїСѓРЅРєС‚РѕРІ (РёР·РјРµРЅСЏСЋС‚СЃСЏ РІ Р·Р°РІРёСЃРёРјРѕСЃС‚Рё РѕС‚ РІС‹Р±РѕСЂР° РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р° mOrderProcessing)
//    QVector<QPointF> mDecartCoords;

    //! РќРѕРјРµСЂ РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р°
    int mNumOfMainPoint;

    //! Р”Р»СЏ РєРѕСЂСЂРµРєС‚РёСЂРѕРІРєРё СЃРёСЃС‚РµРјР°С‚РёС‡РµСЃРєРёС… РѕС€РёР±РѕРє Рё РёРЅРІРµСЂСЃРёРё Р·РЅР°РєР°
    ZCorrector* mCorrector;

    //! Р”Р»СЏ РѕРїСЂРµРґРµР»РµРЅРёСЏ РєРѕРѕСЂРґРёРЅР°С‚
    Solver* mSolver;

    //! РљР»Р°СЃСЃ РґР»СЏ Р·Р°РїРёСЃРё Р»РѕРіРѕРІ
//    ZLogger *mLogger;
    QString mCurrentLogName;
    
};

#endif // ZCOORD_H
