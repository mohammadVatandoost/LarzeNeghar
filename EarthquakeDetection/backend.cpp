#include "backend.h"
#include <math.h>

BackEnd::BackEnd(QObject *parent) : QObject(parent)
{

    serial = new QSerialPort(this);
    serial->close();
    serial->setBaudRate(256000);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    Algorithm *algorithm = new Algorithm();

    pSocket = new QTcpSocket(this);
    connect( pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );
    pSocket->connectToHost("127.0.0.1", 6969);
    if( pSocket->waitForConnected() ) {
       pSocket->write( "connected" );
    }

//    QFileInfoList list = directory.entryInfoList();
//    qDebug()<< "list size :" << list.size();
//    for(int i=0; i<list.size(); i++) {
//      qDebug()<< i << ":" << list[i].fileName();
//    }
}

void BackEnd::setSensorsList(SensorsList *sensorsList)
{
    qDebug() << "function start :" << "setSensorsList";
    mList = sensorsList;
    mList->setTCPSocket(pSocket);

    // timer for connection check
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
    packetTimer = new QTimer(this);
    connect(packetTimer, SIGNAL(timeout()), this, SLOT(timerPacketTimeOut()));
    packetTimer->start(15);
    // run thread
    algorithmThread = new AlgorithmThread(sensorsList);
    algorithmThread->start();
}

QVector<Sensor> BackEnd::getSensorsList()
{
    qDebug() << "function start :" << "getSensorsList";
    return mList->items();
}

QList<int> BackEnd::getSenorsX(QString routerNumber, QString sensorNumber, QString bordar)
{
    qDebug() << "function start :" << "getSenorsX";
    QVector<Sensor> temp = mList->items();
    for(int i=0; i<temp.length();i++) {
        if( (temp.at(i).routerNumber == routerNumber.toInt()) && (temp.at(i).sensorNumber == sensorNumber.toInt())
                && (temp.at(i).bordar == bordar) ) {
            qDebug() << "Founded x";
            return temp.at(i).dataX;
        }
    }
    QList<int> result;
    return result;
}

QList<int> BackEnd::getSenorsY(QString routerNumber, QString sensorNumber, QString bordar)
{
    qDebug() << "function start :" << "getSenorsY";
    QVector<Sensor> temp = mList->items();
    for(int i=0; i<temp.length();i++) {
        if( (temp.at(i).routerNumber == routerNumber.toInt()) && (temp.at(i).sensorNumber == sensorNumber.toInt())
                && (temp.at(i).bordar == bordar) ) {
            qDebug() << "Founded Y";
            return temp.at(i).dataY;
        }
    }
    QList<int> result;
    return result;
}

void BackEnd::test()
{
//    QVector<Sensor> temp = mList->items();
//    for(int i=0;i<temp.length();i++) {
//        qDebug() << i << " : " << temp[i].description << " : " << temp[i].sampleRate<< " : " << temp[i].bandpassFilter
//                 << ":" << temp[i].savingOnLocal << ":" << temp[i].sendingToWeb<< ":" << temp[i].savingOnWeb;
//    }
}

void BackEnd::sendSettings()
{
    qDebug() << "function start :" << "sendSettings";
    QVector<Sensor> sensors = mList->items();
    QJsonObject qJsonObject;

    QString sensorsUnitNumber = "";
    QString sensorsLowPass = "";
    QString sensorsHighPss = "";
    QString sensorsSampleRate = "";
    qJsonObject.insert(Sensors_Frequency_Parameters, "T");
    for(int i=0;i<sensors.length();i++) {
        QString sensorUnitNumber = QString::number(sensors[i].sensorNumber);
//        QStringList pieces = sensors[i].bandpassFilter.split( "-" );


        if(sensors[i].description!=sensors[i].Defaultdescription)
            mList->setChannelListApply(i,SensorsList::Description,sensors[i].description);
//        if(sensors[i].bandpassFilter!=sensors[i].DefaultbandpassFilter)
//            mList->setChannelListApply(i,SensorsList::LBandPass,sensors[i].bandpassFilter);
        if(sensors[i].sampleRate!=sensors[i].DefaultsampleRate)
            mList->setChannelListApply(i,SensorsList::SampleRate,QString::number(sensors[i].sampleRate));

        //in if else bara zakhire kardaneshoon too file jsone ke badan ke barname run shod akharin chizayi bashe ke apply kardi
        //har sensor X hasho hatman zakhire mikonim
        if(i%3==0){
            qJsonObject.insert("Desc"+sensorUnitNumber,sensors[i].description);
//            qJsonObject.insert(Sensor_Bandpass_Filter_Low+sensorUnitNumber, pieces[0]);
//            qJsonObject.insert(Sensor_Bandpass_Filter_High+sensorUnitNumber, pieces[1]);
            qJsonObject.insert(Sensor_Sample_Rate+sensorUnitNumber, sensors[i].sampleRate);
        }
        else{//az beyne Y o Z age ini ke bade zadane apply boode baa ooni ke ghabln too file boode zkhire mishe
            if(sensors[i].description!=sensors[i].Defaultdescription){
                qJsonObject.insert("Desc"+sensorUnitNumber,sensors[i].description);
            }
//            if(sensors[i].bandpassFilter!=sensors[i].DefaultbandpassFilter){
//                QStringList pieces = sensors[i].bandpassFilter.split( "-" );
//                qJsonObject.insert(Sensor_Bandpass_Filter_Low+sensorUnitNumber, pieces[0]);
//                qJsonObject.insert(Sensor_Bandpass_Filter_High+sensorUnitNumber, pieces[1]);
//            }
            if(sensors[i].sampleRate!=sensors[i].DefaultsampleRate)
                qJsonObject.insert(Sensor_Sample_Rate+sensorUnitNumber, sensors[i].sampleRate);
        }
    }

    QString strJson(jsonToString(qJsonObject));
    qDebug() << "sendSettings : " << strJson;

    sendSerial(strJson);

    QJsonDocument doc(qJsonObject);
    QFile jsonFile("defaultChannelSettings");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
    jsonFile.close();

}

