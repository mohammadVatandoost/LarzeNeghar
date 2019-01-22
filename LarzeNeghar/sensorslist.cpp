#include "sensorslist.h"

SensorsList::SensorsList(QObject *parent) : QObject(parent)
{
    if(!QDir("Data").exists()) {
       QDir().mkdir("Data");
    } else {qDebug() << "exist";}
    // timer for connection check
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(3000);
    pastHour = QTime::currentTime().hour();
}

bool SensorsList::setSensorItem(int index, Sensor &sensor)
{
    if(index <0 || index >= sensorItems.size())
        return false;
//    const Sensor oldItem = sensorItems.at(index);
//    if(item.done == oldItem.done && item.description == oldItem.description)
//        return false;

    sensorItems[index] = sensor;
    return true;
}

QVector<Sensor> SensorsList::items()
{
    return sensorItems;
}

void SensorsList::addSensor(Sensor newSensor)
{
    // add to csvFile
//    appendDataToCSV();
    emit preItemAppended();
    qDebug() << "add new Sensor" ;
    sensorItems.append(newSensor);
    sensorsListCSVCounter.append(0);
    emit postItemAppended();
}

void SensorsList::addData(int min, int sec, int milSec, int routerNumber, int sensorNumber, QString sensorBordar,
                          int dataValue, uint8_t dataNumber)
{
    bool isNewSensor = true; // for checking new sensor
    int minDataValue = 0;
    int maxDataValue = 0;
    for(int i=0;i<sensorItems.length();i++) {
        // find sensor
       if(sensorItems[i].routerNumber == routerNumber && sensorItems[i].sensorNumber == sensorNumber
               && sensorItems[i].bordar == sensorBordar) {
          isNewSensor = false ; // is not new Sensor
//              qDebug() << "dataValue "<<sensorNumber<<","<<sensorBordar<< " :" << dataValue;
              if( dataValue > maxDataValue ) {
                  if(maxDataValue < minDataValue) { minDataValue = maxDataValue; }
                  maxDataValue = dataValue;
              } else if(dataValue < minDataValue) { minDataValue = dataValue ; }
              double calculatedTime = getDateTimeToMSec(min)+(min*60*1000+sec*1000+milSec+(dataNumber*10));
              sensorItems[i].addData(calculatedTime, dataValue);
       }
    }
    if(isNewSensor) {
        // is new sensor
        Sensor newSensor(sensorItems.length()+1, routerNumber, sensorNumber, sensorBordar);
            if( dataValue > maxDataValue ) {
                if(maxDataValue < minDataValue) { minDataValue = maxDataValue; }
                maxDataValue = dataValue;
            } else if(dataValue < minDataValue) { minDataValue = dataValue ; }
            newSensor.addData(getDateTimeToMSec(min)+(min*60*1000+sec*1000+milSec+dataNumber), dataValue);
        addSensor(newSensor);
    }
    // for scaling chart
    changeMinMax(minDataValue, maxDataValue, sensorBordar);
}

void SensorsList::setSensorsSettings(int sensorNumber, QString sensorBordar, QString batteryLevel)
{
    qDebug() << "sensprs list setSensorsSettings length";
    for(int i=0;i<sensorItems.length();i++) {
        // find sensor
       if(sensorItems[i].sensorNumber == sensorNumber && sensorItems[i].bordar == sensorBordar) {
           qDebug() << "sensprs list find sensor";
           sensorItems[i].batteryLevel = batteryLevel;
       }
    }
}

void SensorsList::setRoutersSettings(int routerNumber, QString batteryLevel)
{
//    qDebug() << "routers list setRoutersSettings";
//    for(int i=0;i<sensorItems.length();i++) {
//        // find sensor
//       if(sensorItems[i].sensorNumber == sensorNumber && sensorItems[i].bordar == sensorBordar) {
//           qDebug() << "sensprs list find sensor";
//           sensorItems[i].batteryLevel = batteryLevel;
//       }
    //    }
}

