#include "sensorslist.h"

SensorsList::SensorsList(QObject *parent) : QObject(parent)
{
    csvAddres=QDate::currentDate().toString();
    csvAddres+=QTime::currentTime().toString();

    if(!QDir("Data").exists()) {
        QDir().mkdir("Data");
    } else {qDebug() << "exist";}
    // timer for connection check
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(30000);

    QTimer *timerChangeCSV=new QTimer(this);
//    connect(timerChangeCSV,SIGNAL(timeout()),this,SLOT(changeCSVFilesName()));
//    timerChangeCSV->start(EachFilePeriodinMS);

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

void SensorsList::updateSensorInfo(QJsonObject temp)
{
    int routerNum = temp.value("router_number").toInt();
    int sensorNum = temp.value("sensor_number").toInt();
    for(int i=0; i<sensorItems.length(); i++) {
        if( (sensorItems[i].routerNumber == routerNum) && (sensorItems[i].sensorNumber == sensorNum) ) {
            if(temp.value("saving_local").toInt() == 1) {
                sensorItems[i].savingOnLocal = true;
            } else { sensorItems[i].savingOnLocal = false; }

            qDebug()<< "updateSensorInfo : " << routerNum << " , " << sensorNum << " , saving_local:" <<sensorItems[i].savingOnLocal;
//            qDebug()<< "updateSensorInfo : " << temp.value("saving_local").toBool() << " , " << temp.value("saving_local").toString()<< " , " << ;
//            break;
        }
    }
}

void SensorsList::addData(int min, int sec, int milSec, int routerNumber, int sensorNumber, QString sensorBordar,
                          float dataValue, uint8_t dataNumber)
{
//    qDebug()<<"sensorList  addData "<<sensorBordar;
    bool isNewSensor = true; // for checking new sensor
    float minDataValue = 0;
    float maxDataValue = 0;
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
//            double calculatedTime = getDateTimeToMSec(min)+(min*60*1000+sec*1000+milSec+(dataNumber*10));
//            if(routerNumber == 10 && sensorNumber == 110 && sensorBordar == 'x') {
//                qDebug()<<routerNumber << ","<<sensorNumber << " X:"<< calculatedTime <<","<< dataValue ;
//            }
//            sensorItems[i].addData(calculatedTime, dataValue);
            sensorItems[i].addData(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day(), QTime::currentTime().hour(), min, sec, milSec+(dataNumber*10), dataValue);
           // qDebug()<<calculatedTime<<" "<<dataValue;
        }
    }
    if(isNewSensor) {
        // is new sensor
        Sensor newSensor(sensorItems.length()+1, routerNumber, sensorNumber, sensorBordar);
        if( dataValue > maxDataValue ) {
            if(maxDataValue < minDataValue) { minDataValue = maxDataValue; }
            maxDataValue = dataValue;
        } else if(dataValue < minDataValue) { minDataValue = dataValue ; }
//        newSensor.addData(getDateTimeToMSec(min)+(min*60*1000+sec*1000+milSec+dataNumber), dataValue);
         newSensor.addData(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day(), QTime::currentTime().hour(), min, sec, milSec, dataValue);

        if(!checkIsNewSensor(routerNumber, sensorNumber)) {
            qDebug()<< "is new sensor : " << routerNumber << " , " << sensorNumber;
            sendNewSensor(newSensor);
        }
        addSensor(newSensor);
    }
    // for scaling chart
    changeMinMax(minDataValue, maxDataValue, sensorBordar);
}

void SensorsList::setSensorsSettings(int sensorNumber, QString sensorBordar, QString batteryLevel, int routerBatteryLevel)
{

//    qDebug()<<"sensorList  setSensorSetting "<<sensorBordar;

    //emit preItemAppended();
    //qDebug() << " setSensorsSettings length";
    for(int i=0;i<sensorItems.length();i++) {
        // find sensor
        if(sensorItems[i].sensorNumber == sensorNumber) {//                sensorBatteryLevel=QString::number((int)dataByte[i+1]);

            //qDebug() << "sensprs list find sensor";
            // sensorItems[i].batteryLevel = batteryLevel;
            sensorNumberForUpdate=i;
            batteryLevelForUpdate=batteryLevel.toInt();
            RouterbatteryLevelForUpdate=routerBatteryLevel;
            emit callBatteryUpdate();

        }
    }
    //emit postItemAppended();
}

void SensorsList::setChannelListApply(int index, channelListUpdate state , QString data)
{
    sensorNumberForApplyUpdate=index;
    globalStateforApplyUpdate=state;
    globalStringForApplyUpdate=data;
    emit callApplyUpdate();
}

void SensorsList::setTCPSocket(QTcpSocket *temp)
{
    qDebug()<< "SensorsList::setTCPSocket";
    pSocket = temp;
}

