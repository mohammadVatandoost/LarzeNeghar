#include "sensor.h"

Sensor::Sensor(int cN, int rN, int sN, QString b)
{
    channelNumber= cN; routerNumber = rN; sensorNumber = sN; bordar = b;
    if(sN>=110)
        sensorType="ADX355";
    else {
        sensorType="ADX345";
    }

    QString SBFH,SBFL;
    QFile jsonFile("defaultChannelSettings");
    jsonFile.open(QFile::ReadOnly);
    QString jString=jsonFile.readAll();
    jsonFile.close();
    QJsonDocument jdefaultSetting= QJsonDocument::fromJson(jString.toUtf8());
    QJsonObject jObjectSetting = jdefaultSetting.object();
    if(jObjectSetting.contains("Desc"+QString::number(sensorNumber)))
        Defaultdescription=description=jObjectSetting.value("Desc"+QString::number(sensorNumber)).toString();
    if(jObjectSetting.contains("SSR"+QString::number(sensorNumber)))
        sampleRate=DefaultsampleRate=jObjectSetting.value("SSR"+QString::number(sensorNumber)).toInt();
    if(jObjectSetting.contains("SBFH"+QString::number(sensorNumber)))
        SBFH=jObjectSetting.value("SBFH"+QString::number(sensorNumber)).toString();
    if(jObjectSetting.contains("SBFL"+QString::number(sensorNumber)))
        SBFL=jObjectSetting.value("SBFL"+QString::number(sensorNumber)).toString();
//    bandpassFilter=DefaultbandpassFilter=SBFL+"-"+SBFH;

}

Sensor::Sensor(int cN, int rN, int sN, QString b, QString d, QString bL, int sR, QString bF, QString s, bool sOL, bool sOW,
               bool sTW, QString sT)
{
    channelNumber= cN; routerNumber = rN; sensorNumber = sN; bordar = b; description = d; sampleRate = sR;
//    bandpassFilter = bF;
    savingOnLocal = sOL; savingOnWeb = sOW; sendingToWeb = sTW ; story = s;
    sensorType = sT;

    //  getDataFromCSV();
}

Sensor::Sensor()
{

}

void Sensor::addSensorLoss(int sensorLossTemp)
{
    sensorLoss.removeFirst();
    sensorLoss.append(sensorLossTemp);
}

int Sensor::getSensorLossAverage()
{
    int sum = 0 ;
    for(int i=0; i<sensorLoss.size(); i++) {
        sum = sum + sensorLoss[i];
    }
    sum = sum / sensorLoss.size();
    return sum;
}

void Sensor::addData(double time, double value)
{
    if( value > max ) {
        if(max < min) { min = max; }
        max = value;
    } else if(value < min) {
        min = value ;
    }
    QPointF temp(time,value);
    data.append(temp);
}

void Sensor::addData(int year, int month, int day, int hour, int minute, int second, int miliSecond, double value)
{
    // calculate offset
    if(counter < average_number) {
        sum = sum + value ;
        counter++;
    }
    if(counter == average_number) {
        offset = sum / average_number ;
        counter++;
    }
    SensorData temp(minute, second, miliSecond, value-offset);
    if(year == 0) {yearBuff = year; monthBuff = month; dayBuff = day; hourBuff = hour; minuteBuff = minute;}
    if( (hourBuff != hour) && (savingOnLocal) && (isConnected) ) {
        qDebug()<< "storing to file add data hourBuff:"<< hourBuff << " , "<< hour ;
        QVector<QStringList> dataStringList;
        for(int j = 0; j<dataList.length(); j++) {
            QStringList listRow;
            listRow.append( QString::number(dataList[j].minute)+"_"+QString::number(dataList[j].second)+"_"+QString::number(dataList[j].miliSecond) );
            listRow.append(QString::number( dataList[j].value ));
            dataStringList.append(listRow);
        }
          appendDataToCSV(dataStringList, "./Data/R"+QString::number(routerNumber)+"S"+QString::number(sensorNumber)+bordar+"_"+QString::number(yearBuff)+"_"+QString::number(monthBuff)+"_"+QString::number(dayBuff)+"_"+QString::number(hourBuff)+".csv"); //"_"+QString::number(minuteBuff)+

        dataList.clear();
    }
    // for savingOnLocal
    if(savingOnLocal && (isConnected)) {
      dataList.append( temp );
    }
    // for test
    if(runTest) {
        testDataList.append(temp);
    }
    // for algorithm
    dataBuffer.append(temp);
    alghorithmDataBuffer.append(abs(value-offset));
    if(earthquackHappen) {
        if(dataBuffer.length() == (dataBufferCounterLimit + dataBufferAfetrLimit) ) {
            QVector<QStringList> dataStringList;
            for(int j = 0; j<dataBuffer.length(); j++) {
                QStringList listRow;
                listRow.append(QString::number(dataBuffer[j].minute)+"_"+QString::number(dataBuffer[j].second)+"_"+QString::number(dataBuffer[j].miliSecond) );
                listRow.append(QString::number( dataBuffer[j].value ));
                dataStringList.append(listRow);
            }
            appendDataToCSV(dataStringList, "./Data/Earthquake_R"+QString::number(routerNumber)+"S"+QString::number(sensorNumber)+bordar+"_"+QString::number(yearBuff)+"_"+QString::number(monthBuff)+"_"+QString::number(dayBuff)+"_"+QString::number(hourBuff)+".csv"); //"_"+QString::number(minuteBuff)+
            dataBuffer.clear();
            alghorithmDataBuffer.clear();
            earthquackHappen = false;
        }
    } else {
       while(dataBuffer.length() > dataBufferCounterLimit) {dataBuffer.remove(0);alghorithmDataBuffer.remove(0);}
    }
//    algorithmRunCounter++;
//    if(algorithmRunCounter == 10) {
//        algorithmRunCounter = 0 ;
        // run algorithm
//    }
    // refresh buffer
    yearBuff = year; monthBuff = month; dayBuff = day; hourBuff = hour; minuteBuff = minute;
}

void Sensor::getDataFromCSV()
{
//    QFile csvFile("./Data/"+QString::number(routerNumber)+QString::number(sensorNumber)+bordar+".csv");
//    if(csvFile.exists()) {
//        QVector<QStringList> sensorData = readCSV("./Data/"+QString::number(routerNumber)+QString::number(sensorNumber)+bordar+".csv", ";");
//        //        qDebug() << "sensorData.length() :" << sensorData.length() ;
//        for(int i=0; i<sensorData.length(); i++) {
//            //            qDebug() << "sensorData[i].length() :" << sensorData[i].length() ;
//            if(sensorData[i].length() == 2) {
//                //              qDebug() << i << " : " << sensorData[i][0].toDouble() << "," << sensorData[i][1].toDouble();
//                addData(sensorData[i][0].toDouble(), sensorData[i][1].toDouble());
//            }
//        }
    //    }
}

void Sensor::startTest()
{
    testDataList.clear();
    runTest = true;
}

void Sensor::stopTest()
{
    runTest = false;
    QVector<QStringList> dataStringList;
    for(int j = 0; j<dataList.length(); j++) {
        QStringList listRow;
        listRow.append( QString::number(dataList[j].minute)+"_"+QString::number(dataList[j].second)+"_"+QString::number(dataList[j].miliSecond) );
        listRow.append(QString::number( dataList[j].value ));
        dataStringList.append(listRow);
    }
    appendDataToCSV(dataStringList, "./Data/Test_R"+QString::number(routerNumber)+"S"+QString::number(sensorNumber)+bordar+".csv");
    testDataList.clear();
}
