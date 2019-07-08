#include <QCoreApplication>
#include <QDebug>
#include "backend.h"
#include "sensorslist.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<< "earthquake detector";
    SensorsList sensorsList;
    BackEnd backEnd;
    backEnd.setSensorsList(&sensorsList);
    return a.exec();
}