void SensorsList::sendNewSensor(Sensor temp)
{
    QJsonObject qJsonObject ;
    qJsonObject.insert(packetType, newSensorPacket);
    qJsonObject.insert("router_number", temp.routerNumber);
    qJsonObject.insert("sensor_number", temp.sensorNumber);
    qJsonObject.insert("low_pass", temp.low_pass);
    qJsonObject.insert("high_pass", temp.high_pass);
    qJsonObject.insert("bordar", temp.bordar);
    qJsonObject.insert("sensorBatteryLevel", temp.batteryLevel);
    qJsonObject.insert("routerBatteryLevel", temp.batteryLevel);
    qJsonObject.insert("id", temp.channelNumber);
    qJsonObject.insert("antenSignal", "");
    qJsonObject.insert("discreption", "");
    qJsonObject.insert("saving_local", 0);
    QJsonDocument doc(qJsonObject);
    qDebug() << "newSensorPacket :"<< doc.toJson(QJsonDocument::Compact);
    pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
}

bool SensorsList::checkIsNewSensor(int routerNumberTemp, int sensorNumberTemp)
{
    qDebug()<< "checkIsNewSensor" << routerNumberTemp <<","<< sensorNumberTemp;
    for(int i=0; i<sensorItems.length();i++) {
        if(sensorItems[i].routerNumber == routerNumberTemp && sensorItems[i].sensorNumber == sensorNumberTemp) {
            return true;
        }
    }
    return false;
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
   //    qDebug() << "sensorslist setSensorFrequency";
//    for(int i=0;i<sensorItems.length();i++) {
//        // find sensor
//        if(sensorItems[i].sensorNumber == sensorNumber) {
//            sensorItems[i].sampleRate = sampleRate;
//            sensorItems[i].bandpassFilter = bandpassFilter;
//        }
//    }
}

void SensorsList::addSensorInfoToCSV(Sensor newSensor)
{
    //    QVector<QStringList> dataList;
    //    QStringList listRow;
}

void SensorsList::runTest()
{
    for(int i=0; i<sensorItems.length(); i++) {
//       sensorItems[i].runTest = true;
        sensorItems[i].startTest();
    }
}

void SensorsList::stopTest()
{
    for(int i=0; i<sensorItems.length(); i++) {
//       sensorItems[i].runTest = false;
        sensorItems[i].stopTest();
    }
}

int SensorsList::getSensorLoss(int router_num, int sensor_num, int sensorLossTemp)
{
    for(int i=0; i<sensorItems.length(); i++) {
        if( (sensorItems[i].routerNumber == router_num) && (sensorItems[i].sensorNumber == sensor_num) && (sensorItems[i].bordar == "x")) {
            sensorItems[i].addSensorLoss(sensorLossTemp);
            int temp = sensorItems[i].getSensorLossAverage();
            if(temp>7) {sensorItems[i].isConnected = false;}
            return temp;
        }
    }
}

float SensorsList::getSensorXmin()
{
    if(sensorXmin < 0) {
        return (sensorXmin/zoomScaleX)+scrollXData;
    } else {
        return (sensorXmin*zoomScaleX)+scrollXData;
    }

}

float SensorsList::getSensorXmax()
{
    if(sensorXmax < 0) {
        return (sensorXmax*zoomScaleX)+scrollXData;
    } else {
        return (sensorXmax/zoomScaleX)+scrollXData;
    }
}

float SensorsList::getSensorYmin()
{
    if(sensorYmin < 0) {
        return (sensorYmin/zoomScaleY)+scrollYData;
    } else {
        return (sensorYmin*zoomScaleY)+scrollYData;
    }
}

float SensorsList::getSensorYmax()
{
    if(sensorYmax < 0) {
        return (sensorYmax*zoomScaleY)+scrollYData;
    } else {
        return (sensorYmax/zoomScaleY)+scrollYData;
    }
}

float SensorsList::getSensorZmin()
{
    if(sensorZmin < 0) {
        return (sensorZmin/zoomScaleZ)+scrollZData;
    } else {
        return (sensorZmin*zoomScaleZ)+scrollZData;
    }
}

float SensorsList::getSensorZmax()
{
    if(sensorZmax < 0) {
        return (sensorZmax*zoomScaleZ)+scrollZData;
    } else {
        return (sensorZmax/zoomScaleZ)+scrollZData;
    }
}

