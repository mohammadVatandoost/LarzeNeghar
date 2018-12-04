#include "sensorslist.h"

SensorsList::SensorsList(QObject *parent) : QObject(parent)
{
//    QDir dataFolder("Data");
    QFile csvFile("./Data/sensorsInfo.csv");
    if(csvFile.exists()) {
        QVector<QStringList> sensorsList = readCSV("./Data/sensorsInfo.csv", ",");
        qDebug() << "sensorsList.length() :" << sensorsList.length() ;
        for(int i=0; i<sensorsList.length(); i++) {
            qDebug() << sensorsList[i].length() ;
            if(sensorsList[i].length() > 11) {
                bool sOL = true; bool sOW = true; bool sTW = true;
                if(sensorsList[i][8] == "FALSE") {sOL = false;}
                if(sensorsList[i][9] == "FALSE") {sOW = false;}
                if(sensorsList[i][10] == "FALSE") {sTW = false;}
               Sensor temp(channelNumCounter, sensorsList[i][0].toInt(),sensorsList[i][1].toInt(), sensorsList[i][2],
                       sensorsList[i][3], sensorsList[i][4], sensorsList[i][5].toInt(), sensorsList[i][6], sensorsList[i][7],
                       sOL, sOW, false, sensorsList[i][11] );
               changeMinMax(temp.min, temp.max, temp.bordar);
               sensorItems.append(temp);
               channelNumCounter++;
            }
        }
    }
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
    sensorItems.append(newSensor);
}

void SensorsList::addData(int min, int sec, int milSec, int routerNumber, int sensorNumber, QString sensorBordar, QString sensorDatas)
{
    bool isNewSensor = true; // for checking new sensor
    int minDataValue = 0;
    int maxDataValue = 0;
    for(int i=0;i<sensorItems.length();i++) {
       if(sensorItems[i].routerNumber == routerNumber && sensorItems[i].sensorNumber == sensorNumber
               && sensorItems[i].bordar == sensorBordar) {
          isNewSensor = false ; // is not new Sensor
          for(int j=0; j<sensorDatas.length();j++) {
              QChar first_8bit = sensorDatas[j];
              QChar second_8bit = sensorDatas[j+1];
              int dataValue = first_8bit.toLatin1()*256 + second_8bit.toLatin1() ;
              if( dataValue > maxDataValue ) {
                  if(maxDataValue < minDataValue) { minDataValue = maxDataValue; }
                  maxDataValue = dataValue;
              } else if(dataValue < minDataValue) { minDataValue = dataValue ; }
              sensorItems[i].addData((min*60*1000+sec*1000+milSec+(j/2)*10), dataValue);
              j++;
          }
       }
    }
    if(isNewSensor) {
        Sensor newSensor(sensorItems.length()+1, routerNumber, sensorNumber, sensorBordar);
        for(int j=0; j<sensorDatas.length();j++) {
            QChar first_8bit = sensorDatas[j];
            QChar second_8bit = sensorDatas[j+1];
            int dataValue = first_8bit.toLatin1()*256 + second_8bit.toLatin1() ;
            if( dataValue > maxDataValue ) {
                if(maxDataValue < minDataValue) { minDataValue = maxDataValue; }
                maxDataValue = dataValue;
            } else if(dataValue < minDataValue) { minDataValue = dataValue ; }
            newSensor.addData((min*60*1000+sec*1000+milSec+(j/2)*10), dataValue);
            j++;
        }
        addSensor(newSensor);
    }
    changeMinMax(minDataValue, maxDataValue, sensorBordar);
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
    if(sensorXmin < 0) {
      return sensorYmin/zoomScaleY;
    } else {
        return sensorYmin*zoomScaleY;
    }
}

int SensorsList::getSensorYmax()
{
    if(sensorYmax < 0) {
      return sensorYmax*zoomScaleY;
    } else {
        return sensorYmax/zoomScaleY;
    }
}

int SensorsList::getSensorZmin()
{
    if(sensorXmin < 0) {
      return sensorZmin/zoomScaleZ;
    } else {
        return sensorZmin*zoomScaleZ;
    }
}

int SensorsList::getSensorZmax()
{
    if(sensorZmax < 0) {
      return sensorZmax*zoomScaleZ;
    } else {
        return sensorZmax/zoomScaleZ;
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
