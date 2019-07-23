#include "algorithmthread.h"

AlgorithmThread::AlgorithmThread(SensorsList *sList)
{
    sensorsList = sList ;
}

void AlgorithmThread::earthquakeHappen()
{
//    qDebug() << "AlgorithmThread::earthquakeHappen()";
    for(int i=0; i<sensorsList->sensorItems.size(); i++) {
        sensorsList->sensorItems[i].earthquackHappen = true ;
    }
}

void AlgorithmThread::setParameters(float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength, double a1, double a2, double a3, double a4)
{
    algorithm.setParameters(highPass, lowPass, longPoint, shortWin, staLtaTreshold, winLength, a1, a2, a3, a4);
}

void AlgorithmThread::setRunAlghoritm(bool temp)
{
    runAlghorithm = temp ;
    qDebug() << "setRunAlghoritm :" << runAlghorithm ;
}

void AlgorithmThread::run()
{
    while(true) {
//      qDebug()<< "thread run";
      if(runAlghorithm) {
//          qDebug()<< "thread run runAlghorithm";
        for(int i=0; i<sensorsList->sensorItems.size(); i++) {
          if(!sensorsList->sensorItems[i].earthquackHappen) {
            if(sensorsList->sensorItems.at(i).onGroundSensor && (sensorsList->sensorItems.at(i).bordar == "z") ) {
             // filter data
                float earthquake_state = algorithm.runAlgorithm(sensorsList->sensorItems.at(i).alghorithmDataBuffer, &earthquakeMagnitude);

                if(earthquake_state != no_earthquake) {
//                    qDebug()<<"thread run ground sensor pstime:" <<pstime;
                  // calculate earthquak parameters
                    if(earthquake_state == earthquake_Alarm) {
                       signalAlarm();
                    }
                    earthquakeHappen();
                    break;
                }
            } else if(sensorsList->sensorItems.at(i).onRoofSensor && (sensorsList->sensorItems.at(i).bordar == "z") ) {
                float earthquake_state = algorithm.runAlgorithm(sensorsList->sensorItems.at(i).alghorithmDataBuffer);

                if(earthquake_state != no_earthquake) {
//                    qDebug()<<"thread run Roof sensor pstime:" <<pstime;
                    if(earthquake_state == earthquake_Alarm) {
                        signalAlarm();
                    }
                  // calculate earthquak parameters
//                    earthquakeHappen();
                }
            }
          }
        }
      }
       QThread::msleep(100);
    }
}
