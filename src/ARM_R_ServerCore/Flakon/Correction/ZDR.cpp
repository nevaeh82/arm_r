#include "ZDR.h"


#include <QDebug>

ZDR::ZDR(QObject *parent) :
    QObject(parent)
{
    //Р”Р»СЏ РєРѕРѕСЂРґРёРЅР°С‚
    mCoord2 = new ZCoord();

    //Р”Р»СЏ РєРѕСЂСЂРµРєС‚РёСЂРѕРІРєРё СЃРёСЃС‚РµРјР°С‚РёРєРё
    mCorrector2 = new ZCorrector();
}


void ZDR::getDataFromFlackon(int aNumMain, QVector<QVector<QPointF> > aCorrel, float aThreshold, QVector<double> &lDR)
{
    bool isCount=true;

    //Р’РµРєС‚РѕСЂ РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё РґР°РЅРЅС‹С…
    QVector<float> aBenchmark;

    //Р’РµРєС‚РѕСЂ СЂР°Р·РЅРѕСЃС‚РµР№ СЂР°СЃСЃС‚РѕСЏРЅРёР№
    QVector<double> aDR;

    if (aCorrel.size()<5) isCount=false; //Р•СЃР»Рё РЅРµС‚ 5С‚Рё СЂР°Р·РЅРѕСЃС‚РµР№ - РІС‹С…РѕРґРёРј
    if (aNumMain<0 || aNumMain>5) isCount=false; //Р•СЃР»Рё РЅРµРІРµСЂРµРЅ РЅРѕРјРµСЂ РѕСЃРЅРѕРІРЅРѕРіРѕ РїСѓРЅРєС‚Р° - РІС‹С…РѕРґРёРј
    if (aThreshold<0 || aThreshold>10000) aThreshold=0;

//    qDebug()<<"РљРѕСЂСЂРµР»СЏС†РёР№ РїСЂРёС€Р»Рѕ "<<aCorrel.size();
    if (isCount)
    {

//        qDebug()<<"РљРѕСЂСЂРµР»СЏС†РёР№ РїСЂРёС€Р»Рѕ "<<aCorrel.size();
        //РџРµСЂРµР±РѕСЂ РІСЃРµС… РєРѕСЂСЂРµР»СЏС†РёР№, РЅР°С…РѕР¶РґРµРЅРёРµ РјР°РєСЃРёРјСѓРјР° Рё РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё
        for (int i=0;i<(mCoord2->mDataOfPoints.aHeight.size()-1);i++)
        {
            QVector<QPointF> aLine=aCorrel.at(i);

            //Р”Р»СЏ РїРѕРёСЃРєР° РјР°РєСЃРёРјСѓРјР° Рё РєСЂРёС‚РµСЂРёСЏ РєРѕСЂСЂРµР»СЏС†РёРё
            float aTempForMax=-9999;
            float aCurrentDr;
            float aTempBenchmark;

            //Р”Р»СЏ РЅР°С…РѕР¶РґРµРЅРёСЏ РњРћ
            float aSum=0;
            float aMean;

            //Р’С‹С‡РёСЃР»РёС‚СЊ РЅРѕРјРµСЂ РіСЂР°С„РёРєР° РєРѕСЂСЂРµР»СЏС†РёРё

            //РќРѕРјРµСЂ РїРµСЂРІРѕРіРѕ РїСѓРЅС‚РєР°
            int aNumA=aNumMain;

            //РќРѕРјРµСЂ РІС‚РѕСЂРѕРіРѕ РїСѓРЅРєС‚Р°
            int aNumB;
            if (aNumMain>i) aNumB=i;
            else aNumB=i+1;

            //Р’РЅРµСЃС‚Рё РїРѕРїСЂР°РІРєСѓ Рё РЅР°Р№С‚Рё РјР°РєСЃРёРјСѓРј
//            qDebug()<<"РџРѕРїСЂР°РІРєРё РґР»СЏ РєРѕСЂСЂРµР»СЏС†РёРё -"<<mCoord2->getPointsDistance(aNumB)<<"   в„–"<<aNumB;
//            qDebug()<<"РџРѕРїСЂР°РІРєРё РґР»СЏ РєРѕСЂСЂРµР»СЏС†РёРё +"<<mCoord2->getPointsDistance(aNumA)<<"   в„–"<<aNumA;

            for (int p=0;p<30000;p++)
            {
                aLine.replace(p, QPointF(aLine.at(p).x()-(mCoord2->getPointsDistance(aNumB)-mCoord2->getPointsDistance(aNumA)),aLine.at(p).y()));
                if (aTempForMax<aLine.at(p).y())
                {
                    aTempForMax=aLine.at(p).y();
                    aCurrentDr=aLine.at(p).x();
                }
                aSum=aSum+aLine.at(p).y();
            }
            aMean=aSum/30000;
//            qDebug()<< "Amean = " << i << aMean;
//            qDebug()<< "aTempForMax = " << i << aTempForMax;
//            qDebug()<< "aCurrentDr = " << i << aCurrentDr;



            //Р Р°СЃСЃС‡РёС‚Р°С‚СЊ РєСЂРёС‚РµСЂРёР№ РєРѕСЂСЂРµР»СЏС†РёРё
            if (aTempForMax<0.03) aTempBenchmark=((aTempForMax-aMean)/(aMean*10));
            else if ((aTempForMax<0.1) && (aTempForMax>0.03)) aTempBenchmark=((aTempForMax-aMean)/(aMean));
            else if (aTempForMax>=0.1) aTempBenchmark=((aTempForMax-aMean)/(aMean))*(aTempForMax*10);

            //Р’С‹С‡РёСЃР»РµРЅРЅС‹Рµ РїРѕ РіСЂР°С„РёРєСѓ РєРѕСЂСЂРµР»СЏС†РёРё СЂР°Р·РЅРѕСЃС‚Рё Рё РґРѕСЃС‚РѕРІРµСЂРЅРѕСЃС‚Рё
            aBenchmark.append(aTempBenchmark);
            aDR.append(aCurrentDr);
        }
        if (aDR.size()==5)
        {

        //РџСЂРѕРІРµСЂРєР° РєРѕСЂСЂРµРєС‚РЅРѕСЃС‚Рё РґР°РЅРЅС‹С…
        mCoord2->checkData(aNumMain, aDR, aBenchmark, aThreshold);

        //РљРѕСЂСЂРµРєС‚РёСЂРѕРІРєР° СЃРёСЃС‚РµРјР°С‚РёС‡РµСЃРєРёС… РѕС€РёР±РѕРє

        mCorrector2->correctDr(aNumMain, aDR);
        }
    }
    lDR.clear();

    for (int i=0; i<aDR.size(); i++) lDR.append(aDR.at(i));
}