void SensorsList::setSensorFrequency(int sensorNumber, int sampleRate, QString bandpassFilter)
{
    qDebug() << "sensprs list setSensorFrequency";
    for(int i=0;i<sensorItems.length();i++) {
        // find sensor
       if(sensorItems[i].sensorNumber == sensorNumber) {
           sensorItems[i].sampleRate = sampleRate ;
           sensorItems[i].bandpassFilter = bandpassFilter ;
       }
    }
}

void SensorsList::addSensorInfoToCSV(Sensor newSensor)
{
//    QVector<QStringList> dataList;
//    QStringList listRow;
}

int SensorsList::getSensorXmin()
{
    if(sensorXmin < 0) {
      return (sensorXmin/zoomScaleX)+scrollXData;
    } else {
      return (sensorXmin*zoomScaleX)+scrollXData;
    }

}

int SensorsList::getSensorXmax()
{
    if(sensorXmax < 0) {
      return (sensorXmax*zoomScaleX)+scrollXData;
    } else {
      return (sensorXmax/zoomScaleX)+scrollXData;
    }
}

int SensorsList::getSensorYmin()
{
    if(sensorYmin < 0) {
      return (sensorYmin/zoomScaleY)+scrollYData;
    } else {
        return (sensorYmin*zoomScaleY)+scrollYData;
    }
}

int SensorsList::getSensorYmax()
{
    if(sensorYmax < 0) {
      return (sensorYmax*zoomScaleY)+scrollYData;
    } else {
        return (sensorYmax/zoomScaleY)+scrollYData;
    }
}

int SensorsList::getSensorZmin()
{
    if(sensorZmin < 0) {
      return (sensorZmin/zoomScaleZ)+scrollZData;
    } else {
        return (sensorZmin*zoomScaleZ)+scrollZData;
    }
}

int SensorsList::getSensorZmax()
{
    if(sensorZmax < 0) {
      return (sensorZmax*zoomScaleZ)+scrollZData;
    } else {
        return (sensorZmax/zoomScaleZ)+scrollZData;
    }
}

void SensorsList::zoomPlus(QString bordar)
{
    if(bordar == "x") {
      zoomScaleX = zoomScaleX*2;
    } else if(bordar == "y") {
        zoomScaleY = zoomScaleY*2;
    } else if(bordar == "z") {
        zoomScaleZ = zoomScaleZ*2;
    }
}

void SensorsList::zoomMines(QString bordar)
{
    if(bordar == "x") {
      zoomScaleX = zoomScaleX/2;
    } else if(bordar == "y") {
        zoomScaleY = zoomScaleY/2;
    } else if(bordar == "z") {
        zoomScaleZ = zoomScaleZ/2;
    }
}

void SensorsList::scrollDataPlus(QString bordar)
{
    qDebug() << "scrollDataPlus:" << bordar;
    if(bordar == "x") {
      scrollXData = scrollXData + (changeScrollData); // /zoomScaleX not support float if you want, should change
    } else if(bordar == "y") {
        scrollYData = scrollYData + (changeScrollData); // /zoomScaleY
    } else if(bordar == "z") {
        scrollZData = scrollZData + (changeScrollData); // /zoomScaleZ
    }
}

void SensorsList::scrollDataMines(QString bordar)
{
    qDebug() << "scrollDataMines:" << bordar;
    if(bordar == "x") {
      scrollXData = scrollXData - (changeScrollData); // /zoomScaleX not support float if you want, should change
    } else if(bordar == "y") {
        scrollYData = scrollYData - (changeScrollData);// /zoomScaleY
    } else if(bordar == "z") {
        scrollZData = scrollZData - (changeScrollData); // /zoomScaleZ
    }
}

void SensorsList::scrollTimePlus(QString bordar)
{
    if(bordar == "x") {
      scrollXTime = scrollXTime + (changeScrollTime/zoomScaleX);
    } else if(bordar == "y") {
        scrollYTime = scrollYTime + (changeScrollTime/zoomScaleY);
    } else if(bordar == "z") {
        scrollZTime = scrollZTime + (changeScrollTime/zoomScaleZ);
    }
}

