#include "backend.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)
Q_DECLARE_METATYPE(QDateTimeAxis *)

BackEnd::BackEnd(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
    qRegisterMetaType<QDateTimeAxis*>();

    serial = new QSerialPort(this);
    serial->close();
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    // timer for connection check
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
    packetTimer = new QTimer(this);
    connect(packetTimer, SIGNAL(timeout()), this, SLOT(timerPacketTimeOut()));
    packetTimer->start(15);
//    decodeJSON("{\"RUN\":\"0\",\"Msc\":\"0\",\"SEC\":\"1\",\"SN\":\"2\",\"SDN\":\"10\",\"RPL\":\"126\",\"DCS\":\"70\"}");
//    decodeJSON("{\"ST\":[{\"SUN\":\"0\",\"SST\":\"0\",\"SBL\":\"0\"},{\"SUN\":\"0\",\"SST\":\"0\",\"SBL\":\"0\"}],\"RT\":[{\"RUN\":\"0\",\"RBL\":\"0\"}]}");
}

void BackEnd::setSensorsList(SensorsList *sensorsList)
{
    mList = sensorsList;
//    mList->addSensor(Sensor(1,2,3,"x"));
//    mList->addData(1, 1, 2, 1, 2, "y", "1234");
}

QVector<Sensor> BackEnd::getSensorsList()
{
    // return QVector<Sensor>
    return mList->items();
}

QList<int> BackEnd::getSenorsX(QString routerNumber, QString sensorNumber, QString bordar)
{
    QVector<Sensor> temp = mList->items();
//    qDebug() << "getSenorsX : " <<routerNumber << " , "<<sensorNumber<<" , "<<bordar;
    for(int i=0; i<temp.length();i++) {
//        qDebug() << i << " : "<<temp.at(i).routerNumber << " , " << temp.at(i).sensorNumber << " , " << temp.at(i).bordar;
        if( (temp.at(i).routerNumber == routerNumber.toInt()) && (temp.at(i).sensorNumber == sensorNumber.toInt())
                && (temp.at(i).bordar == bordar) ) {
            qDebug() << "Founded x";
          return temp.at(i).dataX;
        }
    }
    // must something returned
    qDebug() << " Finished ";
     QList<int> result;
    return result;
}

QList<int> BackEnd::getSenorsY(QString routerNumber, QString sensorNumber, QString bordar)
{
    QVector<Sensor> temp = mList->items();
//    qDebug() << "getSenorsX : " <<routerNumber << " , "<<sensorNumber<<" , "<<bordar;
    for(int i=0; i<temp.length();i++) {
//        qDebug() << i << " : "<<temp.at(i).routerNumber << " , " << temp.at(i).sensorNumber << " , " << temp.at(i).bordar;
        if( (temp.at(i).routerNumber == routerNumber.toInt()) && (temp.at(i).sensorNumber == sensorNumber.toInt())
                && (temp.at(i).bordar == bordar) ) {
            qDebug() << "Founded Y";
          return temp.at(i).dataY;
        }
    }
    // must something returned
    qDebug() << " Finished ";
     QList<int> result;
    return result;
}

void BackEnd::test()
{
   QVector<Sensor> temp = mList->items();
   for(int i=0;i<temp.length();i++) {
       qDebug() << i << " : " << temp[i].description << " : " << temp[i].sampleRate<< " : " << temp[i].bandpassFilter
                << ":" << temp[i].savingOnLocal << ":" << temp[i].sendingToWeb<< ":" << temp[i].savingOnWeb;
   }
}

