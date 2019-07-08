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
#include <QTime>
#include <qmath.h>
#include <QTcpSocket>
#include <packetsdefine.h>
#include <algorithm.h>
#include <QStorageInfo>
#include <QDir>
#include <QFileInfo>
#include <QFile>

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
    Q_INVOKABLE void connectSerialPort(QString portName);
    Q_INVOKABLE static QVariant availablePorts() ;
    int serialPortCounterTry =0 ;
    QSerialPort *serial;
    QString come_port;
    QTimer *timer, *packetTimer;
    uint8_t timerCounter = 0 ;
    uint8_t timerCounter2 = 0 ;
    uint8_t packetTimerCounter = 11 ;
    uint8_t jsonPacketTimerCounter = 11 ;
    bool connectState = false;
    bool sendAverage = true;
    void decodeJSON(QString message);
    void sendSerial(QString temp);
    void sendInitialize();
    void sendAlarmOn();
    void sendAlarmOff();
    void sendACK();
    void sendNACK();
    void updateTime();
    void getSettings();
    void decodeByteArray();
    void decodeByteArrayType2();
    void resetPacketSwitch();
    bool checkCheckSum(QString jsonPacket, int checkSum);
    void sendNack();
    bool checkDataSendToChart1();
    bool checkDataSendToChart2();
    bool checkDataSendToChart3();
    void newDecode();
    QJsonArray calculateAverage(QJsonArray dataArray) ;
    void sendChartData(QJsonObject tempQJsonObject, int sensorUID, QJsonArray xBordar, QJsonArray yBordar, QJsonArray zBordar);
//    void sendSensorData
    QString jsonToString(QJsonObject jsonObject);
    QString minute,second,miliSecond;
    int routerNum, sensorQuantity, routerMinute, routerSecnd, routerMiliSec, checkSum, packetLengh, SensorDataSize, routerBattery;
    int updateBatteryCounter=-10; //For updating the chart
    int UID; //First Data in each sensor data  <Start Start UID SBL <...x...> <...y...> <...z...> UID SBL <...x...> <...y...> <...z...> ... Stop Stop>
    const int casualSensorPacketLength=62; //without considering start and stop bits 62=(66-4)
    const int ProSensorPacketLength=92;     //without considering start and stop bits 96=(96-4)
    QString sensorType, sensorBatteryLevel, sensorLossLevel;
    bool chart1 = false; bool chart2 = false; bool chart3 = false;
    int chart1Router, chart1Senesor, chart2Router, chart2Senesor, chart3Router, chart3Senesor;
    QString chart1Bordar, chart2Bordar, chart3Bordar;
    int flagStart = 10 ; int byteCounter = 0;
    uint8_t passMinute ;
    QByteArray *bufByte;
    QByteArray dataByte;
    int packetSize;
    QTcpSocket *pSocket;
    QStorageInfo storage = QStorageInfo::root();


signals:

public slots:
    void appendItem(); //Sensor sensorItem
    void timerSlot();
    void timerPacketTimeOut();
private slots:
    void recieveSerialPort();
    void readTcpData();
private:
    SensorsList *mList;
    QString recivedSerialPort;
    int flagRecieve = 0;
    //    void addData(int min, int sec, int milSec, int routerNumber, int sensorNumber,
    //                 QString sensorBordar, QString sensorDatas);
};

#endif // BACKEND_H
