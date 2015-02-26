#ifndef RSPECTR_H
#define RSPECTR_H

#include <qmath.h>
#include <qglobal.h>
#include "Constants.h"

class RSpectr
{
    double data_step;
    double max[MAX_SPECTR_POINTS];
    double data[MAX_SPECTR_POINTS];
public:
    RSpectr();
    virtual ~RSpectr();
    int GetPoints(double freq_min, double freq_step, unsigned num, double *buff);
    double GetPoint(double freq);
    int GetPointsMax(double freq_min, double freq_step, unsigned num, double *buff);
    double GetPointMax(double freq);
    int SetPoints(double freq_min, double freq_step, unsigned num, double *buff);
    void SetPoint(double freq, double point);
    void ResetMax();
//// Test
    void CreateTestData();
};

#endif // RSPECTR_H
