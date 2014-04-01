#include "ZCoord.h"

ZCoord::ZCoord(QObject *parent) :
    QObject(parent)
{
    QTextCodec *pCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(pCodec);

    unsigned short aTempNumPoints;
    QVector<QString> aTitleOfPoint;
    QVector<QPointF> aTempCoordsOfPoint;
    QVector<double> aTempHeight;

    dataPoint(aTempNumPoints, aTitleOfPoint, aTempCoordsOfPoint, aTempHeight);
    setDataPoint(aTempNumPoints, aTitleOfPoint, aTempCoordsOfPoint, aTempHeight, mDataOfPoints);
}

void ZCoord::dataPoint(unsigned short &lNumPoints, QVector<QString> &lTitleOfPoint, QVector<QPointF> &lCoordsOfPoint, QVector<double> &lHeight)
{
    QString app_dir = QCoreApplication::applicationDirPath();
    app_dir.append("/RDS/RDSpoints.ini");
    QSettings aSettings(app_dir, QSettings::IniFormat);
    aSettings.setIniCodec("UTF-8");

    lNumPoints=aSettings.value("Amount/Amount").toInt();
    lTitleOfPoint.resize(lNumPoints); lCoordsOfPoint.resize(lNumPoints); lHeight.resize(lNumPoints);

    for (int i=0; i<lNumPoints; i++)
    {
        QString aTempSection=QString("Point_%1").arg(i);
        lTitleOfPoint.replace(i, aSettings.value(QString("%1/Title").arg(aTempSection)).toString());
        lCoordsOfPoint.replace(i,QPointF(aSettings.value(QString("%1/Lat").arg(aTempSection)).toDouble(),aSettings.value(QString("%1/Lon").arg(aTempSection)).toDouble()));
        lHeight.replace(i,aSettings.value(QString("%1/Hgt").arg(aTempSection)).toDouble());
    }
}

void ZCoord::setDataPoint(unsigned short aNumPoints, QVector<QString> aTitleOfPoint, QVector<QPointF> aCoordsOfPoint, QVector<double> aHeight, DataOfPoints &lDataOfPoints)
{
    lDataOfPoints.aCoordsOfPoint.resize(aNumPoints);
    lDataOfPoints.aTitleOfPoint.resize(aNumPoints);
    lDataOfPoints.aHeight.resize(aNumPoints);
    lDataOfPoints.aDistance.resize(aNumPoints);

    //РљРѕР»-РІРѕ РїСѓРЅРєС‚РѕРІ
    lDataOfPoints.aNumPoints=aNumPoints;
    //РќР°Р·РІР°РЅРёСЏ РїСѓРЅРєС‚РѕРІ
    lDataOfPoints.aTitleOfPoint=aTitleOfPoint;
    //РљРѕРѕСЂРґРёРЅР°С‚С‹ РїСѓРЅРєС‚РѕРІ
    lDataOfPoints.aCoordsOfPoint=aCoordsOfPoint;
    //Р’С‹СЃРѕС‚С‹ РїСѓРЅРєС‚РѕРІ
    lDataOfPoints.aHeight=aHeight;
    //Р Р°СЃСЃС‚РѕСЏРЅРёСЏ РјРµР¶РґСѓ 0 (РЅР° РєРѕС‚РѕСЂС‹Р№ РїСЂРёС…РѕРґСЏС‚ РІСЃРµ СЃРёРіРЅР°Р»С‹) Рё i РїСѓРЅРєС‚Р°РјРё
    countDistanceOfPoints(lDataOfPoints);
}

