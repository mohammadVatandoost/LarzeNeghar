#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "Butterworth.h"
#include "QDebug"
//#include "Iir.h"
//#include "assert_print.h"

class Algorithm
{
public:
    Algorithm();
    // fliter design
    float fs = 100;
    float f1 = 0.1;
    float f2 = 15;
    // noramalizing frequency
    float w1 = f1 / (fs/2);
    float w2 = f2 / (fs/2);
    // 4th order butterworth
    int filterOrder = 4;
    double overallGain = 1.0;
    vector <Biquad> coeffs;
    Butterworth butterworth;
    QVector<float> a;
    QVector<float> b;
    // filtering
    QVector<int> tseries_f;
    // sta/lta
    int n1 = 100;
    int ns = 10;
    float dt = 0.005;
    int thresh = 5;
    int nt;
    QVector<int> aseries;
    QVector<int> sra;
    // filtering
    int win_length = 3;
    void setParameters(float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength);
    void butterWorthTest();
    void getButterWorthCoeffs();
    QVector<int> absVector(QVector<int> temp);
    QVector<int> zeros(int size);
    float meanQVector(QVector<int> temp);
    QVector<int> splitQvector(QVector<int> temp, int start, int stop);
};

#endif // ALGORITHM_H
