#include "algorithmthread.h"

AlgorithmThread::AlgorithmThread(SensorsList *sList)
{
    sensorsList = sList ;
}

void AlgorithmThread::earthquakeHappen()
{
    for(int i=0; i<sensorsList->sensorItems.size(); i++) {
        sensorsList->sensorItems[i].earthquackHappen = true ;
    }
}

void AlgorithmThread::run()
{
    while(true) {
        for(int i=0; i<sensorsList->sensorItems.size(); i++) {
            if(sensorsList->sensorItems.at(i).onGroundSensor) {
             // filter data
                if(algorithm.runAlgorithm(sensorsList->sensorItems.at(i).dataBuffer)) {
                  // calculate earthquak parameters
                    earthquakeHappen();
                    break;
                }
            } else if(sensorsList->sensorItems.at(i).onRoofSensor) {
                if(algorithm.runAlgorithm(sensorsList->sensorItems.at(i).dataBuffer)) {
                  // calculate earthquak parameters
                    earthquakeHappen();
                }
            }
        }
       sleep(100);
    }
}