void BackEnd::sendSettings()
{
//    sendInitialize();
    QVector<Sensor> sensors = mList->items();
    QJsonObject qJsonObject;
//    QJsonArray sensorsArray;

    QString sensorsUnitNumber = "";
    QString sensorsLowPass = "";
    QString sensorsHighPss = "";
    QString sensorsSampleRate = "";
//    char *acsUserName ;
    qJsonObject.insert(Sensors_Frequency_Parameters, "T");
    for(int i=0;i<sensors.length();i++) {
        QString sensorUnitNumber = QString::number(sensors[i].sensorNumber);
        QStringList pieces = sensors[i].bandpassFilter.split( "-" );
        qJsonObject.insert(Sensor_Bandpass_Filter_Low+sensorUnitNumber, pieces[0]);
        qJsonObject.insert(Sensor_Bandpass_Filter_High+sensorUnitNumber, pieces[1]);
        qJsonObject.insert(Sensor_Sample_Rate+sensorUnitNumber, sensors[i].sampleRate);
//        QChar temp = i;
//        uint8_t sensorUnitNumber1 = sensors[i].sensorNumber%256;
//        uint8_t sensorUnitNumber2 = sensors[i].sensorNumber/256;
//        sensorsUnitNumber.append(temp);sensorsUnitNumber.append(temp);
//        int LowPassFileter = pieces[0].toInt();
////        qDebug() << "LowPassFileter : " << LowPassFileter;
//        uint8_t sensorLowPass1 = LowPassFileter%256;
//        uint8_t sensorLowPass2 = LowPassFileter/256;
////        qDebug() << "sensorLowPass1 : " << sensorLowPass1 << "sensorLowPass2 : " << sensorLowPass2 ;
//        sensorsLowPass.append(temp);sensorsLowPass.append(temp);
////        qDebug() << "sensorsLowPass : " << sensorsLowPass;
//        int HighPassFileter = pieces[1].toInt();
////        qDebug() << "HighPassFileter : " << HighPassFileter;
//        uint8_t sensorHighPass1 = HighPassFileter%256;
//        uint8_t sensorHighPass2 = HighPassFileter/256;
////        qDebug() << "sensorHighPass1 : " << sensorHighPass1 << "sensorHighPass2 : " << sensorHighPass2 ;
//        sensorsHighPss.append(temp);sensorsHighPss.append(temp);
////        qDebug() << "sensorsHighPss : " << sensorsHighPss;
//        uint8_t sensorSampleRate1 = sensors[i].sampleRate%256;
//        uint8_t sensorSampleRate2 = sensors[i].sampleRate/256;
//        sensorsSampleRate.append(temp);sensorsSampleRate.append(temp);
    }
//    qJsonObject.insert(Sensor_Unit_Number, sensorsUnitNumber);

//    qJsonObject.insert(Sensors_Frequency_Parameters, sensorsArray);
    QString strJson(jsonToString(qJsonObject));
    qDebug() << "sendSettings : " << strJson;
    sendSerial(strJson);
}

void BackEnd::decodeJSON(QString message) {
    qDebug().noquote() << "decodeJSON :" << message;
    QJsonDocument qJsonDocument = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject qJsonObject = qJsonDocument.object();
     // {"RUN":"0","Msc":"0","SEC":"1","SN":"2","SDN":"10","RPL":"126","DCS":"70"}
    if(qJsonObject.contains(Router_Unit_Number) && qJsonObject.contains(Sensor_Num) &&
            qJsonObject.contains(Router_second) && qJsonObject.contains(Sensor_Data_Num) &&
            qJsonObject.contains(Router_milisec) ) {
        routerNum = qJsonObject.value(Router_Unit_Number).toString().toInt();
        sensorQuantity = qJsonObject.value(Sensor_Num).toString().toInt();
        SensorDataSize = qJsonObject.value(Sensor_Data_Num).toString().toInt() * 6;
        updateTime();
        routerMinute = minute.toInt();
//        qDebug() << "minute" << minute << "second" << second << "miliSecond" << miliSecond ;
        routerSecnd = qJsonObject.value(Router_second).toString().toInt();
        routerMiliSec = qJsonObject.value(Router_milisec).toString().toInt();
        checkSum = qJsonObject.value(Data_Check_SUM).toString().toInt();
        packetLengh = qJsonObject.value(Router_Packet_Lenth).toString().toInt();
        dataByte.resize(packetLengh-4); // 4 =  2 byte start + 2 byte stop
        flagStart = 0;flagRecieve = 10;packetTimerCounter = 0;
//        qDebug() << "routerNum :" << routerNum<< " sensorQuantity :" << sensorQuantity<< " routerMinute :" << routerMinute<< " routerSecnd :" << routerSecnd<< " routerMiliSec :" << routerMiliSec<< " checkSum :" << checkSum<< " packetLengh :" << packetLengh;
    } else if( qJsonObject.contains(Sensors_settings) && qJsonObject.contains(Routers_Settings) ) {
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
    } else if(qJsonObject.contains(ACK)) {
        qDebug() << "decodeJSON : ACK";
    } else if(qJsonObject.contains(Sensors_Frequency_Parameters)) {
        // {"SFP":[{"SUN":"1","SBFL":"0","SBFH":"50","SSR":"200"},{"SUN":"2","SBFL":"20","SBFH":"60","SSR":"200"}]}
        qDebug() << "decodeJSON : has Sensors Sensors_Frequency_Parameters";
        if(qJsonObject.value(Sensors_Frequency_Parameters).isArray()) {
            QJsonArray sensorsSettings = qJsonObject.value(Sensors_Frequency_Parameters).toArray();
            foreach (const QJsonValue & value, sensorsSettings) {
               QJsonObject obj = value.toObject();
               int sensorNumber = obj.value(Sensor_Unit_Number).toString().toInt();
               QString Sample_Filter_Low = obj.value(Sensor_Bandpass_Filter_Low).toString();
               QString Sample_Filter_High = obj.value(Sensor_Bandpass_Filter_High).toString();
               int sample_Frequency = obj.value(Sensor_Sample_Rate).toString().toInt();
               qDebug() << "Sensors_Frequency_Parameters :"<< sensorNumber << "," << Sample_Filter_Low << "," << Sample_Filter_High<< "," << sample_Frequency;
            }
        }
    }
}