void BackEnd::connectSerialPort(QString portName)
{
    if(!serial->isOpen()) {
        serial->setPortName(portName);
        if(serial->open(QIODevice::ReadWrite)) {
            connectState = true; qDebug() << " conndected : ";
            connect(serial, SIGNAL(readyRead()), SLOT(recieveSerialPort()));
            sendInitialize();sendInitialize();sendInitialize();sendInitialize();sendInitialize();
            updateTime();
            passMinute = minute.toInt();
        } else {
            connectState = false; qDebug() << "Not conndected : ";
            serial->close();
        }
    }
}

QVariant BackEnd::availablePorts()
{
    QList<QSerialPortInfo> portsAvailable = QSerialPortInfo::availablePorts();
    QStringList names_PortsAvailable;
    for(const QSerialPortInfo& portInfo : portsAvailable) {
        names_PortsAvailable<<portInfo.portName();
    }
    return QVariant::fromValue(names_PortsAvailable);
}

void BackEnd::decodeJSON(QString message) {
//    qDebug() << "function start :" << "decodeJSON :" << message;
    updateBatteryCounter++;
    if(updateBatteryCounter==1000)
        updateBatteryCounter=0;
    //qDebug().noquote() << "decodeJSON :" << message;
    QJsonDocument qJsonDocument = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject qJsonObject = qJsonDocument.object();
    if(qJsonObject.contains(Router_Unit_Number) && qJsonObject.contains(Sensor_Num) &&
            qJsonObject.contains(Router_second) && qJsonObject.contains(Sensor_Data_Num) &&
            qJsonObject.contains(Router_milisec) ) {
        routerNum = qJsonObject.value(Router_Unit_Number).toString().toInt();
        sensorQuantity = qJsonObject.value(Sensor_Num).toString().toInt();
        SensorDataSize = qJsonObject.value(Sensor_Data_Num).toString().toInt() * 6;

        updateTime();
        routerMinute = qJsonObject.value(Router_minute).toString().toInt();
        routerSecnd = qJsonObject.value(Router_second).toString().toInt();
        routerMiliSec = qJsonObject.value(Router_milisec).toString().toInt();
//         qDebug() <<"minute:" <<routerMinute<< " , routerSecnd:" << routerSecnd << ",routerMiliSec:" << routerMiliSec ;
        routerBattery=qJsonObject.value(Router_Battery_Level).toString().toInt();
        checkSum = qJsonObject.value(Data_Check_SUM).toString().toInt();
        packetLengh = qJsonObject.value(Router_Packet_Lenth).toString().toInt();
        dataByte.resize(packetLengh-4); // 4 =  2 byte start + 2 byte stop
        flagStart = 0;flagRecieve = 10;packetTimerCounter = 0;
    }
    else if(qJsonObject.contains(ACK)) {
        qDebug() << "decodeJSON : ACK";
    }
    else if(qJsonObject.contains(Sensors_settings)) {
//        qDebug() << "decodeJSON : request time";
        sendInitialize();
    }

    /*
     *else if( qJsonObject.contains(Sensors_settings) && qJsonObject.contains(Routers_Settings) ) {
        qDebug() << "decodeJSON : has Sensors setting data";
        if(qJsonObject.value(Sensors_settings).isArray()) {
            QJsonArray sensorsSettings = qJsonObject.value(Sensors_settings).toArray();
            foreach (const QJsonValue & value, sensorsSettings) {
                QJsonObject obj = value.toObject();
                int sensorNumber = obj.value(Sensor_Unit_Number).toString().toInt();
                QString sensorType = obj.value(Sensor_Type).toString();
                QString sensorBatteryLevel = obj.value(Sensor_Battery_Level).toString();
                qDebug() << "sensor settings :"<< sensorNumber << "," << sensorType << "," << sensorBatteryLevel;
                mList->setSensorsSettings(sensorNumber, sensorType, sensorBatteryLevel);
            }
        }
    }*/

}

void BackEnd::sendSerial(QString temp)
{
//    qDebug() << "function start :" << "sendSerial";
    QByteArray tx_data; tx_data.append(temp);
    serial->write(tx_data);
}

