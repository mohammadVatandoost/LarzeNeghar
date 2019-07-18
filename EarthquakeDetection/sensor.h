#ifndef SENSOR_H
#define SENSOR_H
#include <QtCore>
#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QPointF>
#include "csv.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <sensordata.h>

//struct SensorData {
//    int dataAccelarator;
//    int time;
//};
#define average_number 1000
#define dataBufferCounterLimit 30000 // for 30 seconds
#define dataBufferAfetrLimit 120000 // for 120 seconds (after eqthrquake detected)

class Sensor
{
public:
    Sensor(int cN, int rN, int sN, QString b);
    Sensor(int cN, int rN, int sN, QString b, QString d, QString bL, int sR, QString bF, QString s, bool sOL, bool sOW,
           bool sTW, QString sT);
    Sensor();
    // int cN,int uN, QString d, QString bL, int sR, QString bF, bool sOL, bool sOW, bool sTW, QString s, QString sT
    int channelNumber;// = 1;
    int routerNumber;// = 123;
    int sensorNumber;// = 345;
    QString bordar;// = "x";
    QString description = "Sensor";
    QString batteryLevel;// = "80%";
    QString routerBatteryLevel;
    int sampleRate = 200;
    int counter = 0 ;
    int sum = 0;
    QString low_pass = "15";
    QString high_pass = "0.1";
    QString Defaultbordar;
    QString Defaultdescription="Sensor";
    int DefaultsampleRate;
    QString DefaultbandpassFilter;
    uint8_t algorithmRunCounter = 0;
    bool savingOnLocal = false;
    bool savingOnWeb = false;
    bool sendingToWeb = false;
    bool runTest = false ;
    bool isConnected = true;
    bool onRoofSensor = false;
    bool onGroundSensor = false;
    bool earthquackHappen = false;
    QString story = "4x";
    QString sensorType = "A";
    QList<int> dataX;
    QList<int> dataY;
    QVector<QPointF> data;
    QVector<SensorData> dataList, testDataList, dataBuffer;
    QVector<double> alghorithmDataBuffer;
    int yearBuff = 0;
    int monthBuff, dayBuff, hourBuff, minuteBuff;
    QList<int> sensorLoss = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    void addSensorLoss(int sensorLossTemp);
    int getSensorLossAverage();
    void addData(double time, double value);
    void addData(int year, int month, int day, int hour, int minute, int second, int miliSecond, double value);
    void getDataFromCSV();
    void startTest();
    void stopTest();
    int min = 0;
    int max = 0;
    int offset = 0;

};

#endif // SENSOR_H
