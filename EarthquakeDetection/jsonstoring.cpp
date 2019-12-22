#include "jsonstoring.h"

JsonStoring::JsonStoring()
{

}
//struct BoardData {
//    uint8_t humidityIn = 0;
//    uint8_t humidityOut = 0;
//    uint8_t humidityArea = 0 ;
//    uint16_t pumpSpeed = 0 ;
//    uint8_t batteryCharge = 100;
//    bool chargingStatus = false;
//    bool flowErrorStatus = false;
//    bool electricalErrorStatus = false;
//    float presureSenesor = 0;
//    float tempuretureArea = 0;
//    float tempuretureBoard = 0;
//    bool fan1 = false;
//    bool fan2 = false;
//    string time ;
//};
void JsonStoring::storeFileDirectory(QString FileDirectory)
{
     QJsonObject qJsonObject;
     qJsonObject.insert("FileDirectory", FileDirectory);

     QString data = jsonToString(qJsonObject);
     storeToFile(data, GeneralDataFile);
}

QString JsonStoring::getFileDirectory()
{
   QString jsonString = getFileString(GeneralDataFile);
   if(jsonString == "") {
       cout<< "JsonStoring::getFileDirectory() does not exist"<<endl;
       return  "";
   }
   cout<< "getFileDirectory:"<< jsonString.toStdString();
   QJsonDocument qJsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
   QJsonObject qJsonObject = qJsonDocument.object();
   return  qJsonObject.value("FileDirectory").toString();
}

QString JsonStoring::jsonToString(QJsonObject jsonObject)
{
    QJsonDocument doc(jsonObject);
    return  doc.toJson(QJsonDocument::Compact);
}

void JsonStoring::storeToFile(QString data, QString fileName)
{
    QFile file(fileName);
        if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text) )
        {
            QTextStream textStream( &file );
            QStringList stringList;
            textStream << data;
            file.close();
        } else {
            cout<< " file " << fileName.data() << " could not open" << endl;
        }
}

QString JsonStoring::getFileString(QString fileName)
{
    QString temp = "";
    QFile file(fileName);
    if(!file.exists()) {
        return temp;
    }
    if( !file.open(QFile::ReadOnly | QFile::Text) ) {
        cout << fileName.data() << " File not exists "<<endl;
    } else {
        QTextStream in(&file);
        temp = in.readLine();
        file.close();
    }

    return temp;
}