void BackEnd::sendInitialize()
{
//    qDebug() << "function start :" << "sendInitialize";
    updateTime();
    QJsonObject qJsonObject;
    qJsonObject.insert(Sensors_settings, "?");
    qJsonObject.insert(Time_Minute,minute);
    qJsonObject.insert(Time_Second,second);
    qJsonObject.insert(Time_Mili_Second,miliSecond);
    qJsonObject.insert("ALM", 0);
//    qDebug() << "sendInitialize : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::sendAlarmOn()
{
    qDebug() << "function start :" << "sendAlarmOn";
    updateTime();
    QJsonObject qJsonObject;
    qJsonObject.insert(Sensors_settings, "?");
    qJsonObject.insert(Time_Minute,minute);
    qJsonObject.insert(Time_Second,second);
    qJsonObject.insert(Time_Mili_Second,miliSecond);
    qJsonObject.insert("ALM", 1);
    qDebug() << "sendInitialize : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::sendAlarmOff()
{
    qDebug() << "function start :" << "sendAlarmOff";
    updateTime();
    QJsonObject qJsonObject;
    qJsonObject.insert(Sensors_settings, "?");
    qJsonObject.insert(Time_Minute,minute);
    qJsonObject.insert(Time_Second,second);
    qJsonObject.insert(Time_Mili_Second,miliSecond);
    qJsonObject.insert("ALM", 0);
    qDebug() << "sendInitialize : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::sendACK()
{
    qDebug() << "function start :" << "sendACK";
    QJsonObject qJsonObject;
    qJsonObject.insert(ACK, "T");
//    qDebug() << "sendACK : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::sendNACK()
{
    qDebug() << "function start :" << "sendNACK";
    QJsonObject qJsonObject;
    qJsonObject.insert(NACk, "T");
    qDebug() << "sendNACK : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::updateTime()
{
//    qDebug() << "function start :" << "updateTime";
    QTime temp = QTime::currentTime();
    minute = QString::number(temp.minute());
    second = QString::number(temp.second());
    miliSecond = QString::number(temp.msec());
}

void BackEnd::getSettings()
{
    qDebug() << "function start :" << "getSettings";
    QJsonObject qJsonObject;
    qJsonObject.insert(Sensors_Frequency_Parameters, "?");
    QString strJson(jsonToString(qJsonObject));
    qDebug() << "getSettings : " << strJson;
    sendSerial(strJson);
}

void BackEnd::decodeByteArrayType2()
{
       qDebug() << "function start :" << "decodeByteArrayType2";
    uint64_t sum = 0 ;
    for(int i =0; i<dataByte.length();i++) {
        sum = sum + ((uint8_t)dataByte[i]);
    }

    // qDebug() << "length :"<< dataByte.length()<< "checkSum :"<< (sum%256) << " my checkSum:"<< sum;
    if((sum%256) == checkSum) {
        // for TCP socket
        QJsonObject qJsonObjectTemp;
        qJsonObjectTemp.insert(packetType, receiveChartDataType);
        qJsonObjectTemp.insert("R", routerNum);
        qJsonObjectTemp.insert("S", UID);
        qJsonObjectTemp.insert(Router_minute, routerMinute);
        qJsonObjectTemp.insert(Router_second, routerSecnd);
        qJsonObjectTemp.insert(Router_milisec, routerMiliSec);
        QJsonArray xBordar, yBordar, zBordar;

        uint8_t sensorNumber[sensorQuantity];

        for(int i =0; i<dataByte.length();i++) {
            /*
             * To obtain UID and SBL
             * theses two parameters are before starting x y z data for each sensor
             * based on packet template we can access to these two parameters in predefined specific positions
             * updateBatteryCounter counts up to 1000 and then it will reset to zero
             * if it is less that 3 we want to call it setSonsorsetting function which emits a signal for class sensormodel and slot myupdate
             */
            if(i%ProSensorPacketLength==0){
                UID=dataByte[i];
                sensorBatteryLevel=QString::number((int)dataByte[i+1]);
                // qDebug()<<"UID: "<<UID<<"  SBL:"<<sensorBatteryLevel;
                i+=2;

                if(updateBatteryCounter<=3){
                    mList->setSensorsSettings(UID,sensorType,sensorBatteryLevel,routerBattery);
                    //qDebug()<<"update called "<<updateBatteryCounter<<" UID:"<<UID;
                }

            }


            if( ( i%(SensorDataSize+1) ) != 0 ) {
                // 9 forward step data is constant
                int xData = ((dataByte[i] & 0xff) | ((dataByte[i + 1] & 0xff) << 8) | ((dataByte[i + 2] & 0xff) << 16));
                i++;i++;i++;


                if(sensorNumber[i / (SensorDataSize+1)] != 0) {
                    qDebug()<<routerNum << ","<<UID << " X:"<< xData/pow(2,23) ;
                    xBordar.append(xData/pow(2,23));
                    mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "x", xData/pow(2,23), (i%(SensorDataSize+1))/9);
                }

                int yData = ((dataByte[i] & 0xff) | ((dataByte[i + 1] & 0xff) << 8) | ((dataByte[i + 2] & 0xff) << 16));
                i++;i++;i++;

                if(sensorNumber[i / (SensorDataSize+1)] != 0) {
                    qDebug()<<routerNum << ","<<UID << " Y:"<< yData/pow(2,23) ;
                    yBordar.append(yData/pow(2,23));
                    mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "y", yData/pow(2,23), (i%(SensorDataSize+1))/9);
                }

                int zData = ((dataByte[i] & 0xff) | ((dataByte[i + 1] & 0xff) << 8) | ((dataByte[i + 2] & 0xff) << 16));
                i++;i++;

                if(sensorNumber[i / (SensorDataSize+1)] != 0) {
                    qDebug()<<routerNum << ","<<UID << " Z:"<< zData/pow(2,23) ;
                    zBordar.append(zData/pow(2,23));
                    mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "z", zData/pow(2,23), (i%(SensorDataSize+1))/9);
                }
            }
            else {
                sensorNumber[i / (SensorDataSize+1)] =  dataByte[i];
            }
        }
        int couterCheck = 0;
//          qDebug()<< "routerNum : " << routerNum << " , UID : "<< UID ;
        if(checkDataSendToChart1()) {
//              qDebug()<< "checkDataSendToChart1()";
            if(chart1Bordar == 'X') {
//                 qDebug()<< "routerNum : " << routerNum << " , UID : "<< UID ;
                qJsonObjectTemp.insert("chart1", xBordar);couterCheck++;
            } else if(chart1Bordar == 'Y') {
                qJsonObjectTemp.insert("chart1", yBordar);couterCheck++;
            } else if(chart1Bordar == 'Z') {qJsonObjectTemp.insert("chart1", zBordar);couterCheck++;}
        }
        if(checkDataSendToChart2()) {
  //            qDebug()<< "checkDataSendToChart2()";
            if(chart2Bordar == 'X') {
                qJsonObjectTemp.insert("chart2", xBordar);couterCheck++;
            } else if(chart2Bordar == 'Y') {
                qJsonObjectTemp.insert("chart2", yBordar);couterCheck++;
            } else if(chart2Bordar == 'Z') {qJsonObjectTemp.insert("chart2", zBordar);couterCheck++;}
        }
        if(checkDataSendToChart3()) {
  //            qDebug()<< "checkDataSendToChart2()";
            if(chart3Bordar == 'X') {
                qJsonObjectTemp.insert("chart3", xBordar);couterCheck++;
            } else if(chart3Bordar == 'Y') {
                qJsonObjectTemp.insert("chart3", yBordar);couterCheck++;
            } else if(chart3Bordar == 'Z') {qJsonObjectTemp.insert("chart3", zBordar);couterCheck++;}
        }
        if(couterCheck > 0) {
          QJsonDocument doc(qJsonObjectTemp);
//            qDebug() << "send data for chart :"<< doc.toJson(QJsonDocument::Compact);
          pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
        }
    }
    else {
        qDebug() << "**************check sum is wrong";sendNack();
    }
}

void BackEnd::decodeByteArray()
{
  //    qDebug() << "function start :" << "decodeByteArray";
    uint64_t sum = 0 ;
    for(int i =0; i<dataByte.length();i++) {
        sum = sum + ((uint8_t)dataByte[i]);
    }

    //qDebug()<<"start";
    //qDebug() << "length :"<< dataByte.length()<< "checkSum :"<< (sum%256) << " my checkSum:"<< sum;
    if((sum%256) == checkSum) {
        // for TCP socket
        QJsonObject qJsonObjectTemp;
        qJsonObjectTemp.insert(packetType, receiveChartDataType);
        qJsonObjectTemp.insert("R", routerNum);
        qJsonObjectTemp.insert("S", UID);
        qJsonObjectTemp.insert(Router_minute, routerMinute);
        qJsonObjectTemp.insert(Router_second, routerSecnd);
        qJsonObjectTemp.insert(Router_milisec, routerMiliSec);
        QJsonArray xBordar, yBordar, zBordar;

        uint8_t sensorNumber[sensorQuantity];

        for(int i =0; i<dataByte.length();i++) {
           // qDebug()<<i<<"  "<<(int)dataByte[i];
            /*
             * To obtain UID and SBL
             * theses two parameters are before starting x y z data for each sensor
             * based on packet template we can access to these two parameters in predefined specific positions
             */
            if(i%casualSensorPacketLength==0){
                UID=dataByte[i];
                sensorBatteryLevel=QString::number((int)dataByte[i+1]);
                //qDebug()<<"UID: "<<UID<<"  SBL:"<<sensorBatteryLevel;
                i+=2;
                if(updateBatteryCounter<=3){
                    mList->setSensorsSettings(UID,sensorType,sensorBatteryLevel,routerBattery);
                    //qDebug()<<"update called "<<updateBatteryCounter<<" UID:"<<UID;
                }
            }



            if( ( i%(SensorDataSize+1) ) != 0 ) {
                // 6 forward step data is constant
                int xData = (((signed char)dataByte[i] & 0xff) | ((signed char)dataByte[i + 1] << 8))*100;
                    //qDebug()<<(signed)dataByte[i]<<"  "<<(int)dataByte[i+1];
                i++;i++;
  //                    qDebug()<<routerNum << ","<<UID << " X:"<< xData ;
               // if(sensorNumber[i / (SensorDataSize+1)] != 0){
                    xBordar.append(xData/pow(2,15));
                    mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "x", xData/pow(2,15), (i%(SensorDataSize+1))/6);
                //}


                int yData = (((signed char)dataByte[i] & 0xff) | ((signed char)dataByte[i + 1] << 8))*100;
                i++;i++;
               // qDebug()<<"y "<<yData;
  //                qDebug()<<routerNum << ","<<UID << " Y:"<< yData ;

                //if(sensorNumber[i / (SensorDataSize+1)] != 0){
                    yBordar.append(yData/pow(2,15));
                    mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "y", yData/pow(2,15), (i%(SensorDataSize+1))/6);
                //}

                int zData = (((signed char)dataByte[i] & 0xff) | ((signed char)dataByte[i + 1] << 8))*100; i++;
  //                qDebug()<<routerNum << ","<<UID << " Z:"<< zData ;
                //if(sensorNumber[i / (SensorDataSize+1)] != 0){
                    zBordar.append(zData/pow(2,15));
                    mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "z", zData/pow(2,15), (i%(SensorDataSize+1))/6);
                //}
            } else {
                sensorNumber[i / (SensorDataSize+1)] =  dataByte[i];
            }
        }
        int couterCheck = 0;
//          qDebug()<< "routerNum : " << routerNum << " , UID : "<< UID ;
        if(checkDataSendToChart1()) {
//              qDebug()<< "checkDataSendToChart1()";
            if(chart1Bordar == 'X') {
//                 qDebug()<< "routerNum : " << routerNum << " , UID : "<< UID ;
                qJsonObjectTemp.insert("chart1", xBordar);couterCheck++;
            } else if(chart1Bordar == 'Y') {
                qJsonObjectTemp.insert("chart1", yBordar);couterCheck++;
            } else if(chart1Bordar == 'Z') {qJsonObjectTemp.insert("chart1", zBordar);couterCheck++;}
        }
        if(checkDataSendToChart2()) {
  //            qDebug()<< "checkDataSendToChart2()";
            if(chart2Bordar == 'X') {
                qJsonObjectTemp.insert("chart2", xBordar);couterCheck++;
            } else if(chart2Bordar == 'Y') {
                qJsonObjectTemp.insert("chart2", yBordar);couterCheck++;
            } else if(chart2Bordar == 'Z') {qJsonObjectTemp.insert("chart2", zBordar);couterCheck++;}
        }
        if(checkDataSendToChart3()) {
  //            qDebug()<< "checkDataSendToChart2()";
            if(chart3Bordar == 'X') {
                qJsonObjectTemp.insert("chart3", xBordar);couterCheck++;
            } else if(chart3Bordar == 'Y') {
                qJsonObjectTemp.insert("chart3", yBordar);couterCheck++;
            } else if(chart3Bordar == 'Z') {qJsonObjectTemp.insert("chart3", zBordar);couterCheck++;}
        }
        if(couterCheck > 0) {
          QJsonDocument doc(qJsonObjectTemp);
//            qDebug() << "send data for chart :"<< doc.toJson(QJsonDocument::Compact);
          pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
        }
    } else {
        qDebug() << "**************check sum is wrong";sendNack();
    }
}

