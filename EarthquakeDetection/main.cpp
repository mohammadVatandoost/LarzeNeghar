#include <QCoreApplication>
#include <QDebug>
#include <thread>
#include "backend.h"
#include "sensorslist.h"



//void getStdIn() {
//    string command;
//    while(std::cin>>command)
//    {
//        backEnd.readStdIn(command);
//    }
//}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<< "earthquake detector";
    SensorsList sensorsList;
    BackEnd backEnd;
    backEnd.setSensorsList(&sensorsList);

//    thread forStdINThread(getStdIn);
//    forStdINThread.detach();

    return a.exec();
}
