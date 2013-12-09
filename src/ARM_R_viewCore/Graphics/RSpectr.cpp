/******************************************************************
/
/this class respond for storing and handling spectrum data
/
/*******************************************************************/
#include "RSpectr.h"

RSpectr::RSpectr()
{
    unsigned long i;
    data_step =  (double)MAX_SPECTR_POINTS/(double)(FREQ_MAX - FREQ_MIN);
//	double temp = 200.0/MAX_SPECTR_POINTS;
    for(i = 0; i < MAX_SPECTR_POINTS; i++)
    {
        data[i] = -150.0;
        max[i] = -150.0;
    }
}

RSpectr::~RSpectr()
{

}

/// find maxima
void RSpectr::ResetMax()
{
    unsigned long i;
    for(i = 0; i < MAX_SPECTR_POINTS; i++)
    {
        max[i] = data[i];
    }
}

/// write points from minimum (threshold) to buff
int RSpectr::GetPoints(double freq_min, double freq_step, unsigned num, double *buff)
{
    unsigned long i, min_f, /*max_f,*/ temp;
    if(freq_min >= FREQ_MIN)
    {
        min_f = (unsigned long)freq_min/freq_step;
        if(min_f + num < FREQ_MAX)
        {
            temp = min_f + num;
        }
        else
        {
            temp = FREQ_MAX;
        }

        for(i = min_f; i < temp; i++)
        {
            buff[i] = data[(unsigned long)(i*freq_step*data_step)]; // в будущем найти max и min
        }
    }
    else
        return -1;
}

/// must find maxs and mins (have to correct)
int RSpectr::GetPointsMax(double freq_min, double freq_step, unsigned num, double *buff)
{
    unsigned long i, min_f, /*max_f,*/ temp;
    if(freq_min >= FREQ_MIN)
    {
        min_f = (unsigned long)freq_min/freq_step;
        if(min_f + num < FREQ_MAX)
        {
            temp = min_f + num;
        }
        else
        {
            temp = FREQ_MAX;
        }

        for(i = min_f; i < temp; i++)
        {
            buff[i] = max[(unsigned long)(i*freq_step*data_step)]; // в будущем найти max и min
        }
    }
    else
        return -1;
}

/// find max in the frequency
double RSpectr::GetPointMax(double freq)
{
    if((freq > FREQ_MIN)&&(freq < FREQ_MAX))
    {
        return max[(unsigned)(freq*data_step)];
    }
    else
    {
        return -120.0;
    }
}

/// write points to internam array
int RSpectr::SetPoints(double freq_min, double freq_step, unsigned num, double *buff)
{
    unsigned long i, min_f, /*max_f,*/ temp;
    if(freq_min >= FREQ_MIN)
    {
        min_f = (unsigned long)freq_min/freq_step;
        if(min_f + num < FREQ_MAX)
        {
            temp = min_f + num;
        }
        else
        {
            temp = FREQ_MAX;
        }

        for(i = min_f; i < temp; i++)
        {
            data[(unsigned long)(i*freq_step*data_step)]=buff[i]; // в будущем Заполнить все точки data
        }
    }
    else
        return -1;
}

/// return point in the frequency
double RSpectr::GetPoint(double freq)
{
    if((freq>FREQ_MIN)&&(freq<FREQ_MAX))
    {
        return data[(unsigned)(freq*data_step)];
    }
    else
    {
        return -120.0;
    }
}

/// add point to array
void RSpectr::SetPoint(double freq, double point)
{
    if((freq > FREQ_MIN)&&(freq < FREQ_MAX))
    {
        unsigned i = (unsigned)(freq*data_step);
        if(i < MAX_SPECTR_POINTS)
        {
            data[i] = point;
            if(max[i]<data[i]) max[i] = data[i];
        }
    }
}

/// test data (random)
void RSpectr::CreateTestData()
{
    double temp;
    temp = 0.0;
    for(int i=1; i<MAX_SPECTR_POINTS; i++)
    {
//		if(i%2) data[i] = -50.0 - log((double)rand());
//		else data[i] = -50.0 + log((double)rand());
        data[i] = -50.0 - log((double)qrand())*5;
        if(data[i]>-25.0) data[i]=-25.0;
        if(max[i]<data[i]) max[i] = data[i];
    }
}
