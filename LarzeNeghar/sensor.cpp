#include "sensor.h"

Sensor::Sensor(int cN, int rN, int sN, QString b)
{
   channelNumber= cN; routerNumber = rN; sensorNumber = sN; bordar = b;
   getDataFromCSV();
}

Sensor::Sensor(int cN, int rN, int sN, QString b, QString d, QString bL, int sR, QString bF, QString s, bool sOL, bool sOW,
               bool sTW, QString sT)
{
  channelNumber= cN; routerNumber = rN; sensorNumber = sN; bordar = b; description = d; sampleRate = sR;
  bandpassFilter = bF;  savingOnLocal = sOL; savingOnWeb = sOW; sendingToWeb = sTW ; story = s;
  sensorType = sT;
  getDataFromCSV();
}

Sensor::Sensor()
{

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

void Sensor::getDataFromCSV()
{
    QFile csvFile("./Data/"+QString::number(routerNumber)+QString::number(sensorNumber)+bordar+".csv");
    if(csvFile.exists()) {
        QVector<QStringList> sensorData = readCSV("./Data/"+QString::number(routerNumber)+QString::number(sensorNumber)+bordar+".csv", ";");
//        qDebug() << "sensorData.length() :" << sensorData.length() ;
        for(int i=0; i<sensorData.length(); i++) {
//            qDebug() << "sensorData[i].length() :" << sensorData[i].length() ;
            if(sensorData[i].length() == 2) {
//              qDebug() << i << " : " << sensorData[i][0].toDouble() << "," << sensorData[i][1].toDouble();
              addData(sensorData[i][0].toDouble(), sensorData[i][1].toDouble());
            }
        }
    }
}
