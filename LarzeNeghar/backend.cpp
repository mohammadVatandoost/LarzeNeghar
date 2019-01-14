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
    if(serial->setBaudRate(QSerialPort::Baud115200)) {
        qDebug() << "baudrate set" << serial->baudRate();
    } else {qDebug() << "baudrate do not set";}
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    // timer for connection check
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
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
    message = message.remove("\t");
    message = message.remove("\n");
    qDebug().noquote() << "decodeJSON :" << message;
    QJsonDocument qJsonDocument = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject qJsonObject = qJsonDocument.object();
//    if(qJsonObject.contains(Data_Check_SUM)) {
////        qDebug() << "has check sum";
//        if(checkCheckSum(message, stringByteToInt(qJsonObject.value(Data_Check_SUM).toString()) )) {
//           qDebug() << "check sum correct";sendACK();
//        } else {qDebug() << "check sum wrong"; sendNACK();}
//    }

//    qDebug() << "decodeJSON test:" << jsonToString(qJsonObject);
//    if(qJsonObject.contains(Sensor_Data)) {qDebug() << "decodeJSON test Sensor_Data:"  ;}
//    if(qJsonObject.contains(Router_Unit_Number)) {qDebug() << "decodeJSON test Router_Unit_Number:" << qJsonObject.value(Router_Unit_Number).toInt();}
//    if(qJsonObject.contains(Router_minute)) {qDebug() << "decodeJSON test Router_minute:" << qJsonObject.value(Router_minute).toInt()  ;}
//    if(qJsonObject.contains(Router_second)) {qDebug() << "decodeJSON test Router_second:" << qJsonObject.value(Router_second).toInt()  ;}
//    if(qJsonObject.contains(Router_milisec)) {qDebug() << "decodeJSON test Router_milisec:" << qJsonObject.value(Router_milisec).toInt()  ;}

    if(qJsonObject.contains(Sensor_Data) && qJsonObject.contains(Router_Unit_Number) &&
            qJsonObject.contains(Router_second) &&
            qJsonObject.contains(Router_milisec) ) {
//        qDebug() << "decodeJSON : routerNumber" << stringByteToInt(qJsonObject.value(Router_Unit_Number).toString());
        int routerNumber = qJsonObject.value(Router_Unit_Number).toInt(); //stringByteToInt(qJsonObject.value(Router_Unit_Number).toString());
        int min = minute.toInt();
        int sec = qJsonObject.value(Router_second).toInt(); //stringByteToInt(qJsonObject.value(Router_second).toString());
        int milSec = qJsonObject.value(Router_milisec).toInt(); //stringByteToInt(qJsonObject.value(Router_milisec).toString());
//        qDebug() << "sec :" << sec << ", milSec: " << milSec ;
        QJsonArray sensorsData = qJsonObject.value(Sensor_Data).toArray();
        foreach (const QJsonValue & value, sensorsData) {
            QJsonObject obj = value.toObject();
            if(obj.contains(Sensor_Unit_Number)) {
//                qDebug() << "decodeJSON : has Sensor_Unit_Number" << stringByteToInt(obj.value(Sensor_Unit_Number).toString());
                int sensorNumber = qJsonObject.value(Sensor_Unit_Number).toInt(); //stringByteToInt(obj.value(Sensor_Unit_Number).toString());
                qDebug() << "Sensor_Unit_Number : " << sensorNumber;
                if(obj.contains(Sensor_Data_X)) {
//                    qDebug() << "Sensor_Data_X  ";
                   addData(min, sec, milSec, routerNumber, sensorNumber,"x",obj.value(Sensor_Data_X).toString());
                }
                if(obj.contains(Sensor_Data_Y)) {
//                    qDebug() << "Sensor_Data_Y  ";
                   addData(min, sec, milSec, routerNumber, sensorNumber,"y",obj.value(Sensor_Data_Y).toString());
                }
                if(obj.contains(Sensor_Data_Z)) {
//                    qDebug() << "Sensor_Data_Z  ";
                   addData(min, sec, milSec, routerNumber, sensorNumber,"z",obj.value(Sensor_Data_Z).toString());
                }
            }
        }
        getSettings();
    } else if( qJsonObject.contains(Sensors_settings) && qJsonObject.contains(Routers_Settings) ) {
       qDebug() << "decodeJSON : has Sensors setting data";
       QJsonArray sensorsSettings = qJsonObject.value(Sensors_settings).toArray();
       foreach (const QJsonValue & value, sensorsSettings) {
           QJsonObject obj = value.toObject();
           int sensorNumber = qJsonObject.value(Sensor_Unit_Number).toInt(); //stringByteToInt(obj.value(Sensor_Unit_Number).toString());
           QString sensorType = obj.value(Sensor_Type).toString();
           QString sensorBatteryLevel = obj.value(Sensor_Battery_Level).toString();
           qDebug() << "sensor settings :"<< sensorNumber << "," << sensorType << "," << sensorBatteryLevel;
           mList->setSensorsSettings(sensorNumber, sensorType, sensorBatteryLevel);
       }
//       QJsonArray routersSettings = qJsonObject.value(Routers_Settings).toArray();
    }
}