void ZCoord::countDistanceOfPoints(DataOfPoints &lDataOfPoints)
{
    float aX, aY, aZ; //РћСЃРЅРѕРІРЅРѕР№ РїСѓРЅРєС‚
    float aTempX, aTempY, aTempZ;
    float aTempDistance;

    QVector<float> aWire;//РџРѕРїСЂР°РІРєРё РЅР° РґР»РёРЅС‹ РїСЂРѕРІРѕРґРѕРІ
//    aWire.append(0);//РЎРѕС„СЊСЏ
//    aWire.append(0);//РўРѕРєСЃРѕРІРѕ
//    aWire.append(0);//Р’СЃРµРІРѕР»Р¶СЃРє

    aWire.append(70);//Р“Р°Р»
    aWire.append(140);//РџРёС‡РѕСЂРё
    aWire.append(140);//РќР°Р±Р°РєРµРІРё
    aWire.append(140);//РўР°РіР»Р°РЅ
    aWire.append(140);//РћС‡Р°РјС‡РёСЂР°
    aWire.append(420);//РЎР°Р±РµСЂРёРѕ

    convertGeogrToGeocentric(lDataOfPoints.aCoordsOfPoint.at(0), lDataOfPoints.aHeight.at(0), aX, aY, aZ);//РћСЃРЅРѕРІРЅРѕР№ РїСѓРЅРєС‚, РЅР° РєРѕС‚РѕСЂС‹Р№ РїСЂРёС…РѕРґСЏС‚ РІСЃРµ СЃРёРіРЅР°Р»С‹
    for (int i=0; i<lDataOfPoints.aNumPoints; i++)
    {
        convertGeogrToGeocentric(lDataOfPoints.aCoordsOfPoint.at(i), lDataOfPoints.aHeight.at(i), aTempX, aTempY, aTempZ);
        countDistanceOfPoints(aX, aY, aZ, aTempX, aTempY, aTempZ, aTempDistance);
//        if (i==5) aTempDistance=retransmissionSaberio(); //РџРћРџР РђР’РљРђ Р”Р›РЇ РЎРђР‘Р•Р РРћ

        //Р”РѕР±Р°РІРёС‚СЊ РїРѕРїСЂР°РІРєСѓ РЅР° РїСЂРѕРІРѕРґР°
        aTempDistance=aTempDistance+aWire.at(i);
        lDataOfPoints.aDistance.replace(i, -aTempDistance);
//        qDebug()<<"РЎРѕС„СЊСЏ - "<<i<<" "<<lDataOfPoints.aDistance.at(i)<<" РјРµС‚СЂРѕРІ";
    }
}

void ZCoord::convertGeogrToGeocentric(QPointF aCoordsOfPoint, float aHeight, float &lX, float &lY, float &lZ)
{
    float ERadius=6366197.724;
	float pi=M_PI;
    float d = cos(aCoordsOfPoint.x()*pi/180);
    float RzHeight = ERadius + aHeight;
    lX = RzHeight * d * cos(aCoordsOfPoint.y()*pi/180);
    lY = RzHeight * d * sin(aCoordsOfPoint.y()*pi/180);
    lZ = RzHeight * sin(aCoordsOfPoint.x()*pi/180);
}

void ZCoord::countDistanceOfPoints(float aX1, float aY1, float aZ1, float aX2, float aY2, float aZ2, float &lDistance)
{
    lDistance = sqrt((aX1 - aX2) * (aX1 - aX2) + (aY1 - aY2) * (aY1 - aY2) + (aZ1 - aZ2) * (aZ1 - aZ2));
}

//void ZCoord::countDecartCoord(QVector<QPointF> aCoordsOfPoint, unsigned int aNumMainPoint, QVector<QPointF> &lDecartCoordsOfPoint)
//{
//    unsigned int aNumOfPoints=aCoordsOfPoint.size();
//    qDebug()<<"РљРѕР»-РІРѕ РїСѓРЅРєС‚РѕРІ "<<aNumOfPoints;

//    QPointF aTemp;

//    lDecartCoordsOfPoint.clear();

//    for (int i=0; i<aNumOfPoints; i++)
//    {
//        geogrToDecart(aCoordsOfPoint.at(i), aTemp, aCoordsOfPoint.at(aNumMainPoint));
//        lDecartCoordsOfPoint.append(aTemp);
//        qDebug()<<"i="<<i<<" Lat="<<aCoordsOfPoint.at(i).x()<<" Lon="<<aCoordsOfPoint.at(i).y();
//        qDebug()<<"Р¦РµРЅС‚СЂ РІ 0 "<<lDecartCoordsOfPoint.at(i).x()<<" "<<lDecartCoordsOfPoint.at(i).y()<<" ";
//    }
//}

//void ZCoord::countDecartCoord(unsigned int aNumMainPoint)
//{
//    unsigned int aNumOfPoints=mDataOfPoints.aCoordsOfPoint.size();
//    QPointF aTemp;

//    mDecartCoords.clear();

//    for (int i=0; i<aNumOfPoints; i++)
//    {
//        geogrToDecart(mDataOfPoints.aCoordsOfPoint.at(i), aTemp, mDataOfPoints.aCoordsOfPoint.at(aNumMainPoint));
//        mDecartCoords.append(aTemp);
//        qDebug()<<"Р Р°СЃС‡РµС‚ РґРµРєР°СЂС‚РѕРІС‹С… РєРѕРѕСЂРґРёРЅР°С‚ "<<i<<" "<<" "<<mDecartCoords.at(i).x()<<" "<<mDecartCoords.at(i).y()<<" ";
//    }
//}

