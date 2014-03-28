#include "ZCorrector.h"

ZCorrector::ZCorrector(QObject *parent) :
    QObject(parent)
{
    mCorrectMatrix.resize(6*6);

    QString app_dir = QCoreApplication::applicationDirPath();
    app_dir.append("/RDS/DRCorrection.ini");
    QSettings aSet(app_dir, QSettings::IniFormat);
    //aSettings.setIniCodec("UTF-8");

    for (int i=0; i<6; i++)
    {
        for (int j=0; j<6; j++)
        {
//            if (i=j)
//            {
            setElement(i, j, aSet.value(QString("%1/%2").arg(i).arg(j)).toDouble());
//            }
        }
    }

//qDebug()<<"Отработал ZCorrector";
}

void ZCorrector::correctDr(int aMainIndex, QVector<double> &lDR)
{
    for (int i=0; i<2; i++)
    {
    int aSecondIndex;
    if (aMainIndex < (i+1)) aSecondIndex=i+1;
    else aSecondIndex=i;

    //Замена знака для отправки в solver
    double aOldDr=-lDR.at(i);
//    qDebug()<<"до корректировки "<<lDR.at(i);
    lDR.replace(i, aOldDr+getElement(aMainIndex, aSecondIndex));
//    qDebug()<<"после корректировки "<<lDR.at(i);
    }

}

void ZCorrector::setElement(int aX, int aY, double aElement)
{
    try
    {
        mCorrectMatrix.replace(6*aX+aY, aElement);// = aElement;
//        qDebug()<<"X Y Correct"<<aX<<" "<<aY<<" "<<aElement;
//        qDebug() << "!!!!!!!!!!!!!" << 6*aX+aY << mCorrectMatrix.size();
    }
    catch(...)
    {
//        qDebug() << this;
    }
}

double ZCorrector::getElement(int aX, int aY)
{
    return mCorrectMatrix.at(6*aX+aY);
}
