#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H
#include <algorithm.h>
#include <QThread>
#include "sensorslist.h"

class AlgorithmThread : public QThread
{
public:
    AlgorithmThread(SensorsList *sList);
    SensorsList *sensorsList;
    Algorithm algorithm;
    void earthquakeHappen();
protected:
   virtual void run();
};

#endif // ALGORITHMTHREAD_H
