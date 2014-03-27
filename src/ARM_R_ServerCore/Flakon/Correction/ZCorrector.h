#ifndef ZCORRECTOR_H
#define ZCORRECTOR_H

#include <QCoreApplication>
#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSettings>

/*!
    Класс, который компенсирует систематические ошибки
*/
class ZCorrector : public QObject
{
    Q_OBJECT
public:
    explicit ZCorrector(QObject *parent = 0);
    
signals:
    
public slots:
	//! Компенсировать систематические ошибки
    void correctDr(int aMainIndex, QVector<double> &lDR);

private:

    //! Загнать элемент в матрицу
    void setElement(int aX, int aY, double aElement);

    //! Достать элемент из матрицы
    double getElement(int aX, int aY);

private:

    QVector<double> mCorrectMatrix;
    
};

#endif // ZCORRECTOR_H