void BackEnd::resetPacketSwitch()
{
  //    qDebug() << "function start :" << "resetPacketSwitch";
    byteCounter = 0 ;flagStart = 10;flagRecieve = 0;
}

bool BackEnd::checkCheckSum(QString jsonPacket, int checkSum)
{
    qDebug() << "function start :" << "checkCheckSum";
    QString temp = jsonPacket.replace(",\"DCS\":"+jsonPacket.split(",\"DCS\":")[1],"");
    qDebug() << "checkCheckSum : " << temp;
    QByteArray data; data.append(temp);
    uint32_t sum = 0;
    for(int i=0; i<data.length(); i++) {
        sum = sum + data[i];
    }
    qDebug() << "CheckSum : " << checkSum << " sum : " << (sum%256);
    if( (sum%256) == checkSum ) { return true; }
    return false;
}

void BackEnd::sendNack()
{
    qDebug() << "function start :" << "sendNack";
    QJsonObject qJsonObject;
    qJsonObject.insert(ACK, NACk);
    qDebug() << "send Nak : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

bool BackEnd::checkDataSendToChart1()
{
    if(chart1) {
        if(chart1Router == routerNum && chart1Senesor == UID) {
            return true;
        }
    }
    return false;
}

bool BackEnd::checkDataSendToChart2()
{
    if(chart2) {
        if(chart2Router == routerNum && chart2Senesor == UID) {
            return true;
        }
    }
    return false;
}

bool BackEnd::checkDataSendToChart3()
{
    if(chart3) {
        if(chart3Router == routerNum && chart3Senesor == UID) {
            return true;
        }
    }
    return false;
}

void BackEnd::newDecode()
{
// qDebug() << "function start :" << "newDecode";
 uint64_t sum = 0 ;
 // calculate checkSum
 for(int i =0; i<dataByte.length();i++) {
     sum = sum + ((uint8_t)dataByte[i]);
 }
 //check CheckSum
 if((sum%256) == checkSum) {
     QJsonObject sensorsInfoPacket;
     sensorsInfoPacket.insert("router_number", routerNum); sensorsInfoPacket.insert(Router_Battery_Level, routerBattery);
     sensorsInfoPacket.insert(packetType, sensorsInfoType);
     QJsonArray sensorsInfoArray;
     for(int i=0; i < dataByte.length() ; i++) {
        UID=dataByte[i]; i++;
        sensorBatteryLevel = QString::number((int)dataByte[i]);
        if((int)dataByte[i] == -1) {sensorBatteryLevel = "-";}
        i++;
        sensorLossLevel = QString::number(mList->getSensorLoss(routerNum, UID, (int)dataByte[i])); i++;
        int xOffset, yOffset, zOffset;
        mList->getSensorOffset(routerNum, UID, &xOffset, &yOffset, &zOffset);
//        qDebug()<<routerNum << ","<< UID << " offsets :"<<xOffset << " , " << yOffset << " , "<< zOffset;
//        int xOffset = getSensorOffset()
        // for TCP socket
        QJsonObject qJsonObjectTemp, sensorInfo;
        qJsonObjectTemp.insert(packetType, receiveChartDataType);
        qJsonObjectTemp.insert("R", routerNum);
        qJsonObjectTemp.insert("S", UID);
        qJsonObjectTemp.insert(Router_minute, routerMinute);
        qJsonObjectTemp.insert(Router_second, routerSecnd);
        qJsonObjectTemp.insert(Router_milisec, routerMiliSec);
        sensorInfo.insert("sensor_number", UID);
        sensorInfo.insert(antenSignal, sensorLossLevel);
        sensorInfo.insert(Sensor_Battery_Level, sensorBatteryLevel);
        sensorsInfoArray.push_front(sensorInfo);
        QJsonArray xBordar, yBordar, zBordar;
        uint8_t dataNumber = 0;
//        qDebug() << "UID : " << UID << " , " << sensorBatteryLevel << " , " << sensorLossLevel;
        //Low res
        if( (UID/100) == 0) {
            int j = i+60;
            while(i<j) {
                int xData = (((signed char)dataByte[i] & 0xff) | ((signed char)dataByte[i + 1] << 8))*3.9;
                xData = xData - xOffset;
                i++;i++;
                xBordar.append(xData);
                mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "x", xData, dataNumber);

                int yData = (((signed char)dataByte[i] & 0xff) | ((signed char)dataByte[i + 1] << 8))*3.9;
                yData = yData - yOffset;
                i++;i++;
//                 qDebug() << "YData : " << yData ;
                yBordar.append(yData);
                mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "y", yData, dataNumber);

                int zData = (((signed char)dataByte[i] & 0xff) | ((signed char)dataByte[i + 1] << 8))*3.9;
                zData = zData - zOffset;
                i++;i++;
                zBordar.append(zData);
                mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "z", zData, dataNumber);

                dataNumber++;
            }
            // because in for loop it will be plus plus again
            i--;
        } else {
          // high res
            int j = i+90;
            while(i<j) {
                int xData;
                if( (dataByte[i+2] & 0xf0) == 0xf0) {
                    xData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i+2] & 0xff) << 16)  | ((0xff) << 24) );
                } else {
                    xData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i+2] & 0xff) << 16) );
                }
                xData = (xData/pow(2,7)) - xOffset;
