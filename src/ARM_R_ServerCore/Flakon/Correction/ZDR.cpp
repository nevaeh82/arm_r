#include "ZDR.h"
#include <Logger.h>

ZDR::ZDR(QObject *parent) :
    QObject(parent)
{
	mCoord2 = new ZCoord(this);
	mCorrector2 = new ZCorrector(this);
}

void ZDR::getDataFromFlackon(int aNumMain, QVector<QVector<QPointF> > aCorrel, float aThreshold, QVector<double> &lDR)
{
	bool isCount = true;

    QVector<float> aBenchmark;

    QVector<double> aDR;

	if (aCorrel.size() < 5) {
		isCount=false;
	}
	if (aNumMain < 0 || aNumMain > 5) {
		isCount=false;
	}
	if (aThreshold < 0 || aThreshold > 10000) {
		aThreshold=0;
	}

    if (isCount)
    {
		for (int i = 0; i < (mCoord2->mDataOfPoints.aHeight.size()-1); i++)
        {
			QVector<QPointF> aLine = aCorrel.at(i);
            float aTempForMax=-9999;
            float aCurrentDr;
            float aTempBenchmark;
            float aSum=0;
            float aMean;
			int aNumA = aNumMain;
            int aNumB;
			if (aNumMain > i) {
				aNumB = i;
			} else {
				aNumB = i + 1;
			}
			for (int p = 0; p < 30000; p++)
            {
                aLine.replace(p, QPointF(aLine.at(p).x()-(mCoord2->getPointsDistance(aNumB)-mCoord2->getPointsDistance(aNumA)),aLine.at(p).y()));
				if (aTempForMax<aLine.at(p).y()) {
                    aTempForMax=aLine.at(p).y();
                    aCurrentDr=aLine.at(p).x();
                }
                aSum=aSum+aLine.at(p).y();
            }
            aMean=aSum/30000;

			if (aTempForMax < 0.03) {
				aTempBenchmark=((aTempForMax-aMean)/(aMean*10));
			} else if ((aTempForMax < 0.1) && (aTempForMax > 0.03)) {
				aTempBenchmark=((aTempForMax-aMean)/(aMean));
			} else if (aTempForMax >= 0.1) {
				aTempBenchmark=((aTempForMax-aMean)/(aMean))*(aTempForMax*10);
			}

            aBenchmark.append(aTempBenchmark);
            aDR.append(aCurrentDr);
        }
		if (aDR.size() == 5)
		{
			mCoord2->checkData(aNumMain, aDR, aBenchmark, aThreshold);
			mCorrector2->correctDr(aNumMain, aDR);
        }
    }
    lDR.clear();

	for (int i = 0; i < aDR.size(); i++) {
		lDR.append(aDR.at(i));
	}
}
