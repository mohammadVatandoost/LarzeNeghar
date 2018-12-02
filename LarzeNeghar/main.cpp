#include <QGuiApplication>
#include <QQmlApplicationEngine>
// for expose object to qml
#include <QQmlContext>
#include <QApplication>
#include "sensormodel.h"
#include "sensorslist.h"
#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<SensorModel>("SensorModel", 1, 0, "SensorModel");
    qmlRegisterUncreatableType<SensorsList>("SensorsList",1,0,"SensorsList",
                                         QStringLiteral("SensorsList should not be created in QML"));
    SensorsList sensorsList;
    BackEnd backEnd;
    backEnd.setSensorsList(&sensorsList);
//    qmlRegisterType<BackEnd>("BackEnd",1,0,"BackEnd");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("SensorsList"), &sensorsList);
    engine.rootContext()->setContextProperty(QStringLiteral("BackEnd"), &backEnd);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
