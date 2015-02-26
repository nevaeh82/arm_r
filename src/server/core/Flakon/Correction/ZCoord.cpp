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

ZCoord::~ZCoord()
{
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

    lDataOfPoints.aNumPoints=aNumPoints;
    lDataOfPoints.aTitleOfPoint=aTitleOfPoint;
    lDataOfPoints.aCoordsOfPoint=aCoordsOfPoint;
    lDataOfPoints.aHeight=aHeight;
    countDistanceOfPoints(lDataOfPoints);
}

void ZCoord::countDistanceOfPoints(DataOfPoints &lDataOfPoints)
{
    float aX, aY, aZ; //РћСЃРЅРѕРІРЅРѕР№ РїСѓРЅРєС‚
    float aTempX, aTempY, aTempZ;
    float aTempDistance;

    QVector<float> aWire;//РџРѕРїСЂР°РІРєРё РЅР° РґР»РёРЅС‹ РїСЂРѕРІРѕРґРѕРІ

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
		if (i==5)
		{
			aTempDistance=retransmissionSaberio();
		}
		aTempDistance=aTempDistance+aWire.at(i);
        lDataOfPoints.aDistance.replace(i, -aTempDistance);
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


	for (int i = 0; i < mDataOfPoints.aHeight.size(); i++) //РќРѕРјРµСЂ РїСѓРЅРєС‚Р°, СЃ РєРѕС‚РѕСЂС‹Рј СЃРІРѕСЂР°С‡РёРІР°РµС‚СЃСЏ РѕСЃРЅРѕРІРЅРѕР№
    {
		if (i != aNumMain)
        {
            int aTemp; //РќРѕРјРµСЂ СЂР°Р·РЅРѕСЃС‚Рё
			if (i < aNumMain) {
				aTemp = i;
			} else {
				aTemp = i - 1;
			}

			aDistance = qAbs(getPointsDistance(aNumMain, i));
			if ((lDR.at(aTemp) < -aDistance*1.2) || (lDR.at(aTemp) > aDistance*1.2)) {
				lDR.replace(aTemp, std::numeric_limits<double>::quiet_NaN());
			}
			if (aBenchmark.at(aTemp) < aThreshold) {
				lDR.replace(aTemp, std::numeric_limits<double>::quiet_NaN());
			}
        }
    }
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