//                int xData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i + 2] & 0xff) << 16));
                i++;i++;i++;
                xBordar.append(xData);
                mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "x", xData, dataNumber);

                int yData;
                if( (dataByte[i+2] & 0xf0) == 0xf0) {
                    yData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i+2] & 0xff) << 16)  | ((0xff) << 24) );
                } else {
                    yData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i+2] & 0xff) << 16) );
                }
                yData = (yData/pow(2, 7)) - yOffset;
//                qDebug() << "YData : " << yData;
                i++;i++;i++;

                yBordar.append(yData);
                mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "y", yData, dataNumber);

//                int zData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i + 2] & 0xff) << 16));
                int zData;
                if( (dataByte[i+2] & 0xf0) == 0xf0) {
                    zData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i+2] & 0xff) << 16)  | ((0xff) << 24) );
                } else {
                    zData = (((signed char)dataByte[i] & 0xff) | (((signed char)dataByte[i + 1] & 0xff) << 8) | (((signed char)dataByte[i+2] & 0xff) << 16) );
                }
                zData = (zData/pow(2,7)) - zOffset;
                i++;i++;i++;
                zBordar.append(zData);
                mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, UID, "z", zData, dataNumber);

                dataNumber++;
            }
            // because in for loop it will be plus plus again
            i--;
        }

        // check if need to be sended by TCP
