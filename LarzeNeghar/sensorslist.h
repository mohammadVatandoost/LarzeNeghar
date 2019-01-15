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
#include <QTimer>

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
                 QString sensorBordar, int dataValue, uint8_t dataNumber);
    void setSensorsSettings(int sensorNumber, QString sensorBordar, QString batteryLevel);
    void setRoutersSettings(int routerNumber, QString batteryLevel);
    void setSensorFrequency(int sensorNumber, int sampleRate, QString bandpassFilter);
    void addSensorInfoToCSV(Sensor newSensor);
    Q_INVOKABLE int getSensorXmin();
    Q_INVOKABLE int getSensorXmax();
    Q_INVOKABLE int getSensorYmin();
    Q_INVOKABLE int getSensorYmax();
    Q_INVOKABLE int getSensorZmin();
    Q_INVOKABLE int getSensorZmax();
    Q_INVOKABLE void zoomPlus(QString bordar);
    Q_INVOKABLE void zoomMines(QString bordar);
    Q_INVOKABLE void scrollDataPlus(QString bordar);
    Q_INVOKABLE void scrollDataMines(QString bordar);
    Q_INVOKABLE void scrollTimePlus(QString bordar);
    Q_INVOKABLE void scrollTimeMines(QString bordar);
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
    float zoomScaleX = 1;
    float zoomScaleY = 1;
    float zoomScaleZ = 1;
    int scrollXData = 0;
    int scrollXTime = 0;
    int scrollYData = 0;
    int scrollYTime = 0;
    int scrollZData = 0;
    int scrollZTime = 0;
    int changeScrollData = 1;
    int changeScrollTime = 1000;
    QTimer *timer;
    double getDateTimeToMSec();
    QList<int> sensorsListCSVCounter;
public slots:
    void timerSlot();
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
