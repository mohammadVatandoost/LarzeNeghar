#ifndef JSONSTORING_H
#define JSONSTORING_H

#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

#define GeneralDataFile "GenralData.txt"




using namespace std;

class JsonStoring
{
public:
    JsonStoring();
    void storeFileDirectory(QString FileDirectory);
    QString getFileDirectory();
    QString jsonToString(QJsonObject jsonObject);
    void storeToFile(QString data, QString fileName);
    QString getFileString(QString fileName);
};

#endif // JSONSTORING_H
