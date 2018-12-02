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
    serial->setBaudRate(QSerialPort::Baud9600);
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
    QVector<Sensor> sensors = mList->items();
    QJsonObject qJsonObject;
    QJsonArray sensorsArray;
    for(int i=0;i<sensors.length();i++) {
        QJsonObject temp;
        temp.insert(Sensor_Sample_Rate, sensors[i].sampleRate);
        temp.insert(Sensor_Bandpass_Filter, sensors[i].bandpassFilter);
        temp.insert(Sensor_Unit_Number, QString::number(sensors[i].routerNumber)+QString::number(sensors[i].sensorNumber)
                    +sensors[i].bordar);
        sensorsArray.append(temp);
    }
    qJsonObject.insert(Sensors_settings,sensorsArray);
    QString strJson(jsonToString(qJsonObject));
    qDebug() << "sendSettings : " << strJson;
    sendSerial(strJson);
}

void BackEnd::decodeJSON(QString message) {
    message = message.remove("\t");
    message = message.remove("\n");
    qDebug() << "decodeJSON :" << message;
    QJsonDocument qJsonDocument = QJsonDocument::fromJson(message.toUtf8());

    QJsonObject qJsonObject = qJsonDocument.object();
    qDebug() << "decodeJSON test:" << jsonToString(qJsonObject);
    if(qJsonObject.contains(Sensors_Data) && qJsonObject.contains(Router_Unit_Number) &&
            qJsonObject.contains(Router_minute) && qJsonObject.contains(Router_second) &&
            qJsonObject.contains(Router_milisec) ) {
        qDebug() << "decodeJSON : has Sensors_Data";
        int routerNumber = qJsonObject.value(Router_Unit_Number).toInt();
        int min = qJsonObject.value(Router_minute).toInt();
        int sec = qJsonObject.value(Router_second).toInt();
        int milSec = qJsonObject.value(Router_milisec).toInt();
        QJsonArray sensorsData = qJsonObject.value(Sensors_Data).toArray();
        foreach (const QJsonValue & value, sensorsData) {
            QJsonObject obj = value.toObject();
            if(obj.contains(Sensor_Unit_Number)) {
                qDebug() << "decodeJSON : has Sensor_Unit_Number";
                int sensorNumber = obj.value(Sensor_Unit_Number).toInt();
                if(obj.contains(Sensor_Data_X)) {
                   addData(min, sec, milSec, routerNumber, sensorNumber,"x",obj.value(Sensor_Data_X).toString());
                }
                if(obj.contains(Sensor_Data_Y)) {
                   addData(min, sec, milSec, routerNumber, sensorNumber,"y",obj.value(Sensor_Data_Y).toString());
                }
                if(obj.contains(Sensor_Data_Z)) {
                   addData(min, sec, milSec, routerNumber, sensorNumber,"z",obj.value(Sensor_Data_Z).toString());
                }
            }
        }
    } else if( qJsonObject.contains(Sensor_Unit_Number) && qJsonObject.contains(Sensor_Data_X) &&
              qJsonObject.contains(Sensor_Data_Y) && qJsonObject.contains(Sensor_Data_Z) ) {
       qDebug() << "decodeJSON : has Sensors setting data";
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

void BackEnd::updateTime()
{
    QTime temp = QTime::currentTime();
    minute = QString::number(temp.minute());
    second = QString::number(temp.second());
    miliSecond = QString::number(temp.msec());
    //    qDebug()<<"updateTime : "<<minute << ":" << second << ":" << miliSecond ;
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
                    axisXTime->setMin(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("x")-(30*1000) ) );
                    axisXTime->setMax(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("x")+(30*1000) ) );
                } else if(sensors[i].bordar == "y") {
                    axisYTime->setMin(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("y")-(30*1000) ) );
                    axisYTime->setMax(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("y")+(30*1000) ) );
                } else if(sensors[i].bordar == "z") {
                    axisZTime->setMin(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("z")-(30*1000) ) );
                    axisZTime->setMax(QDateTime::fromMSecsSinceEpoch( mList->getLastDataTime("z")+(30*1000) ) );
                }
