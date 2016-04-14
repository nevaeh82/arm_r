﻿#include "ZCoord.h"

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
    float aX, aY, aZ; //Р С›РЎРѓР Р…Р С•Р Р†Р Р…Р С•Р в„– Р С—РЎС“Р Р…Р С”РЎвЂљ
    float aTempX, aTempY, aTempZ;
    float aTempDistance;

    QVector<float> aWire;//Р СџР С•Р С—РЎР‚Р В°Р Р†Р С”Р С‘ Р Р…Р В° Р Т‘Р В»Р С‘Р Р…РЎвЂ№ Р С—РЎР‚Р С•Р Р†Р С•Р Т‘Р С•Р Р†

    aWire.append(70);//Р вЂњР В°Р В»
    aWire.append(140);//Р СџР С‘РЎвЂЎР С•РЎР‚Р С‘
    aWire.append(140);//Р СњР В°Р В±Р В°Р С”Р ВµР Р†Р С‘
    aWire.append(140);//Р СћР В°Р С–Р В»Р В°Р Р…
    aWire.append(140);//Р С›РЎвЂЎР В°Р СРЎвЂЎР С‘РЎР‚Р В°
    aWire.append(420);//Р РЋР В°Р В±Р ВµРЎР‚Р С‘Р С•

    convertGeogrToGeocentric(lDataOfPoints.aCoordsOfPoint.at(0), lDataOfPoints.aHeight.at(0), aX, aY, aZ);//Р С›РЎРѓР Р…Р С•Р Р†Р Р…Р С•Р в„– Р С—РЎС“Р Р…Р С”РЎвЂљ, Р Р…Р В° Р С”Р С•РЎвЂљР С•РЎР‚РЎвЂ№Р в„– Р С—РЎР‚Р С‘РЎвЂ¦Р С•Р Т‘РЎРЏРЎвЂљ Р Р†РЎРѓР Вµ РЎРѓР С‘Р С–Р Р…Р В°Р В»РЎвЂ№
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


	for (int i = 0; i < 2; i++) //Р СњР С•Р СР ВµРЎР‚ Р С—РЎС“Р Р…Р С”РЎвЂљР В°, РЎРѓ Р С”Р С•РЎвЂљР С•РЎР‚РЎвЂ№Р С РЎРѓР Р†Р С•РЎР‚Р В°РЎвЂЎР С‘Р Р†Р В°Р ВµРЎвЂљРЎРѓРЎРЏ Р С•РЎРѓР Р…Р С•Р Р†Р Р…Р С•Р в„–
    {
		if (i != aNumMain)
        {
            int aTemp; //Р СњР С•Р СР ВµРЎР‚ РЎР‚Р В°Р В·Р Р…Р С•РЎРѓРЎвЂљР С‘
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

    //Р РЋР С’Р вЂР вЂўР  Р ВР С› - Р ВР СњР вЂњР Р€Р  
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(5), mDataOfPoints.aHeight.at(5), aTempX, aTempY, aTempZ);
    convertGeogrToGeocentric(igurCoord, ingurHgt, aX, aY, aZ);

    countDistanceOfPoints(aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance);
    aSumDist=aSumDist+aTempDistance;
//    qDebug()<<" Р РЋР С’Р вЂР вЂўР  Р ВР С› - Р ВР СњР вЂњР Р€Р   "<<aTempDistance;

    //Р ВР СњР вЂњР Р€Р   - Р СћР С’Р вЂњР вЂєР С’Р Сњ
    convertGeogrToGeocentric(igurCoord, ingurHgt, aTempX, aTempY, aTempZ);
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(3), mDataOfPoints.aHeight.at(3), aX, aY, aZ);

    countDistanceOfPoints(aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance);
    aSumDist=aSumDist+aTempDistance;
//    qDebug()<<" Р ВР СњР вЂњР Р€Р   - Р СћР С’Р вЂњР вЂєР С’Р Сњ "<<aTempDistance;

    //Р СћР С’Р вЂњР вЂєР С’Р Сњ - Р вЂњР С’Р вЂє
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(3), mDataOfPoints.aHeight.at(3), aTempX, aTempY, aTempZ);
    convertGeogrToGeocentric(mDataOfPoints.aCoordsOfPoint.at(0), mDataOfPoints.aHeight.at(0), aX, aY, aZ);

    countDistanceOfPoints(aTempX, aTempY, aTempZ, aX, aY, aZ, aTempDistance);
    aSumDist=aSumDist+aTempDistance;
//    qDebug()<<" Р СћР С’Р вЂњР вЂєР С’Р Сњ - Р вЂњР С’Р вЂє "<<aTempDistance;

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
