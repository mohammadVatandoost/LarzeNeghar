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
//        qDebug() << "x :" << sensorXmin << " , " << sensorXmax;
//        qDebug() << "y :" << sensorYmin << " , " << sensorYmax;
//        qDebug() << "z :" << sensorZmin << " , " << sensorZmax;
//        foreach(QFileInfo item, dataFolder.entryInfoList()) {

//        }
//       QDir().mkdir("Data");
    } else {
        qDebug() << "sensorsInfo.csv does not exist";//QDir().mkdir("Data2");
//        if(!QDir("Data").exists()) {
//           QDir().mkdir("Data");
//        } else {qDebug() << "exist";}
    }
//    QFile csvFile("./Data/"+fileName+".csv");

//    Sensor temp(1,234,567,"x");
//    QString string2 = "Monday, 23 April 12 22:51:41";
//    QString format2 = "dddd, d MMMM yy hh:mm:ss";

//    QDateTime invalid2 = QDateTime::fromString(string2, format2);
//    QTime qTime(1,15,40,500);
//    qDebug() << "SensorsList :" << qTime.msecsSinceStartOfDay();
//    qDebug() << "converted :" << QTime::fromMSecsSinceStartOfDay(qTime.msecsSinceStartOfDay());
//    for(int i=0;i<6;i++) {
//        QTime qTime(1,15,i*10,500);
//        qDebug() << "SensorsList :" << qTime.toString();
//        temp.addData(qTime.msecsSinceStartOfDay(),i);
////        QString string = "12 22:51:4"+QString::number(i);
////        QString format = "d hh:mm:ss";
////        QDateTime invalid = QDateTime::fromString(string, format);
////        qDebug() << "SensorsList :" << invalid.toMSecsSinceEpoch();
////        temp.addData(invalid.toMSecsSinceEpoch(),i);
//    }
//    qDebug() << "SensorsList QTime:" << QTime::fromMSecsSinceStartOfDay(QTime::fromString("01:15:20.500").msecsSinceStartOfDay()).toString();
//    temp.addData(QTime::fromString("01:15:20.500").msecsSinceStartOfDay(),6);
//    Sensor temp1(2,234,567,"y");
//    for(int i=0;i<6;i++) {
//        QString string = "12 22:51:4"+QString::number(i);
//        QString format = "d hh:mm:ss";
//        QDateTime invalid = QDateTime::fromString(string, format);
//        temp.addData(invalid.toMSecsSinceEpoch(),i);
//    }
//    Sensor temp2(3,234,567,"z");
//    for(int i=0;i<6;i++) {
//        QString string = "12 22:51:4"+QString::number(i);
//        QString format = "d hh:mm:ss";
//        QDateTime invalid = QDateTime::fromString(string, format);
//        temp.addData(invalid.toMSecsSinceEpoch(),i);
//    }

//    temp.addData(QDateTime::fromString("01:10:00","hh:mm:ss").toMSecsSinceEpoch(),1);
//    sensorItems.append(temp);//sensorItems.append(temp1);sensorItems.append(temp2);
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
    return sensorXmin;
}

int SensorsList::getSensorXmax()
{
    return sensorXmax;
}

int SensorsList::getSensorYmin()
{
    return sensorYmin;
}

int SensorsList::getSensorYmax()
{
    return sensorYmax;
}

int SensorsList::getSensorZmin()
{
    return sensorZmin;
}

int SensorsList::getSensorZmax()
{
    return sensorZmax;
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
