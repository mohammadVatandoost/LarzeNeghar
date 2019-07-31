#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "sensordata.h"
#include "QDebug"
#include "math.h"

#define no_earthquake 0
#define earthquake_no_Alarm 1000
#define earthquake_Alarm 20000
#define more_than_treshold 30000
#define sampling_frequency 100

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
    double Acc_Threshould = 5;
//    vector <Biquad> coeffs;
//    Butterworth butterworth;
    QVector<float> a;
    QVector<float> b;
    // filtering
    QVector<int> tseries_f;
    // sta/lta
    int nl = 100;
    int ns = 10;
    float dt = 0.005;
    int thresh = 5;
    int nt;
    QVector<int> aseries;
    QVector<int> sra;
    // filtering
    int win_length = 3;
    void setParameters(double accThresould,float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength, double eew_a1, double eew_a2, double eew_a3, double eew_a4);
    double a1 = 1; double a2 = 1.02; double a3 = 1; double a4 = 1.02;
    void butterWorthTest();
    void getButterWorthCoeffs();
    double trapz(QVector<double> temp);
    double trapzWithPower2(QVector<double> temp);
    float runAlgorithm(QVector<double> data, bool isOnGround);
    int runCheckAlarm(QVector<double> data, double *erthMagnitude, float k1);
    QVector<double> bandPassFilter(QVector<double> data);
    QVector<double> absVector(QVector<double> temp);
    QVector<int> zeros(int size);
    double meanQVector(QVector<double> temp);
    QVector<double> splitQvector(QVector<double> temp, int start, int stop);
};

#endif // ALGORITHM_H