//                qDebug() << "points.length()" << points.length();
//                for(int j=0;j<points.length();j++) {
//                    qDebug() << QTime::fromMSecsSinceStartOfDay(points[j].x()).toString() << " , " << points[j].y();
//                    qDebug() << points[j].x() << " , " << points[j].y();
//                }
//                QDateTimeAxis *axisX = new QDateTimeAxis;
//                axisX->setTickCount(6);
//                axisX->setFormat("MMM yyyy");
//                qDebug() << "axisX->format()" << axisX->format();
//                for(int j=0;j<chartSeries->attachedAxes().length();j++) {
//                   qDebug() << "chartSeries :" <<  chartSeries->attachedAxes()[j];
//                }
//                axisX->setTitleText("Date");
//                chart->addAxis(axisX, Qt::AlignBottom);
//                series->attachAxis(axisX);

//                xySeries->clear();
                // Use replace instead of clear + append, it's optimized for performance
//                axisXTime->setMin(QDateTime::fromString("01:00:00","hh:mm:ss"));
//                axisXTime->setMax(QDateTime::fromString("05:00:00","hh:mm:ss"));
//                axisXTime->setMin(QDateTime::currentDateTime());
//                axisXTime->setMax(QDateTime::fromMSecsSinceEpoch(QDateTime::currentDateTime().toMSecsSinceEpoch()+3600000));
//                QDateTime temp;temp.setTime(QTime(1, 10, 00, 345));
//                xySeries->append(QDateTime::fromString("01:10:00:345","hh:mm:ss:zzz").toMSecsSinceEpoch(),10);
//                qDebug() << "true" << QDateTime::currentDateTime().toMSecsSinceEpoch();
//                qDebug() << "not true" << QDateTime::currentDateTime().toMSecsSinceEpoch()+3600000;
//                xySeries->append(QDateTime::currentDateTime().toMSecsSinceEpoch()+360000,10);
//                xySeries->append(QDateTime::currentDateTime().toMSecsSinceEpoch()+720000,20);
//                xySeries->append(temp.t oMSecsSinceEpoch(),50);
//                xySeries->append(QTime(3, 10, 10, 0).msecsSinceStartOfDay(),60);
//                xySeries->append(QDateTime::fromString("04:10:00:678","hh:mm:ss:zzz").toMSecsSinceEpoch(),30);
//                xySeries->append(QDateTime::fromString("04:10:00","hh:mm:ss").toMSecsSinceEpoch(),40);
//                xySeries->append(QDateTime::fromString("05:10:00","hh:mm:ss").toMSecsSinceEpoch(),50);

//                qDebug() << "is equal";
            }
        }
//        if(sensors.length() > 0) {

//            qDebug() << "addData" << temp.at(index).dataY.length();
//            temp[index].addData(time,data);
//        }
//        m_index++;
//        if (m_index > m_data.count() - 1)
//            m_index = 0;

//        QVector<QPointF> points = m_data.at(m_index);
        // Use replace instead of clear + append, it's optimized for performance
//        xySeries->replace(points);

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
          sendInitialize();
        } else {
            connectState = false; qDebug() << "Not conndected : ";
            serial->close();
        }
    }
   } else {
       serial->close();
       connectState = false; //qDebug() << "Disconndected : ";
   }
//   sendInitialize();
}

void BackEnd::recieveSerialPort()
{
    QByteArray data;
    data = serial->readAll();

    for(int i=0;i<data.length();i++) {
        QByteArray *temp = new QByteArray(1,data[i]) ;
        if( data[i] == '{' && flagRecieve == 0) {
            flagRecieve++;recivedSerialPort.clear();recivedSerialPort.append(*temp);
        } else if(data[i] == '{' && flagRecieve > 0) {
            flagRecieve++;recivedSerialPort.append(*temp);
        } else if(data[i] == '}' && flagRecieve == 1) {
            flagRecieve--;recivedSerialPort.append(*temp);decodeJSON(recivedSerialPort);
        } else if(data[i] == '}' && flagRecieve > 1) {
            flagRecieve--;recivedSerialPort.append(*temp);
        } else {
            recivedSerialPort.append(*temp);
        }
//       qDebug() << " :" << data[i];
    }
//    recivedSerialPort.append(temp);
//        qDebug() << "recieveSerialPort :" << recivedSerialPort;
}

void BackEnd::addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
                      QString sensorBordar, QString sensorDatas)
{
    mList->addData(min, sec, milSec, routerNumber, sensorNumber, sensorBordar, sensorDatas);
}
