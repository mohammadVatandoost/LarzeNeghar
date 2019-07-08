#ifndef PACKETSDEFINE_H
#define PACKETSDEFINE_H

#define Sensor_Unit_Number "SUN"
#define Sensor_Type "SST"
#define Sensor_Battery_Level "SBL"
#define Sensor_Sample_Rate "SSR"
#define Sensor_Bandpass_Filter "SBF"
#define Sensor_Bandpass_Filter_Low "SBFL"
#define Sensor_Bandpass_Filter_High "SBFH"
#define Sensor_Story "SS"
#define Time_First_Sensor_Data "TFD"
#define Router_Packet_Lenth         "RPL"
#define Sensor_Data_Num             "SDN"
#define Sensor_Num                  "SN"
#define Router_Unit_Number "RUN"
#define Router_second "SEC"
#define Router_minute "MIN"
#define Router_milisec "Msc"
#define Router_Battery_Level "RBL"

#define Sensor_Data_X "SDX"
#define Sensor_Data_Y "SDY"
#define Sensor_Data_Z "SDZ"

#define Sensors_settings "ST"
#define Routers_Settings "RT"

#define Data_Check_SUM "DCS"

#define Time_Minute "TM"
#define Time_Second "TS"
#define Time_Mili_Second "TMS"


#define Sensor_Data "DT"
#define Sensors_Frequency_Parameters "SFP"

#define ACK "ACK"
#define NACk "NACk"

#define START_BYTE0 0xA5
#define START_BYTE1 0x5A

#define STOP_BYTE0   0x61
#define STOP_BYTE1   0x62

#define packetType "packetType"
#define	selectChartsType  "selectChartsType"
#define	eewConfigType "eewConfigType"
#define	runTestType "runTestType"
#define	stopTestType "stopTestType"
#define	releaseAlarmType "releaseAlarmType"
#define	stopAlarmType "stopAlarmType"
#define	receiveChartDataType "receiveChartDataType"
#define	chartsDataType "chartsDataType"
#define connectionStateType "connectionStateType"
#define connectionState "connectionState"
#define connected "connected"
#define disconnected "disconnected"
#define newSensorPacket "newSensorPacket"
#define sensorsInfo "sensorsInfoType"
#define antenSignal "antenSignal"
#define sensors_Info "sensorsInfoArray"
#define sensorsInfoType "sensorsInfoType"
#endif // PACKETSDEFINE_H