void BackEnd::sendSerial(QString temp)
{
//    temp.remove(0, 1);
//    temp = temp.left(temp.length() - 1);
    qDebug() << "sendSerial :"<< temp;
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

int BackEnd::stringByteToInt(QString temp)
{
    int dataValue = -1 ;
    qDebug() << "stringByteToInt length" << temp.length();
    for(int j=0; j<temp.length();j++) {
        QChar first_8bit = temp[j];
        if(temp.length() == 2) {
         QChar second_8bit = temp[j+1];
         dataValue = first_8bit.toLatin1() + second_8bit.toLatin1()*256 ;
        } else {
         dataValue = first_8bit.toLatin1() ;
        }
    }
    return dataValue;
}

bool BackEnd::checkCheckSum(QString jsonPacket, int checkSum)
{
    QString temp = jsonPacket.replace(",\"DCS\":"+jsonPacket.split(",\"DCS\":")[1],"");
    qDebug().noquote() << "checkCheckSum : " << temp;
    QByteArray data; data.append(temp);
    uint32_t sum = 0;
    for(int i=0; i<data.length(); i++) {
       sum = sum + data[i];
    }
    qDebug() << "CheckSum : " << checkSum << " sum : " << (sum%256);
    if( (sum%256) == checkSum ) { return true; }
    return false;
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
        if(serial->open(QIODevice::ReadWrite)) {
          connectState = true; qDebug() << " conndected : ";
          connect(serial, SIGNAL(readyRead()), SLOT(recieveSerialPort()));
          sendInitialize();//sendInitialize();sendInitialize();sendInitialize();sendInitialize();
        } else {
            connectState = false; qDebug() << "Not conndected : ";
            serial->close();
        }
    }
   } else {
       serial->close();
       connectState = false; //qDebug() << "Disconndected : ";
   }
   timerCounter++;
   if(timerCounter == 30) {updateTime();timerCounter = 0;}
//   if(timerCounter2 == 1) {sendInitialize();}
//   timerCounter2++;
//   if(timerCounter2 == 200) {sendInitialize();timerCounter2 = 2;}
//   qDebug() << "mList length :" << mList->items().length();
//   mList->addData(1, 1, 2, 1, 2, "z", "1234");
//   sendInitialize();
}

void BackEnd::recieveSerialPort()
{
    QByteArray data;
    data = serial->readAll();

    for(int i=0;i<data.length();i++) {
        QByteArray *temp = new QByteArray(1,data[i]) ;
        if( data[i] == '{' && flagRecieve == 0) {
            flagRecieve++;recivedSerialPort.clear();recivedSerialPort.append(*temp);//qDebug() << "recieveSerialPort :" << recivedSerialPort;
        } else if(data[i] == '{' && flagRecieve > 0) {
            flagRecieve++;recivedSerialPort.append(*temp);//qDebug() << "recieveSerialPort :" << recivedSerialPort;
        } else if(data[i] == '}' && flagRecieve == 1) {
            flagRecieve--;recivedSerialPort.append(*temp);decodeJSON(recivedSerialPort);
        } else if(data[i] == '}' && flagRecieve > 1) {
            flagRecieve--;recivedSerialPort.append(*temp);//qDebug() << "recieveSerialPort :" << recivedSerialPort;
        } else {
            recivedSerialPort.append(*temp);
        }
//       qDebug() << " :" << data[i];
    }
//    recivedSerialPort.append(temp);
//   qDebug() << "recieveSerialPort :" << QString::fromStdString(data.toStdString());
}

void BackEnd::addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
                      QString sensorBordar, QString sensorDatas)
{
    mList->addData(min, sec, milSec, routerNumber, sensorNumber, sensorBordar, sensorDatas);
}