void ZCoord::geogrToDecart(QPointF aGeogrCoord,QPointF &lDecartCoord, QPointF aCentrCoord)
{
    float dY, dX, popravka;
    float pi=3.1415926535897932385;
    float ERadius=6366197.724;

    popravka=cos(aCentrCoord.x()*pi/180);

    dY=(aGeogrCoord.x()-aCentrCoord.x())*pi*ERadius/180;
    dX=(aGeogrCoord.y()-aCentrCoord.y())*pi*ERadius/180;
    lDecartCoord=QPointF(dX*popravka, dY);
}

void ZCoord::decartToGeogr(QPointF aDecartCoord, QPointF &lGeogrCoord, QPointF aCentrCoord)
{
    float pi=3.1415926535897932385;
    float ERadius=6366197.724;

    lGeogrCoord.setX(aCentrCoord.x() + aDecartCoord.y()*180/pi/ERadius);
    lGeogrCoord.setY(aCentrCoord.y() + aDecartCoord.x()*180/cos(aCentrCoord.x()*pi/180)/ERadius/pi);
}

float ZCoord::getPointsDistance(int aNumOfPoint)
{

    return mDataOfPoints.aDistance.at(aNumOfPoint);
}

float ZCoord::getPointsDistance(int aNumA, int aNumB)
{
    float aXA, aYA, aZA, aXB, aYB, aZB;
    float aDist;
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(aNumA), mDataOfPoints.aHeight.at(aNumA), aXA, aYA, aZA);
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(aNumB), mDataOfPoints.aHeight.at(aNumB), aXB, aYB, aZB);

    countDistanceOfPoints(aXA, aYA, aZA, aXB, aYB, aZB, aDist);

    return aDist;
}


void ZCoord::checkData(int aNumMain, QVector<double> &lDR, QVector<float> aBenchmark, float aThreshold)
{
    float aDistance;


    for (int i=0;i<mDataOfPoints.aHeight.size();i++) //РќРѕРјРµСЂ РїСѓРЅРєС‚Р°, СЃ РєРѕС‚РѕСЂС‹Рј СЃРІРѕСЂР°С‡РёРІР°РµС‚СЃСЏ РѕСЃРЅРѕРІРЅРѕР№
    {
        if (i!=aNumMain)
        {
            int aTemp; //РќРѕРјРµСЂ СЂР°Р·РЅРѕСЃС‚Рё
            if (i<aNumMain) aTemp=i;
            else aTemp=i-1;

//            qDebug()<<aTemp<<" СЂР°Р·РЅРѕСЃС‚СЊ РґРѕ РєРѕСЂСЂРµРєС‚РёСЂРѕРІРєРё "<<lDR.at(aTemp);


            aDistance=qAbs(getPointsDistance(aNumMain, i));
            if ((lDR.at(aTemp)<-aDistance*1.2) || (lDR.at(aTemp)>aDistance*1.2)) lDR.replace(aTemp, std::numeric_limits<double>::quiet_NaN());
            if (aBenchmark.at(aTemp)<aThreshold) lDR.replace(aTemp, std::numeric_limits<double>::quiet_NaN());
        }
    }
}


void ZCoord::getDataFromSolver(const DataFromRadioLocation& aData)
{

//    qDebug()<<"РџСЂРёС€РµР» СЃРёРіРЅР°Р» РѕС‚ РЎРѕР»РІРµСЂ";
    int aLastItem=aData.timeHMSMs.size()-1;
    if (aData.StateMassive_.at(aLastItem)==STAND) qDebug()<<("РСЃС‚РѕС‡РЅРёРє РЅРµРїРѕРґРІРёР¶РµРЅ");
    if (aData.StateMassive_.at(aLastItem)==FLY) qDebug()<<("РџРѕРґРІРёР¶РЅС‹Р№ РёСЃС‚РѕС‡РЅРёРє");
    if (aData.StateMassive_.at(aLastItem)==UNKNOWN) qDebug()<<("РўРёРї РёСЃС‚РѕС‡РЅРёРєР° РЅРµРѕРїСЂРµРґРµР»РµРЅ");
//    qDebug()<<(QString("Lat=%1    Lon=%2").arg(aData.coordLatLon.at(aLastItem).x()).arg(aData.coordLatLon.at(aLastItem).y()));
    if (aLastItem>20) qDebug()<<(QString("Р’С‹СЃРѕС‚Р°=%1").arg(aData.heigh.at(aLastItem-15)));
//    qDebug()<<(QString("РљСѓСЂСЃ=%1").arg(aData.relativeBearing.at(aLastItem)));
//    qDebug()<<(QString("РЎРєРѕСЂРѕСЃС‚СЊ=%1").arg(aData.airspeed.at(aLastItem)));
//    qDebug()<<(QString("\nР Р°Р·РјРµСЂ РґР°РЅРЅС‹С…=%1").arg(aLastItem+1));

    emit signal_sendCoordData(aData);
//    mLogger->saveCoord("Р»РѕРіРєРѕРѕСЂРґРёРЅР°С‚.txt", aData);
}