//        if(sendAverage) {
//          sendChartData(qJsonObjectTemp, UID, calculateAverage(xBordar), calculateAverage(yBordar), calculateAverage(zBordar) );
//        } else {
          sendChartData(qJsonObjectTemp, UID, xBordar, yBordar, zBordar);
//        }
     }
     //send sensorsInfo
     sensorsInfoPacket.insert(sensors_Info, sensorsInfoArray);
     QJsonDocument doc(sensorsInfoPacket);
//       qDebug() << "send data for chart :"<< doc.toJson(QJsonDocument::Compact);
     pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
 } else {
     qDebug() << "**************check sum is wrong";sendNack();
 }
}

QJsonArray BackEnd::calculateAverage(QJsonArray dataArray, QVector<double> *fft_vector)
{
    double sum = 0;
    int arraySize = dataArray.size();
    for(int i=0; i< arraySize;i++) {
       double temp_double = dataArray.at(i).toDouble();
//       qDebug() << "calculateAverage temp_double:" << temp_double;
       sum = sum + temp_double;
       fft_vector->push_back(temp_double);
    }

    int dataNumber = fft_second*100;
//    qDebug() << "calculateAverage :" << fft_vector->length() << "," << dataNumber;
    while(fft_vector->length() > dataNumber ) { fft_vector->removeAt(0);}
    QJsonArray temp; temp.append( (double)(sum/arraySize) );
//    qDebug() << "calculateAverage :" << fft_vector->length();
//    for(int j=fft_vector->length()-1;j>fft_vector->length()-9; j--) {
//       qDebug() << "calculateAverage :" << j << " , " << fft_vector->at(j);
//    }
//    qDebug() << "calculateAverage :" << fft_vector->length();
    return temp;
}

void BackEnd::sendChartData(QJsonObject tempQJsonObject, int sensorUID, QJsonArray xBordar, QJsonArray yBordar, QJsonArray zBordar)
{
    int couterCheck = 0;
//          qDebug()<< "routerNum : " << routerNum << " , UID : "<< UID ;
    if(checkDataSendToChart1()) {
//          qDebug()<< "checkDataSendToChart1()";
        if(chart1Bordar == 'X') {
//             qDebug()<< "routerNum : " << routerNum << " , UID : "<< UID ;
            tempQJsonObject.insert("chart1", calculateAverage(xBordar, &chart_fft1) );couterCheck++;
            tempQJsonObject.insert(chart1FFT, calculateFFT(chart_fft1) );
        } else if(chart1Bordar == 'Y') {
            tempQJsonObject.insert("chart1", calculateAverage(yBordar, &chart_fft1) );couterCheck++;
            tempQJsonObject.insert(chart1FFT, calculateFFT(chart_fft1) );
        } else if(chart1Bordar == 'Z') {
            tempQJsonObject.insert("chart1", calculateAverage(zBordar, &chart_fft1) );couterCheck++;
            tempQJsonObject.insert(chart1FFT, calculateFFT(chart_fft1) );
        }
    }
    if(checkDataSendToChart2()) {
//            qDebug()<< "checkDataSendToChart2()";
        if(chart2Bordar == 'X') {
            tempQJsonObject.insert("chart2", calculateAverage(xBordar, &chart_fft2) );couterCheck++;
            tempQJsonObject.insert(chart2FFT, calculateFFT(chart_fft2) );
        } else if(chart2Bordar == 'Y') {
            tempQJsonObject.insert("chart2", calculateAverage(yBordar, &chart_fft2) );couterCheck++;
            tempQJsonObject.insert(chart2FFT, calculateFFT(chart_fft2) );
        } else if(chart2Bordar == 'Z') {
            tempQJsonObject.insert("chart2", calculateAverage(zBordar, &chart_fft2) );couterCheck++;
            tempQJsonObject.insert(chart2FFT, calculateFFT(chart_fft2) );
        }
    }
    if(checkDataSendToChart3()) {
//            qDebug()<< "checkDataSendToChart2()";
        if(chart3Bordar == 'X') {
            tempQJsonObject.insert("chart3", calculateAverage(xBordar, &chart_fft3) );couterCheck++;
            tempQJsonObject.insert(chart3FFT, calculateFFT(chart_fft3) );
        } else if(chart3Bordar == 'Y') {
            tempQJsonObject.insert("chart3", calculateAverage(yBordar, &chart_fft3) );couterCheck++;
            tempQJsonObject.insert(chart3FFT, calculateFFT(chart_fft3) );
        } else if(chart3Bordar == 'Z') {
            tempQJsonObject.insert("chart3", calculateAverage(zBordar, &chart_fft3));couterCheck++;
            tempQJsonObject.insert(chart3FFT, calculateFFT(chart_fft3) );
        }
    }
    if(couterCheck > 0) {
      QJsonDocument doc(tempQJsonObject);
//        qDebug() << "send data for chart :"<< doc.toJson(QJsonDocument::Compact);
      pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
    }
}

QString BackEnd::jsonToString(QJsonObject jsonObject)
{
    QJsonDocument doc(jsonObject);
    return  doc.toJson(QJsonDocument::Compact);
}

