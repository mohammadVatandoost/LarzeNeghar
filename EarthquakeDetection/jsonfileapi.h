#ifndef JSONFILEAPI_H
#define JSONFILEAPI_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#define FileName "save.json"
#define SensorsDataArray "SDA"

class JSONFileAPI
{
public:
    JSONFileAPI();
    QJsonObject qJsonobject;
    bool saveFile();
    bool loadFile();
//    bool setSensorOffset(int routerNum, int sensorNum, QString sensorBoradar, int offset);
//    bool newSensor(int routerNum, int sensorNum, QString sensorBoradar);
    void seetSensorsData(QJsonArray qJsonArray);
    QJsonArray getSensorsData();
};

#endif // JSONFILEAPI_H