double ZCoord::retransmissionSaberio()
{
    QPointF igurCoord=QPointF(42.58371, 41.84906);
    float ingurHgt=130+30;

    float aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance;
    double aSumDist=0;

    //РЎРђР‘Р•Р РРћ - РРќР“РЈР 
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(5), mDataOfPoints.aHeight.at(5), aTempX, aTempY, aTempZ);
    convertGeogrToGeocentric(igurCoord, ingurHgt, aX, aY, aZ);

    countDistanceOfPoints(aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance);
    aSumDist=aSumDist+aTempDistance;
//    qDebug()<<" РЎРђР‘Р•Р РРћ - РРќР“РЈР  "<<aTempDistance;

    //РРќР“РЈР  - РўРђР“Р›РђРќ
    convertGeogrToGeocentric(igurCoord, ingurHgt, aTempX, aTempY, aTempZ);
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(3), mDataOfPoints.aHeight.at(3), aX, aY, aZ);

    countDistanceOfPoints(aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance);
    aSumDist=aSumDist+aTempDistance;
//    qDebug()<<" РРќР“РЈР  - РўРђР“Р›РђРќ "<<aTempDistance;

    //РўРђР“Р›РђРќ - Р“РђР›
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(3), mDataOfPoints.aHeight.at(3), aTempX, aTempY, aTempZ);
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(0), mDataOfPoints.aHeight.at(0), aX, aY, aZ);

    countDistanceOfPoints(aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance);
    aSumDist=aSumDist+aTempDistance;
//    qDebug()<<" РўРђР“Р›РђРќ - Р“РђР› "<<aTempDistance;

    return aSumDist;
}

void ZCoord::setSolverDataSize(int aSize)
{
//    if ((aSize>10) && (aSize<2000)) mSolver->SetOutDataLength(aSize);
}

void ZCoord::setSolverAnalyzeSize(int aSize)
{
//    if ((aSize>10) && (aSize<200)) mSolver->SetStateAnalizeCount(aSize);
}


void ZCoord::initSolver()
{
    //Solver
    if (mSolver) delete mSolver;
    mSolver = new Solver();

    mCurrentLogName=QTime::currentTime().toString("hh:mm:ss:zzz");

    //Р Р°Р·РјРµСЂ РїСЂРёС…РѕРґСЏС‰РёС… РґР°РЅРЅС‹С…
    setSolverDataSize(300);

    //РљРѕР»-РІРѕ РґР°РЅРЅС‹С… РґР»СЏ РѕРїСЂРµРґРµР»РµРЅРёСЏ РґРІРёР¶РµРЅРёСЏ
    //setSolverAnalyzeSize(10);

    connect(this, SIGNAL(signal_sendDataFromFlackon(const DataFromFlacon&)), mSolver, SLOT(GetData(const DataFromFlacon&)));
    connect(mSolver, SIGNAL(signal_sendDataFromRadioLocation(const DataFromRadioLocation&)), this, SLOT(getDataFromSolver(const DataFromRadioLocation&)));
}

double ZCoord::distanceAB(const QPointF& aCoordA, double aHgtA, const QPointF& aCoordB, double aHgtB)
{
    double aDistance;
    float aX_A, aY_A, aZ_A, aX_B, aY_B, aZ_B;
    convertGeogrToGeocentric(aCoordA, aHgtA, aX_A, aY_A, aZ_A);
    convertGeogrToGeocentric(aCoordB, aHgtB, aX_B, aY_B, aZ_B);
    aDistance=qSqrt((aX_A-aX_B)*(aX_A-aX_B)+(aY_A-aY_B)*(aY_A-aY_B)+(aZ_A-aZ_B)*(aZ_A-aZ_B));
    return aDistance;
}

QString ZCoord::getTitle(int aIndex)
{
    return mDataOfPoints.aTitleOfPoint.at(aIndex);
}

QPointF ZCoord::getCoord(int aIndex)
{
    return mDataOfPoints.aCoordsOfPoint.at(aIndex);
}

int ZCoord::getSize(void)
{
    return mDataOfPoints.aTitleOfPoint.size();
}
