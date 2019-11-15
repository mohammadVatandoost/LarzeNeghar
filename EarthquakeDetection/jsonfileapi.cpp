#include "jsonfileapi.h"

JSONFileAPI::JSONFileAPI()
{

}

bool JSONFileAPI::saveFile()
{
   QFile saveFile(FileName);
   if (!saveFile.open(QIODevice::WriteOnly)) {
       qWarning("Couldn't open save file.");
       return false;
    }

      QJsonDocument saveDoc(qJsonobject);
      saveFile.write(saveDoc.toJson());
      return true;
}

bool JSONFileAPI::loadFile()
{
    QFile loadFile(FileName);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return false;
        }

        QByteArray saveData = loadFile.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        qJsonobject = loadDoc.object();

        return true;
}

void JSONFileAPI::seetSensorsData(QJsonArray qJsonArray)
{
    qJsonobject.insert(SensorsDataArray, qJsonArray);
}

QJsonArray JSONFileAPI::getSensorsData()
{
   if(qJsonobject.contains(SensorsDataArray) && qJsonobject[SensorsDataArray].isArray()) {
       return qJsonobject.value(SensorsDataArray).toArray();
   }
   QJsonArray temp;
   return temp;
}