QJsonArray BackEnd::calculateFFT(QVector<double> temp)
{
//    qDebug() << "calculateFFT :" << temp.length();
    QJsonArray fft_value;
    if(temp.length() == 0) {return fft_value;}
    fftw_complex out[temp.length()], in[temp.length()];
    for(int j=0; j<temp.length(); j++) {in[j][0] = temp.at(j);in[j][1] = 0;    }
    fftw_plan p = fftw_plan_dft_1d(temp.length(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    int frequencyBand = temp.length();
    if(frequencyBand > 200) {frequencyBand = 200 ;}
    for(int i=0; i< frequencyBand; i++) {
        fft_value.append(sqrt(pow(out[i][0],2)+pow(out[i][1],2) ) );
    }
    fftw_destroy_plan(p);
    fftw_cleanup();
    return fft_value;
}

void BackEnd::appendItem()
{
    mList->appendItem();
}

void BackEnd::timerSlot()
{
    if(QSerialPortInfo::availablePorts().size()>0) {
        if( !serial->isOpen() ) {
            for(int i=0 ; i<QSerialPortInfo::availablePorts().size(); i++ ) {
              serial->setPortName(QSerialPortInfo::availablePorts().at(i).portName());
              qDebug() << " portName : " << QSerialPortInfo::availablePorts().at(i).portName();
              if(serial->open(QIODevice::ReadWrite)) {
                  connectState = true; qDebug() << " conndected : ";
                  connect(serial, SIGNAL(readyRead()), this, SLOT(recieveSerialPort()));
                  sendInitialize();sendInitialize();sendInitialize();sendInitialize();sendInitialize();
                  updateTime();
                  passMinute = minute.toInt();
                  QJsonObject tempQJSONObject;
                  tempQJSONObject.insert(packetType, connectionStateType);
                  tempQJSONObject.insert(connectionState, connected);
                  QJsonDocument doc(tempQJSONObject);
                  pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
                  break;
              } else {
                  connectState = false; qDebug() << "Not conndected : ";
                  serial->close();
              }
            }
//            Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
//                if(serialPortCounterTry == 0 ) {
//                    serial->setPortName(port.portName());
//                    qDebug() << " portName : " << port.portName();
//                    break;
//                    serialPortCounterTry++;
//                } else if()

//            }

        }
    } else {
        serial->close();
        connectState = false;qDebug() << "Disconndected : ";
        QJsonObject tempQJSONObject;
        tempQJSONObject.insert(packetType, connectionStateType);
        tempQJSONObject.insert(connectionState, disconnected);
        QJsonDocument doc(tempQJSONObject);
        pSocket->write(doc.toJson(QJsonDocument::Compact)+"***");
    }
    timerCounter++;
    if(timerCounter == 30) {
        updateTime();timerCounter = 0;
        qDebug()<< storage.bytesAvailable()/1000/1000 << "/" << storage.bytesTotal()/1000/1000 << "MB";
//        qDebug() << " Storage is low " ;
        if( (storage.bytesAvailable()/1000/1000) < 100 ) {
           QDir directory = QDir("Data");
           directory.setSorting(QDir::Time);
           qDebug() << " Storage is too low " ;
           QFileInfoList list = directory.entryInfoList();
           int listSize = list.size() -1;
           int listSizeThresould = listSize * 0.7;
           for(int i=listSize; i>listSizeThresould; i--) {
//               qDebug()<< i << ":" << list[i].fileName();
//               qDebug()<< i << ":" << list[i].filePath();
               QFile file(list[i].filePath());
//               qDebug()<< i << ":" << file.fileName();
               file.remove();
           }
        }
    }
    timerCounter2++;

}

void BackEnd::timerPacketTimeOut()
{
    packetTimerCounter++;
    jsonPacketTimerCounter++;
    if(packetTimerCounter == 9) {
        resetPacketSwitch();qDebug() << "============ packetTimerCounter timeout";
    }
    if(jsonPacketTimerCounter == 9) {
        qDebug() << "============ jsonPacketTimerCounter timeout";
        flagRecieve = 0;
    }
    // protect from overflow
    if(packetTimerCounter > 100) {
        packetTimerCounter = 11 ;
    }
    // protect from overflow
    if(jsonPacketTimerCounter > 100) {
        jsonPacketTimerCounter = 11 ;
    }
}

void BackEnd::recieveSerialPort()
{
    QByteArray data;
    data = serial->readAll();

    for(int i=0;i<data.length();i++) {
        QByteArray *temp = new QByteArray(1,data[i]) ;
        if( data[i] == '{' && flagRecieve == 0  && flagStart == 10) {jsonPacketTimerCounter = 0;//qDebug() << "json start";
            flagRecieve++;recivedSerialPort.clear();recivedSerialPort.append(*temp);//qDebug() << "recieveSerialPort :" << recivedSerialPort;
        } else if(data[i] == '{' && flagRecieve > 0  && flagStart == 10) {
            flagRecieve++;recivedSerialPort.append(*temp);//qDebug() << "flagRecieve :" << flagRecieve;
        } else if(data[i] == '}' && flagRecieve == 1 && flagStart == 10) {
            flagRecieve--;recivedSerialPort.append(*temp); jsonPacketTimerCounter = 11;decodeJSON(recivedSerialPort);//qDebug() << "flagRecieve :" << flagRecieve;//qDebug() << "jsonPacket time counter before:"<<jsonPacketTimerCounter;qDebug() << "jsonPacket time counter after:"<<jsonPacketTimerCounter;
        } else if(data[i] == '}' && flagRecieve > 1 && flagStart == 10) {
            flagRecieve--;recivedSerialPort.append(*temp);//qDebug() << "flagRecieve :" << flagRecieve;
        } else {
            //            uint8_t conv = data[i]; data[i] = 0xA5;
            if(data[i] == START_BYTE0 && flagStart == 0) {
                flagStart++;flagRecieve = 10;
                bufByte = new QByteArray(1,data[i]) ;//qDebug() << "START_BYTE0 ";
            } else if(flagStart == 1) {
                if(data[i] == START_BYTE1) {
                    flagStart++;byteCounter = 0 ;//qDebug() << "START_BYTE1 ";
                } else {flagStart--;recivedSerialPort.append(*bufByte);recivedSerialPort.append(*temp);qDebug() << "Not START_BYTE1 ";}
            } else if(flagStart == 2) {
                if(byteCounter< (packetLengh-4)) {
                    dataByte[byteCounter] = data[i];
                    //                 qDebug() << "byteCounter: "<< byteCounter<< " : " <<((uint8_t)data[i]);
                    byteCounter++;
                } else {
                    byteCounter++;
                    if(data[i] == STOP_BYTE0 && byteCounter == (packetLengh-3)){
                        //qDebug() << "STOP_BYTE0: "<< byteCounter<< " : " <<data[i];;
                    } else if(data[i] == STOP_BYTE1 && byteCounter == (packetLengh-2)){
                        //qDebug() << "STOP_BYTE1: "<< byteCounter<< " : " <<data[i];;
//                        if(routerNum==10)
//                            decodeByteArrayType2();
//                        else
//                            decodeByteArray();
                        // new decode packet function
                        newDecode();
                        resetPacketSwitch();packetTimerCounter = 11 ;
                    } else {
                        qDebug() << "not stop "<<byteCounter << " : " <<data[i];
                    }

                }
            } else {
                recivedSerialPort.append(*temp);
            }
        }
    }

}

void BackEnd::readTcpData()
{
    QByteArray data = pSocket->readAll();
    QString temp;
    temp.append(data);
    qDebug() << "readTcpData :" << temp;
    // check is right packet
    if(temp.contains("***")) {
      // if come multiple packets   
      QStringList packetList =  temp.split("***");
      qDebug()<< "packetList.length() : " << packetList.length();
      // loop through each packet
      for(int i=0; i<packetList.length()-1; i++) {
       QJsonDocument qJsonDocument = QJsonDocument::fromJson(packetList.at(i).toUtf8());
       QJsonObject qJsonObject = qJsonDocument.object();
       if(qJsonObject.contains(packetType)) {
          if(qJsonObject.value(packetType) == selectChartsType) {
              //{'type': 'selectChartsType', 'charts' : [{ R: router, S: sensor, B: chart2Bordar }] }
              QJsonArray chartsJSON = qJsonObject.value("charts").toArray();
              fft_second = qJsonObject.value(fft_chart_second).toString().toInt();
              for(int i=0; i< chartsJSON.size();i++) {
                 QJsonObject tempQJSONObject = chartsJSON.at(i).toObject();
                 // qDebug()<<"tempQJSONObject :" << tempQJSONObject ;
                 // qDebug()<<"empQJSONObject.value :" << tempQJSONObject.value("R").toString().toInt() << " , " << tempQJSONObject.value("S").toString().toInt();
                 if(tempQJSONObject.contains("R") && tempQJSONObject.contains("S") && tempQJSONObject.contains("B")) {
                     if(i==0) {
                        chart1Router = tempQJSONObject.value("R").toString().toInt();
                        chart1Senesor = tempQJSONObject.value("S").toString().toInt();
                        chart1Bordar = tempQJSONObject.value("B").toString();
                        chart1 = true;
                     } else if(i==1) {
                         chart2Router = tempQJSONObject.value("R").toString().toInt();
                         chart2Senesor = tempQJSONObject.value("S").toString().toInt();
                         chart2Bordar = tempQJSONObject.value("B").toString();
                         chart2 = true;
                     } else {
                         chart3Router = tempQJSONObject.value("R").toString().toInt();
                         chart3Senesor = tempQJSONObject.value("S").toString().toInt();
                         chart3Bordar = tempQJSONObject.value("B").toString();
                         chart3 = true;
                     }
               } else {
                   if(i==0) {chart1 = false;} else if(i==1) {chart2 = false;} else {chart3 = false;}
               }
             }
              qDebug()<< chart1Router <<","<< chart1Senesor <<","<< chart1Bordar <<","<< chart2Router <<","<< chart2Senesor <<","<< chart2Bordar <<","<< chart3Router<<","<< chart3Senesor<<","<< chart3Bordar ;
              qDebug()<< "chart1 : "<< chart1 << " chart2 : " << chart2 << " chart3 : " << chart3 << "fft_second" << fft_second ;
              
        } else if(qJsonObject.value(packetType) == eewConfigType) {
              if(algorithmThread) {
                  algorithmThread->setParameters(qJsonObject.value("highPass").toInt(), qJsonObject.value("lowPass").toInt(),
                                                 qJsonObject.value("longPoint").toInt(), qJsonObject.value("shortPoint").toInt(),
                                                 qJsonObject.value("staLtaTreshold").toInt(), qJsonObject.value("winLength").toInt());
                  mList->setFilterFrequency(qJsonObject.value("highPass").toInt(),
                                            qJsonObject.value("lowPass").toInt());
              }
        } else if(qJsonObject.value(packetType) == runTestType) {
            qDebug() << "runTest";
            mList->runTest();
        } else if(qJsonObject.value(packetType) == stopTestType) {
            qDebug() << "stopTest";
            mList->stopTest();
        } else if(qJsonObject.value(packetType) == releaseAlarmType) {
            sendAlarmOn();
        } else if(qJsonObject.value(packetType) == stopAlarmType) {
            sendAlarmOff();
        } else if(qJsonObject.value(packetType) == colibrateType) {
            mList->colibrate();
        } else if(qJsonObject.value(packetType) == sensorsInfo) {
           QJsonArray sensorsInfoArray = qJsonObject.value("sensorsInfo").toArray();
//           int roofRouterNum = qJsonObject.value("onRoof").toObject().value("R").toString().toInt();
//           int roofSensorNum = qJsonObject.value("onRoof").toObject().value("S").toString().toInt();
//           int groundRouterNum = qJsonObject.value("onGround").toObject().value("R").toString().toInt();
//           int groundSensorNum = qJsonObject.value("onGround").toObject().value("S").toString().toInt();
           for(int i=0; i< sensorsInfoArray.size();i++) {
              QJsonObject tempQJSONObject = sensorsInfoArray.at(i).toObject();
              mList->updateSensorInfo(tempQJSONObject);
           }
           bool activateAlgorithm = qJsonObject.value("activateAlgorithm").toBool() ;
           algorithmThread->setRunAlghoritm(activateAlgorithm);
        }
       } else {
         qDebug() << "readTcpData does not have packet type";
       }
      }

    }
}

