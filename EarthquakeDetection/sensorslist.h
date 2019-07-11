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
#include <QTcpSocket>
#include <packetsdefine.h>
#include <QJsonObject>
#include "jsonfileapi.h"

const int MaximumNumberOfFiles  =  100; //if Number of csv files is more than MaximumNumberOfFiles we have to remove erlieast files
const int EachFilePeriodinMS=1000*60*30; // after this MiliSeconds we will close the csv file and store the rest in another file
class SensorsList : public QObject
{
    Q_OBJECT
public:
    explicit SensorsList(QObject *parent = nullptr);
    bool setSensorItem(int index,Sensor &sensor);
    QVector<Sensor> items();
    QVector<Sensor> sensorItems;
    enum channelListUpdate{
        Description, SampleRate, LBandPass,HBandPass,Story
    };

    void addSensor(Sensor newSensor);
    void updateSensorInfo(QJsonObject temp);
    void addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
                 QString sensorBordar, float dataValue, uint8_t dataNumber);
    void setSensorsSettings(int sensorNumber, QString sensorBordar, QString batteryLevel,int routerBatteryLevel);
    void setRoutersSettings(int routerNumber, QString batteryLevel);
    void setSensorFrequency(int sensorNumber, int sampleRate, QString bandpassFilter);
    void addSensorInfoToCSV(Sensor newSensor);
    void runTest();
    void stopTest();
    int getSensorLoss(int router_num, int sensor_num, int sensorLossTemp);
    Q_INVOKABLE float getSensorXmin();
    Q_INVOKABLE float getSensorXmax();
    Q_INVOKABLE float getSensorYmin();
    Q_INVOKABLE float getSensorYmax();
    Q_INVOKABLE float getSensorZmin();
    Q_INVOKABLE float getSensorZmax();
    Q_INVOKABLE void zoomPlus(QString bordar);
    Q_INVOKABLE void zoom(QString bordar , int value);
    Q_INVOKABLE void zoomMines(QString bordar);
    Q_INVOKABLE void scrollDataPlus(QString bordar);
    Q_INVOKABLE void scrollData(QString bordar , int value);
    Q_INVOKABLE void scrollDataMines(QString bordar);
    Q_INVOKABLE void scrollTimePlus(QString bordar);
    Q_INVOKABLE void scrollTime(QString bordar , int value);
    Q_INVOKABLE void scrollTimeMines(QString bordar);
    Q_INVOKABLE int getSize();
    Q_INVOKABLE int getRouterNumberAt(int i);
    Q_INVOKABLE int getSensorNumberAt(int i);
    double getLastDataTime(QString bordar);

    int prevZoomX=0;
    int prevZoomY=0;
    int prevZoomZ=0;

    int prevScrollDataX=0;
    int prevScrollDataY=0;
    int prevScrollDataZ=0;

    int prevScrollTimeX=0;
    int prevScrollTimeY=0;
    int prevScrollTimeZ=0;

    float sensorXmin = -2;
    float sensorXmax = 2;
    double XmaxTime = QDateTime::fromString("Monday, 23 April 01 22:51:41","dddd, d MMMM yy hh:mm:ss").toMSecsSinceEpoch();
    float sensorYmin = -2;
    float sensorYmax = 2;
    double YmaxTime = XmaxTime;
    float sensorZmin = -2;
    float sensorZmax = 2;
    double ZmaxTime = XmaxTime;
    int channelNumCounter = 1 ;
    void changeMinMax(float minValue, float maxValue, QString bordar);
    float zoomScaleX = 1;
    float zoomScaleY = 1;
    float zoomScaleZ = 1;
    float scrollXData = 0;
    int scrollXTime = 0;
    float scrollYData = 0;
    int scrollYTime = 0;
    float scrollZData = 0;
    int scrollZTime = 0;
    float changeScrollData = 0.25;
    int changeScrollTime = 1000;
    QTimer *timer;
    double getDateTimeToMSec(int minute);
    QList<int> sensorsListCSVCounter;
    int pastHour;

    //this three are used in model.cpp slot (myUpdate) for updating the rows
    int sensorNumberForUpdate,batteryLevelForUpdate,RouterbatteryLevelForUpdate;

    //this three are used in model.cpp slot (myApplyUpdate) for updating the rows
    channelListUpdate globalStateforApplyUpdate;
    QString globalStringForApplyUpdate;
    int sensorNumberForApplyUpdate;

    QString csvAddres;
    /*
     * index : which sensor has to change
     * state: which item should update
     * data: what is new string after change that we have to show in chart
     */
    void setChannelListApply(int index,channelListUpdate state , QString data);
    QTcpSocket *pSocket;
    void setTCPSocket(QTcpSocket *temp);
    void sendNewSensor(Sensor temp);
    bool checkIsNewSensor(int routerNumberTemp, int sensorNumberTemp);
    JSONFileAPI jsonFileAPI;
    QJsonArray qJsonArray;
public slots:
    void timerSlot();
    void changeCSVFilesName();
signals:
   void preItemAppended();
   void postItemAppended();

   void preItemRemoved(int index);
   void postItemRemoved();

   void callBatteryUpdate();
   void callApplyUpdate();

public slots:
    void appendItem(); //Sensor sensorItem
    void removeItem(int index);
private:

};

#endif // SENSORSLIST_H
