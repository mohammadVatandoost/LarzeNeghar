#include "sensordata.h"

SensorData::SensorData()
{

}

SensorData::SensorData(int min, int sec, int miliSec, double v)
{
   second = sec; miliSecond = miliSec ; value = v; minute = min;
}
