#ifndef SENSORSLIST_H
#define SENSORSLIST_H

#include <QObject>
#include "sensor.h"
#include <QVector>
#include <QDateTime>
#include "QDebug"
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "csv.h"

class SensorsList : public QObject
{
    Q_OBJECT
public:
    explicit SensorsList(QObject *parent = nullptr);
    bool setSensorItem(int index,Sensor &sensor);
    QVector<Sensor> items();
    QVector<Sensor> sensorItems;
    void addSensor(Sensor newSensor);
    void addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
                 QString sensorBordar, QString sensorDatas);
    Q_INVOKABLE int getSensorXmin();
    Q_INVOKABLE int getSensorXmax();
    Q_INVOKABLE int getSensorYmin();
    Q_INVOKABLE int getSensorYmax();
    Q_INVOKABLE int getSensorZmin();
    Q_INVOKABLE int getSensorZmax();
    double getLastDataTime(QString bordar);
    int sensorXmin = 0;
    int sensorXmax = 100;
    double XmaxTime = QDateTime::fromString("Monday, 23 April 01 22:51:41","dddd, d MMMM yy hh:mm:ss").toMSecsSinceEpoch();
    int sensorYmin = 0;
    int sensorYmax = 2;
    double YmaxTime = XmaxTime;
    int sensorZmin = 0;
    int sensorZmax = 2;
    double ZmaxTime = XmaxTime;
    int channelNumCounter = 1 ;
    void changeMinMax(int minValue, int maxValue, QString bordar);
signals:
   void preItemAppended();
   void postItemAppended();

   void preItemRemoved(int index);
   void postItemRemoved();

public slots:
    void appendItem(); //Sensor sensorItem
    void removeItem(int index);
private:

};

#endif // SENSORSLIST_H
