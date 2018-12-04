#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtCore/QObject>
#include "sensorslist.h"
#include "QDebug"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCharts/QAbstractSeries>
#include <QTime>
#include <QtCharts/QDateTimeAxis>
//#define Sensor_Channel_Number "SCN"

#define Sensor_Unit_Number "SUN"
#define Router_Unit_Number "RUN"
#define Sensor_Data_X "SDX"
#define Sensor_Data_Y "SDY"
#define Sensor_Data_Z "SDZ"
#define Sensor_Battery_Level "SBL"
#define Sensor_Sample_Rate "SSR"
#define Sensor_Bandpass_Filter "SBF"
#define Sensor_Story "SS"
#define Sensor_Sensor_Type "SST"
#define Sensors_Data "sensors"
#define Routers_Settings "RouterST"
#define Sensors_settings "SensorST"
#define Sensor_Data "SensorDT"
#define Time_First_Sensor_Data "TFD"
#define Router_second "SEC"
#define Router_minute "MIN"
#define Router_milisec "Msc"
#define Router_Battery_Level "RBL"
#define Data_Check_SUM "DCS"
#define Time_Minute "TM"
#define Time_Second "TS"
#define Time_Mili_Second "TMS"

QT_CHARTS_USE_NAMESPACE

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);
    Q_INVOKABLE void setSensorsList(SensorsList *sensorsList);
    Q_INVOKABLE QVector<Sensor> getSensorsList();
    Q_INVOKABLE QList<int> getSenorsX(QString routerNumber, QString sensorNumber, QString bordar);
    Q_INVOKABLE QList<int> getSenorsY(QString routerNumber, QString sensorNumber, QString bordar);
    Q_INVOKABLE void test();
    Q_INVOKABLE void sendSettings();

    QSerialPort *serial;
    QString come_port;
    QTimer *timer;
    bool connectState = false;
    void decodeJSON(QString message);
    void sendSerial(QString temp);
    void sendInitialize();
    void updateTime();
    QString jsonToString(QJsonObject jsonObject);
    QString minute,second,miliSecond;
    QDateTimeAxis *axisXTime, *axisYTime, *axisZTime ;

signals:

public slots:
    void updateChart(QAbstractSeries *chartSeries);
    void setAxisXTime(QDateTimeAxis *axis);
    void setAxisYTime(QDateTimeAxis *axis);
    void setAxisZTime(QDateTimeAxis *axis);
    void appendItem(); //Sensor sensorItem
    void timerSlot();
private slots:
    void recieveSerialPort();
private:
    SensorsList *mList;
    QString recivedSerialPort;
    int flagRecieve = 0;
    void addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
                 QString sensorBordar, QString sensorDatas);
};

#endif // BACKEND_H