void SensorsList::scrollTimeMines(QString bordar)
{
    if(bordar == "x") {
      scrollXTime = scrollXTime - (changeScrollTime/zoomScaleX);
    } else if(bordar == "y") {
        scrollYTime = scrollYTime - (changeScrollTime/zoomScaleY);
    } else if(bordar == "z") {
        scrollZTime = scrollZTime - (changeScrollTime/zoomScaleZ);
    }
}

double SensorsList::getLastDataTime(QString bordar)
{
    double maxTemp = 0;
    for(int i=0; i<sensorItems.length(); i++) {
        if(sensorItems[i].bordar == bordar) {
                if(sensorItems[i].data[sensorItems[i].data.length()-1].x() > maxTemp) {
                    maxTemp = sensorItems[i].data[sensorItems[i].data.length()-1].x();
                }
        }
    }
    return maxTemp;
}

void SensorsList::changeMinMax(int minValue, int maxValue, QString bordar)
{
    if(bordar == "x") {
        if( sensorXmin > minValue ) { sensorXmin = minValue; }
        if( sensorXmax < maxValue ) { sensorXmax = maxValue; }
    } else if(bordar == "y") {
        if( sensorYmin > minValue ) { sensorYmin = minValue; }
        if( sensorYmax < maxValue ) { sensorYmax = maxValue; }
    } else if(bordar == "z") {
        if( sensorZmin > minValue ) { sensorZmin = minValue; }
        if( sensorZmax < maxValue ) { sensorZmax = maxValue; }
    }
}

double SensorsList::getDateTimeToMSec(int minute)
{
    QDateTime temp = QDateTime::currentDateTime()  ;
//    qDebug() << "QDateTime::currentDateTime() "<< temp.toString();
    int hour = QTime::currentTime().hour();
    if(pastHour != hour)
    {

        if(minute > 58) {
            hour = pastHour;
             qDebug() << "pastHour != hour hour = pastHour" ;
        } else if(minute < 2) {
            pastHour = hour;
            qDebug() << "pastHour != hour pastHour = hour;" ;
        } else {
          qDebug() << "pastHour != hour ";
        }
    }
    QTime tempTime(hour,0,0,0);
//    qDebug() << "QTime "<< tempTime.toString("hh:mm:ss:zzz");
 //   tempTime.
    temp.setTime(tempTime);
//    qDebug() << "QDateTime::currentDateTime() after "<< temp.toString();
    return temp.toMSecsSinceEpoch();

}

void SensorsList::timerSlot()
{
//    qDebug() << "sensorsList:" << sensorItems.length() << " , " << sensorsListCSVCounter.length() ;
    if(sensorItems.length() == sensorsListCSVCounter.length()) {
    for(int i=0;i<sensorItems.length();i++) {
//        qDebug() << "sensorItems[i].data.length():" << sensorItems[i].data.length() << " , sensorsListCSVCounter[i]*1000) : " << sensorsListCSVCounter[i]*1000 ;
        if( (sensorItems[i].data.length() - ((sensorsListCSVCounter[i]+1)*1000)) > 0) {
            QVector<QStringList> dataList;
            int startStep = sensorsListCSVCounter[i]*1000;
            int stopStep = startStep + 1000;
            for(int j = startStep; j<stopStep; j++) {
                QStringList listRow;
                listRow.append( QString::number( sensorItems[i].data[j].x(),'g',17 ) );
                listRow.append(QString::number( sensorItems[i].data[j].y() ));
                dataList.append(listRow);
            }
            sensorsListCSVCounter[i]++;
            appendDataToCSV(dataList, "./Data/"+QString::number(sensorItems[i].sensorNumber)+sensorItems[i].bordar+".csv");
//            qDebug() << "stored to csv:" << sensorItems[i].sensorNumber;
        }
     }
    }
}

void SensorsList::appendItem()
{
    emit preItemAppended();

//    Sensor sensorItem;
//    sensorItems.append(sensorItem);
    emit postItemAppended();
}

void SensorsList::removeItem(int index)
{
    emit preItemRemoved(index);
    sensorItems.removeAt(index);
    emit postItemRemoved();
}
