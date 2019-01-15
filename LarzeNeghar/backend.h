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
#define Sensor_Type "SST"
#define Sensor_Battery_Level "SBL"
#define Sensor_Sample_Rate "SSR"
#define Sensor_Bandpass_Filter "SBF"
#define Sensor_Bandpass_Filter_Low "SBFL"
#define Sensor_Bandpass_Filter_High "SBFH"
#define Sensor_Story "SS"
#define Time_First_Sensor_Data "TFD"
#define Router_Packet_Lenth         "RPL"
#define Sensor_Data_Num             "SDN"
#define Sensor_Num                  "SN"
#define Router_Unit_Number "RUN"
#define Router_second "SEC"
#define Router_minute "MIN"
#define Router_milisec "Msc"
#define Router_Battery_Level "RBL"

#define Sensor_Data_X "SDX"
#define Sensor_Data_Y "SDY"
#define Sensor_Data_Z "SDZ"
#define Sensors_settings "ST"

#define Data_Check_SUM "DCS"

#define Time_Minute "TM"
#define Time_Second "TS"
#define Time_Mili_Second "TMS"

#define Routers_Settings "RT"

#define Sensor_Data "DT"
#define Sensors_Frequency_Parameters "SFP"

#define ACK "ACK"
#define NACk "NACk"

#define START_BYTE0 0xA5
#define START_BYTE1 0x5A

#define STOP_BYTE0   0x80
#define STOP_BYTE1   0x90

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
    QTimer *timer, *packetTimer;
    uint8_t timerCounter = 0 ;
    uint8_t timerCounter2 = 0 ;
    uint8_t packetTimerCounter = 11 ;
    uint8_t jsonPacketTimerCounter = 11 ;
    bool connectState = false;
    void decodeJSON(QString message);
    void sendSerial(QString temp);
    void sendInitialize();
    void sendACK();
    void sendNACK();
    void updateTime();
    void getSettings();
    void decodeByteArray();
    void resetPacketSwitch();
    bool checkCheckSum(QString jsonPacket, int checkSum);
    void sendNack();
    QString jsonToString(QJsonObject jsonObject);
    QString minute,second,miliSecond;
    QDateTimeAxis *axisXTime, *axisYTime, *axisZTime ;
    int routerNum, sensorQuantity, routerMinute, routerSecnd, routerMiliSec, checkSum, packetLengh, SensorDataSize;
    int flagStart = 10 ; int byteCounter = 0;
    uint8_t passMinute ;
     QByteArray *bufByte;
     QByteArray dataByte;
     int packetSize;
signals:

public slots:
    void updateChart(QAbstractSeries *chartSeries);
    void setAxisXTime(QDateTimeAxis *axis);
    void setAxisYTime(QDateTimeAxis *axis);
    void setAxisZTime(QDateTimeAxis *axis);
    void appendItem(); //Sensor sensorItem
    void timerSlot();
    void timerPacketTimeOut();
private slots:
    void recieveSerialPort();
private:
    SensorsList *mList;
    QString recivedSerialPort;
    int flagRecieve = 0;
//    void addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
//                 QString sensorBordar, QString sensorDatas);
};

#endif // BACKEND_H
