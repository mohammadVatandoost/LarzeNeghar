#ifndef SENSORDATA_H
#define SENSORDATA_H


class SensorData
{
public:
    SensorData();
    SensorData(int min, int sec, int miliSec, double v);
    int second, miliSecond, minute;
    double value;
};

#endif // SENSORDATA_H
