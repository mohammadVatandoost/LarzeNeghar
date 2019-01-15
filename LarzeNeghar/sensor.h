#ifndef SENSOR_H
#define SENSOR_H
#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QPointF>
#include "csv.h"

struct SensorData {
    int dataAccelarator;
    int time;
};

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
    int sampleRate = 200;
    QString bandpassFilter = "0-50";
    bool savingOnLocal = false;
    bool savingOnWeb = false;
    bool sendingToWeb = false;
    QString story = "4x";
    QString sensorType = "A";
    QList<int> dataX;
    QList<int> dataY;
    QVector<QPointF> data;
    void addData(double time, double value);
    void getDataFromCSV();
    int min = 0;
    int max = 0;

};

#endif // SENSOR_H
