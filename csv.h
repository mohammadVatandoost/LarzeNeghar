#ifndef CSV_H
#define CSV_H

#include <QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDebug>
// setprecision example
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include "float.h"
#include <QDir>

QVector<QStringList> readCSV(QString fileLocation, QString splitString);
void appendDataToCSV(QVector<QStringList> dataList, QString fileName);

#endif // CSV_H
