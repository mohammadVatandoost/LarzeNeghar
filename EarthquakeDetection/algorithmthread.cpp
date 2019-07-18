#include "algorithmthread.h"

AlgorithmThread::AlgorithmThread(SensorsList *sList)
{
    sensorsList = sList ;
}

void AlgorithmThread::earthquakeHappen()
{
    qDebug() << "AlgorithmThread::earthquakeHappen()";
    for(int i=0; i<sensorsList->sensorItems.size(); i++) {
        sensorsList->sensorItems[i].earthquackHappen = true ;
    }
}

void AlgorithmThread::setParameters(float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength)
{
    algorithm.setParameters(highPass, lowPass, longPoint, shortWin, staLtaTreshold, winLength);
}

void AlgorithmThread::setRunAlghoritm(bool temp)
{
    runAlghorithm = temp ;
}

void AlgorithmThread::run()
{
    while(true) {
      if(runAlghorithm) {
        for(int i=0; i<sensorsList->sensorItems.size(); i++) {
            if(sensorsList->sensorItems.at(i).onGroundSensor && (sensorsList->sensorItems.at(i).bordar == "z") ) {
             // filter data
                float pstime = algorithm.runAlgorithm(sensorsList->sensorItems.at(i).alghorithmDataBuffer);
                if(pstime != 0) {
                  // calculate earthquak parameters
                    earthquakeHappen();
                    break;
                }
            } else if(sensorsList->sensorItems.at(i).onRoofSensor && (sensorsList->sensorItems.at(i).bordar == "z") ) {
                float pstime = algorithm.runAlgorithm(sensorsList->sensorItems.at(i).alghorithmDataBuffer);
                if(pstime != 0) {
                  // calculate earthquak parameters
                    earthquakeHappen();
                }
            }
        }
      }
       sleep(100);
    }
}
