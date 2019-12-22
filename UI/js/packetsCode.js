var packetsCode = {
	packetType: 'packetType',
	selectChartsType : 'selectChartsType',
	eewConfigType: 'eewConfigType',
	runTestType: 'runTestType',
	stopTestType: 'stopTestType',
	releaseAlarmType: 'releaseAlarmType',
	stopAlarmType: 'stopAlarmType',
	receiveChartDataType: 'receiveChartDataType',
	chartsDataType: 'chartsDataType',
    connectionStateType: 'connectionStateType',
    connectionState: 'connectionState',
    newSensorPacket: 'newSensorPacket',
    Router_second: "SEC",
    Router_minute: "MIN",
    Router_milisec: "Msc",
	connected: "connected",
    disconnected: "disconnected",
    sensorsInfo: 'sensorsInfoType',
    antenSignal: 'antenSignal',
	Router_Battery_Level: "RBL",
	Sensor_Battery_Level: "SBL",
	sensorsInfoType: "sensorsInfoType",
	onRoof: "onRoof",
    onGround: "onGround",
    fft_chart_second: "fft_chart_second",
    chart1FFT: "chart1FFT",
    chart2FFT: "chart2FFT",
    chart3FFT: "chart3FFT",
    colibrateType: "colibrateType",
    chooseFile: "chooseFile",
    earthquakeType: "earthquakeType"
} 

// connection state
// {'type': 'connectionStateType', 'connectionState':'True'  }
//runTestType
// {'type': 'runTestType'}
//stopTestType
// {'type': 'stopTestType'}
//rleaseAlarmType
// {'type': 'rleaseAlarmType'}
//stopAlarmType
// {'type': 'stopAlarmType'}
//selectChartsType
//{'type': 'selectChartsType', 'charts' : [{ R: router, S: sensor, B: chart2Bordar },] }
//receiveChartDataType
//{'type': 'receiveChartDataType', 
// 'charts' : { 
//	'chart1': {'R': 1, 'S': 2, 'data': [{'time': '212123', value: 13213}]},
//  'chart2': {'data': []},     // empty 
//  'chart3': {'data': []} } } // empty 

module.exports = packetsCode;