void BackEnd::sendSerial(QString temp)
{
    QByteArray tx_data; tx_data.append(temp);
    serial->write(tx_data);
}

void BackEnd::sendInitialize()
{
   updateTime();
   QJsonObject qJsonObject;
   qJsonObject.insert(Sensors_settings, "?");
   qJsonObject.insert(Time_Minute,minute);
   qJsonObject.insert(Time_Second,second);
   qJsonObject.insert(Time_Mili_Second,miliSecond);
   qDebug() << "sendInitialize : " << jsonToString(qJsonObject);
   sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::sendACK()
{
    QJsonObject qJsonObject;
    qJsonObject.insert(ACK, "T");
    qDebug() << "sendACK : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::sendNACK()
{
    QJsonObject qJsonObject;
    qJsonObject.insert(NACk, "T");
    qDebug() << "sendNACK : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

void BackEnd::updateTime()
{
    QTime temp = QTime::currentTime();
    minute = QString::number(temp.minute());
    second = QString::number(temp.second());
    miliSecond = QString::number(temp.msec());
    //    qDebug()<<"updateTime : "<<minute << ":" << second << ":" << miliSecond ;
}

void BackEnd::getSettings()
{
    QJsonObject qJsonObject;
    qJsonObject.insert(Sensors_Frequency_Parameters, "?");
    QString strJson(jsonToString(qJsonObject));
    qDebug() << "getSettings : " << strJson;
    sendSerial(strJson);
}

void BackEnd::decodeByteArray()
{
    uint64_t sum = 0 ;
    for(int i =0; i<dataByte.length();i++) {
        sum = sum + ((uint8_t)dataByte[i]);
    }
//    qDebug() <<((uint8_t)dataByte[0])<<","<<((uint8_t)dataByte[1])<<","<<((uint8_t)dataByte[2])<<","
//             <<((uint8_t)dataByte[3])<<","<<((uint8_t)dataByte[4])<<","<<((uint8_t)dataByte[5])<<","
//             <<((uint8_t)dataByte[6])<<","<<((uint8_t)dataByte[7])<<","<<((uint8_t)dataByte[8])<<","
//             <<((uint8_t)dataByte[9])<<","<<((uint8_t)dataByte[10])<<","<<((uint8_t)dataByte[11])<<","
//             <<((uint8_t)dataByte[12])<<","<<((uint8_t)dataByte[13])<<",";
//    qDebug() << "length :"<< dataByte.length() << "checkSum :"<< (sum%256) << " my checkSum:"<< sum;
    if((sum%256) == checkSum) {
    uint8_t sensorNumber[sensorQuantity];
     for(int i =0; i<dataByte.length();i++) {
       if( ( i%(SensorDataSize+1) ) != 0) {
//           qDebug().noquote() << " passMinute:" << passMinute << " CurrentMinute:" << routerMinute << "sec" << routerSecnd;
           if(passMinute != routerMinute) {
               if(routerSecnd > 57) {
                  routerMinute = passMinute ;
               }
           } else {passMinute = routerMinute ;}

           // 6 forward step data is constant
//            int xData = dataByte[i] + (dataByte[i+1]*256) ; i++;i++;
            int xData = ((dataByte[i] & 0xff) | (dataByte[i + 1] << 8));
            qDebug()<<"xData: " <<i<<":" <<sensorNumber[i / (SensorDataSize+1)]<<","<<((uint8_t)dataByte[i])<<","<<((uint8_t)dataByte[i+1])<<","<< xData;
            i++;i++;
            //qDebug()<<xData;
//            qDebug().noquote() << "decodeByteArray x" << i << "," << (i / (SensorDataSize+1))<< "," << ((i%(SensorDataSize+1))-1)/6;
            mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, sensorNumber[i / (SensorDataSize+1)], "x", xData, (i%(SensorDataSize+1))/6);
//            int yData = dataByte[i] + (dataByte[i+1]*256) ; i++;i++;
//            int yData = dataByte[i] | dataByte[i+1] << 8 ;
            int yData = ((dataByte[i] & 0xff) | (dataByte[i + 1] << 8));
//            qDebug()<<"yData: " <<((uint8_t)dataByte[i])<<","<<((uint8_t)dataByte[i+1])<<","<< yData;
            i++;i++;
//            qDebug().noquote() << "decodeByteArray y " << i << "," << (i / (SensorDataSize+1))<< "," << ((i%(SensorDataSize+1))-1)/6;
            mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, sensorNumber[i / (SensorDataSize+1)], "y", yData, (i%(SensorDataSize+1))/6);
//            int zData = dataByte[i] + (dataByte[i+1]*256) ; i++; // next i++ happen in for
            int zData = ((dataByte[i] & 0xff) | (dataByte[i + 1] << 8)); i++;
            //qDebug()<<zData;

//            qDebug().noquote() << "decodeByteArray z" << i << "," << (i / (SensorDataSize+1))<< "," << ((i%(SensorDataSize+1))-1)/6;
            mList->addData(routerMinute, routerSecnd, routerMiliSec, routerNum, sensorNumber[i / (SensorDataSize+1)], "z", zData, (i%(SensorDataSize+1))/6);
       } else {
         sensorNumber[i / (SensorDataSize+1)] =  dataByte[i];
       }
     }
    } else {
         qDebug() << "**************check sum is wrong";sendNack();
//         qDebug().noquote() << dataByte.toHex();
    }
}

void BackEnd::resetPacketSwitch()
{
    byteCounter = 0 ;flagStart = 10;flagRecieve = 0;
}

bool BackEnd::checkCheckSum(QString jsonPacket, int checkSum)
{
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
    QJsonObject qJsonObject;
    qJsonObject.insert(ACK, NACk);
    qDebug() << "send Nak : " << jsonToString(qJsonObject);
    sendSerial(jsonToString(qJsonObject)) ;
}

QString BackEnd::jsonToString(QJsonObject jsonObject)
{
    QJsonDocument doc(jsonObject);
    return  doc.toJson(QJsonDocument::Compact);
}

void BackEnd::updateChart(QAbstractSeries *chartSeries)
{
    if (chartSeries) {
//        qDebug() << "updateChart" << chartSeries->name();
        QXYSeries *xySeries = static_cast<QXYSeries *>(chartSeries);
        QVector<Sensor> sensors = mList->items();
        QStringList listItems = chartSeries->name().split(',', QString::SkipEmptyParts);
        for(int j=0;j<listItems.length();j++) {
            listItems[j] = listItems[j].replace(" ","");
        }
//        QString finalString = listItems[1];
        for(int i=0;i<sensors.length();i++) {
            if( (sensors[i].routerNumber == listItems[0].toInt()) && (sensors[i].sensorNumber == listItems[1].toInt())
                    && (sensors[i].bordar == listItems[2]) ) {
                QVector<QPointF> points = sensors[i].data;
                xySeries->replace(points);
                if(sensors[i].bordar == "x") {
                    double minXtime = mList->getLastDataTime("x")-(30*1000/mList->zoomScaleX)+mList->scrollXTime;
                    double maxXtime = mList->getLastDataTime("x")+(30*1000/mList->zoomScaleX)+mList->scrollXTime;
                    axisXTime->setMin(QDateTime::fromMSecsSinceEpoch( minXtime ) );
                    axisXTime->setMax(QDateTime::fromMSecsSinceEpoch( maxXtime ) );
                } else if(sensors[i].bordar == "y") {
                    axisYTime->setMin(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("y")-(30*1000) ) );
                    axisYTime->setMax(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("y")+(30*1000) ) );
                } else if(sensors[i].bordar == "z") {
                    axisZTime->setMin(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("z")-(30*1000) ) );
                    axisZTime->setMax(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("z")+(30*1000) ) );
                }
            }
        }
    }
}