void SensorsList::zoom(QString bordar, int value){
    if(bordar == "x"){
        if(value>prevZoomX)
            zoomPlus(bordar);
        else
            zoomMines(bordar);
        prevZoomX=value;

    } else if (bordar == "y") {
        if(value>prevZoomY)
            zoomPlus(bordar);
        else
            zoomMines(bordar);
        prevZoomY=value;
    } else if (bordar == "z"){
        if(value>prevZoomZ)
            zoomPlus(bordar);
        else
            zoomMines(bordar);
        prevZoomZ=value;
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

void SensorsList::scrollData(QString bordar, int value){
    if(bordar == "x"){
        if(value>prevScrollDataX)
            scrollDataPlus(bordar);
        else
            scrollDataMines(bordar);
        prevScrollDataX=value;

    } else if (bordar == "y") {
        if(value>prevScrollDataY)
            scrollDataPlus(bordar);
        else
            scrollDataMines(bordar);
        prevScrollDataY=value;
    } else if (bordar == "z"){
        if(value>prevScrollDataZ)
            scrollDataPlus(bordar);
        else
            scrollDataMines(bordar);
        prevScrollDataZ=value;
    }
}

void SensorsList::scrollDataPlus(QString bordar)
{
    qDebug() << "scrollDataPlus:" << bordar;
    if(bordar == "x") {
        scrollXData = scrollXData + (changeScrollData/(zoomScaleX)); // /zoomScaleX not support float if you want, should change
    } else if(bordar == "y") {
        scrollYData = scrollYData + (changeScrollData/zoomScaleY); // /zoomScaleY
    } else if(bordar == "z") {
        scrollZData = scrollZData + (changeScrollData/zoomScaleZ); // /zoomScaleZ
    }
}

void SensorsList::scrollDataMines(QString bordar)
{
//    qDebug() << "scrollDataMines:" << bordar;
    if(bordar == "x") {
        scrollXData = scrollXData - (changeScrollData/zoomScaleX); // /zoomScaleX not support float if you want, should change
    } else if(bordar == "y") {
        scrollYData = scrollYData - (changeScrollData/zoomScaleY);// /zoomScaleY
    } else if(bordar == "z") {
        scrollZData = scrollZData - (changeScrollData/zoomScaleZ); // /zoomScaleZ
    }
}

void SensorsList::scrollTime(QString bordar, int value){
    if(bordar == "x"){
        if(value>prevScrollTimeX)
            scrollTimePlus(bordar);
        else
            scrollTimeMines(bordar);
        prevScrollTimeX=value;

    } else if (bordar == "y") {
        if(value>prevScrollTimeY)
            scrollTimePlus(bordar);
        else
            scrollTimeMines(bordar);
        prevScrollTimeY=value;
    } else if (bordar == "z"){
        if(value>prevScrollTimeZ)
            scrollTimePlus(bordar);
        else
            scrollTimeMines(bordar);
        prevScrollTimeZ=value;
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

int SensorsList::getSize()
{
    return sensorItems.length();
}

int SensorsList::getRouterNumberAt(int i)
{
    return sensorItems[i].routerNumber;
}

int SensorsList::getSensorNumberAt(int i)
{
   return sensorItems[i].sensorNumber;
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

void SensorsList::changeMinMax(float minValue, float maxValue, QString bordar)
{
   //    qDebug()<<"sensorList  changeMinMax "<<bordar;

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
  //    qDebug()<<"sensorList  getDateTimeToMSec ";

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
//    qDebug()<<"sensorList  timerslot ";

    //    qDebug() << "sensorsList:" << sensorItems.length() << " , " << sensorsListCSVCounter.length() ;
//    if(sensorItems.length() == sensorsListCSVCounter.length()) {
//        for(int i=0;i<sensorItems.length();i++) {
//            //        qDebug() << "sensorItems[i].data.length():" << sensorItems[i].data.length() << " , sensorsListCSVCounter[i]*1000) : " << sensorsListCSVCounter[i]*1000 ;
//            if( (sensorItems[i].data.length() - ((sensorsListCSVCounter[i]+1)*1000)) > 0) {
//                QVector<QStringList> dataList;
//                int startStep = sensorsListCSVCounter[i]*1000;
//                int stopStep = startStep + 1000;
//                for(int j = startStep; j<stopStep; j++) {
//                    QStringList listRow;
//                    listRow.append( QString::number( sensorItems[i].data[j].x(),'g',17 ) );
//                    listRow.append(QString::number( sensorItems[i].data[j].y() ));
//                    dataList.append(listRow);
//                }
//                sensorsListCSVCounter[i]++;
//                appendDataToCSV(dataList, "./Data/"+csvAddres+QString::number(sensorItems[i].sensorNumber)+sensorItems[i].bordar+".csv");
//            }
//        }
//    }
  // ************* must check sd card space
}

void SensorsList::changeCSVFilesName()
{
    //change the name of the csv on each timeout
    //the csvAddress is the first part of the .csv file in Data folder
    csvAddres=QDate::currentDate().toString();
    csvAddres+=QTime::currentTime().toString();

    //in each timeout we have to check wheter number of stored files is acceptable or not
    //when number of file is not acceptable it means that we shoud take care of the memory by deleting some of earliest files
    QDir directory("./Data");
    int total_files = directory.count();
    int countTillSensorLength=0;
    if(total_files>MaximumNumberOfFiles){
        QStringList csvFiles = directory.entryList(QStringList() << "*.csv",QDir::Files);
        foreach(QString filename, csvFiles) {
            /*
             * for example if we have 4 sensor it means we have 4*3=12=sensorItems.length() file in each period
             * thus we have to remove all files for specific period together
             */
            if(countTillSensorLength++<sensorItems.length()){
            QFile file ("./Data/"+filename);
            file.remove();
            }else {
                break;
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
