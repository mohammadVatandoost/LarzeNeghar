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

void AlgorithmThread::setParameters(double accThreshould, float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength, double a1, double a2, double a3, double a4)
{
    windowSize = winLength;
    algorithm.setParameters(accThreshould, highPass, lowPass, longPoint, shortWin, staLtaTreshold, winLength, a1, a2, a3, a4);
}

void AlgorithmThread::setRunAlghoritm(bool temp)
{
    runAlghorithm = temp ;
    qDebug() << "setRunAlghoritm :" << runAlghorithm ;
}

void AlgorithmThread::run()
{
    while(true) {
//        qDebug()<< "AlgorithmThread";
      if(runAlghorithm) {

        for(int i=0; i<sensorsList->sensorItems.size(); i++) {
          if(!earthquakeHappenFlag) {
//              if(earthquakeHappenFlag) {
//                  emit storeEarthquakeData();
//                  earthquakeHappenFlag = false;
//              }
            if(sensorsList->sensorItems.at(i).onGroundSensor && (sensorsList->sensorItems.at(i).bordar == "z") && (sensorsList->sensorItems.at(i).alghorithmDataBuffer.length()>100) ) {
//                qDebug() <<sensorsList->sensorItems.at(i).sensorNumber<<","<<sensorsList->sensorItems.at(i).bordar << " onGroundSensor :" <<sensorsList->sensorItems.at(i).alghorithmDataBuffer.length();
             // filter data
                float earthquake_state = algorithm.runAlgorithm(sensorsList->sensorItems.at(i).alghorithmDataBuffer, true);
                if(earthquake_state == more_than_treshold) {
                    earthquakeHappen();
                    qDebug() << "AlgorithmThread onGroundSensor more_than_treshold";
                } else if(earthquake_state != no_earthquake) {
                    earthquakeHappenFlag = true;
                    k1 = earthquake_state;
                    earthquakeHappen();wichSensor = isOnGouend;
                    qDebug() << "AlgorithmThread onGroundSensor Earthqiake Happen";
                }
            } else if(sensorsList->sensorItems.at(i).onRoofSensor && (sensorsList->sensorItems.at(i).bordar == "z") && (sensorsList->sensorItems.at(i).alghorithmDataBuffer.length()>100) ) {
                float earthquake_state = algorithm.runAlgorithm(sensorsList->sensorItems.at(i).alghorithmDataBuffer, false);
                if(earthquake_state != no_earthquake) {
                    qDebug() << "AlgorithmThread onRoofSensor Earthqiake";
                     earthquakeHappenFlag = true;
                     k1 = earthquake_state; wichSensor = isOnRoof;
                     earthquakeHappen();
                }
            }
          } else {
              // check it to store data more than win Length
              if(sensorsList->sensorItems.at(i).onGroundSensor && (sensorsList->sensorItems.at(i).bordar == "z") && (wichSensor == isOnGouend) &&
                      (sensorsList->sensorItems.at(i).alghorithmDataBuffer.length()> (dataBufferCounterLimit+(windowSize*sampling_frequency)) ) ) {
                   float earthquake_state = algorithm.runCheckAlarm(sensorsList->sensorItems.at(i).alghorithmDataBuffer, &earthquakeMagnitude, k1);
                   if(earthquake_state == earthquake_Alarm) {
                       emit signalAlarm();earthquakeHappenFlag = false;emit storeEarthquakeData();
                       qDebug() << "AlgorithmThread onGroundSensor earthquake_Alarm signalAlarm";
                       break;
                   } else {
                            earthquakeHappenFlag = false;
                            emit storeEarthquakeData();
                            qDebug() << "AlgorithmThread onGroundSensor earthquake_no_Alarm storeEarthquakeData" ;
                   }
              }
              if(sensorsList->sensorItems.at(i).onRoofSensor && (sensorsList->sensorItems.at(i).bordar == "z") && (wichSensor == isOnRoof) &&
                      (sensorsList->sensorItems.at(i).alghorithmDataBuffer.length()> (dataBufferCounterLimit+(windowSize*sampling_frequency)) ) ) {
                   float earthquake_state = algorithm.runCheckAlarm(sensorsList->sensorItems.at(i).alghorithmDataBuffer, &earthquakeMagnitude, k1);
                   if(earthquake_state == earthquake_Alarm) {
                       emit signalAlarm();earthquakeHappenFlag = false;emit storeEarthquakeData();
                       qDebug() << "AlgorithmThread onRoofSensor earthquake_Alarm signalAlarm";
                       break;
                   } else {
                       earthquakeHappenFlag = false;
                       emit storeEarthquakeData();
                       qDebug() << "AlgorithmThread onRoofSensor earthquake_Alarm storeEarthquakeData";
                   }
              }
              // does not stay for ever in this mode
              if(!sensorsList->sensorItems.at(i).earthquackHappen){
                  earthquakeHappenFlag = false;qDebug() << "AlgorithmThread earthquake measur time out";
              }
          }
        }
      }
       QThread::msleep(100);
    }
}
