#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H
#include <QObject>
#include <QtCore/QObject>
#include <algorithm.h>
#include <QThread>
#include "sensorslist.h"

#define no_earthquake 0
#define earthquake_no_Alarm 1000
#define earthquake_Alarm 20000
#define dataBufferCounterLimit 3000 // for 30 seconds
#define more_than_treshold 30000
#define sampling_frequency 100
#define isOnRoof 1
#define isOnGouend 2

class AlgorithmThread : public QThread
{
    Q_OBJECT
public:
    AlgorithmThread(SensorsList *sList);
    SensorsList *sensorsList;
    Algorithm algorithm;
    void earthquakeHappen();
    void setParameters(double accThreshould, float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength, double a1, double a2, double a3, double a4);
    bool runAlghorithm = true;
    void setRunAlghoritm(bool temp);
    double earthquakeMagnitude = 0;
    bool earthquakeHappenFlag = false;
    int windowSize = 3;
    uint8_t wichSensor ;
    float k1;
signals:
 void signalAlarm();
 void storeEarthquakeData();
protected:
   virtual void run();

};

#endif // ALGORITHMTHREAD_H