void BackEnd::setAxisXTime(QDateTimeAxis *axis)
{
    axisXTime = axis;qDebug() << "setAxisXTime()" << axisXTime->format();
}

void BackEnd::setAxisYTime(QDateTimeAxis *axis)
{
    axisYTime = axis;
}

void BackEnd::setAxisZTime(QDateTimeAxis *axis)
{
    axisZTime = axis;
}

void BackEnd::appendItem()
{
//   Sensor temp;
    mList->appendItem();
}

void BackEnd::timerSlot()
{
   if(QSerialPortInfo::availablePorts().size()>0) {
    if(!serial->isOpen()) {
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
                   serial->setPortName(port.portName());
                   qDebug() << " portName : " << port.portName();
        }
        serial->setPortName("COM11");
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
   } else {
       serial->close();
       connectState = false;qDebug() << "Disconndected : ";
   }
   timerCounter++;
   if(timerCounter == 30) {updateTime();timerCounter = 0;}
   if(timerCounter2 == 1) {sendInitialize();}
   timerCounter2++;
   if(timerCounter2 == 200) {sendInitialize();timerCounter2 = 2;}
//   qDebug() << "mList length :" << mList->items().length();
//   mList->addData(1, 1, 2, 1, 2, "z", "1234");
   //   sendInitialize();
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
//                 qDebug() << "byteCounter: "<< byteCounter<< " : " <<data[i];;
                 byteCounter++;
                } else {
                  byteCounter++;
                  if(data[i] == STOP_BYTE0 && byteCounter == (packetLengh-3)){
//                       qDebug() << "STOP_BYTE0: "<< byteCounter<< " : " <<data[i];;
                  } else if(data[i] == STOP_BYTE1 && byteCounter == (packetLengh-2)){
//                       qDebug() << "STOP_BYTE1: "<< byteCounter<< " : " <<data[i];;
                       decodeByteArray();resetPacketSwitch();packetTimerCounter = 11 ;
                  } else {
//                      qDebug() << "not stop "<<byteCounter << " : " <<data[i];
                  }
//                  if(byteCounter == 124) {
//                  }
                }
            } else {
//                if(flagRecieve==10){qDebug() << "byteCounter "<<byteCounter<< " flagStart "<<flagStart;}
//                if(byteCounter>10){qDebug() << "byteCounter "<<byteCounter<< " flagRecieve "<<flagRecieve<< " flagStart "<<flagStart;}
              recivedSerialPort.append(*temp);
            }
        }
//       qDebug() << " :" << data[i];
    }
//    recivedSerialPort.append(temp);
//   qDebug() << "recieveSerialPort :" << QString::fromStdString(data.toStdString());
}

//void BackEnd::addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
//                      QString sensorBordar, QString sensorDatas)
//{
////    addData(int min, int sec, int milSec, int routerNumber, int sensorNumber, QString sensorBordar,
////                              int dataValue, uint8_t mili)
////    mList->addData(min, sec, milSec, routerNumber, sensorNumber, sensorBordar, sensorDatas);
//}
