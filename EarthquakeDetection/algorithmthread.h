#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H
#include <algorithm.h>
#include <QThread>
#include "sensorslist.h"

#define no_earthquake 0
#define earthquake_no_Alarm 10
#define earthquake_Alarm 20

class AlgorithmThread : public QThread
{
public:
    AlgorithmThread(SensorsList *sList);
    SensorsList *sensorsList;
    Algorithm algorithm;
    void earthquakeHappen();
    void setParameters(float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength, double a1, double a2, double a3, double a4);
    bool runAlghorithm = true;
    void setRunAlghoritm(bool temp);
    double earthquakeMagnitude = 0;
signals:
 void signalAlarm();
protected:
   virtual void run();

};

#endif // ALGORITHMTHREAD_H
