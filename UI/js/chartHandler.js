const electron = require("electron");
const ipc = electron.ipcRenderer;
const notifier = require('node-notifier');

var packetsCode = {
  packetType: 'packetType',
  selectChartsType : 'selectChartsType',
  eewConfigType: 'eewConfigType',
  runTestType: 'runTestType',
  stopTestType: 'stopTestType',
  rleaseAlarmType: 'rleaseAlarmType',
  stopAlarmType: 'stopAlarmType',
  receiveChartDataType: 'receiveChartDataType',
  chartsDataType: 'chartsDataType',
    connectionStateType: 'connectionStateType',
    connectionState: 'connectionState',
    newSensorPacket: 'newSensorPacket',
    Router_second: "SEC",
    Router_minute: "MIN",
    Router_milisec: "Msc"
} 
var packetCounter = 0;
var counter = 0;
var numberOfDataShowing = 600;
var times = [];
var dataChart1 = [];
var dataChart2 = [];
var dataChart3 = [];
var seconds = 0;
var miliSeconds = 0 ;
// for fft
var maxFrequency = 200;
var fftChart1 = [];
var fftChart2 = [];
var fftChart3 = [];
var frequency = [];
var frequencyRangeValue = document.getElementById("frequencyRangeValue");
var slider = document.getElementById("frequencyRange");
for(var i=0; i<maxFrequency; i++) {
  frequency.push(i);
}
slider.oninput = function() {
            frequencyRangeValue.innerHTML = this.value+' Hz';
            maxFrequency = this.value;
            while(frequency.length > 0) {frequency.shift();}
            for(var i=0; i<maxFrequency; i++) {
                 frequency.push(i);
             }
}



          // for(var i=0; i < numberOfDataShowing ; i++) {
          //   // if(i%20 === 0) {
          //     times.push("12:"+counter);
          //    // }
          //     dataChart1.push(randomScalingFactor());
          //     dataChart2.push(randomScalingFactor());
          //     dataChart3.push(randomScalingFactor());
          //     counter = parseInt(counter) + 1 ;
          // }
          // dataChart1 = [30.2734375, 30.4931640625, 30.4931640625, 30.0384521484375, 30.584716796875, 30.2734375, 30.5328369140625, 30.2093505859375, 30.6243896484375, 30.4595947265625, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, 30.1788330078125, 30.3436279296875, 30.23681640625, 30.43212890625, 30.157470703125, 30.133056640625, 30.2703857421875, 30.523681640625, 30.3009033203125, 30.157470703125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, 30.303955078125, 30.5389404296875, 30.33447265625, 30.621337890625, 30.682373046875, 30.3375244140625, 30.145263671875, 30.3680419921875, 30.4443359375, 29.8675537109375, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, 30.5084228515625, 30.56640625, 30.31005859375, 30.4443359375, 30.4351806640625, 30.1849365234375, 30.8563232421875, 30.5572509765625, 30.078125, 30.108642578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, 30.0506591796875, 30.1605224609375, 30.87158203125, 30.4412841796875, 30.535888671875, 29.8583984375, 31.146240234375, 30.87158203125, 30.7830810546875, 29.6722412109375, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125, -0.0030517578125];

var color = Chart.helpers.color;
var config = {
       type: 'line',
       data: {
                  labels: times,
                  datasets: [{
                      label: 'Chart1',
                      backgroundColor: color(window.chartColors.red).alpha(0.5).rgbString(),
                      borderColor: window.chartColors.red,
                      fill: false,
                      data: dataChart1,
                  }, {
                      label: 'Chart2',
                      backgroundColor: color(window.chartColors.blue).alpha(0.5).rgbString(),
                      borderColor: window.chartColors.blue,
                      fill: false,
                      data: dataChart2,
                  }, {
                      label: 'Chart3',
                      backgroundColor: color(window.chartColors.green).alpha(0.5).rgbString(),
                      borderColor: window.chartColors.green,
                      fill: false,
                      data: dataChart3,
                  }]
              },
              options: {
                  title: {
                      text: 'Sensors Data'
                  },
                  elements: {
                    point:{
                        radius: 0
                    }
                  },
                  scales: {
                      yAxes: [{
                          scaleLabel: {
                              display: true,
                              labelString: 'value'
                          }
                      }],
                      xAxes: [{
                        ticks: {
                         fontSize: 18
                        }
                      }]
                  },
                  animation: {
                     duration: 0
                  },
                  events: []
              }
          };
// config for FFT chart
var configFFTChart = {
       type: 'line',
       data: {
                  labels: frequency,
                  datasets: [{
                      label: 'Chart1',
                      backgroundColor: color(window.chartColors.red).alpha(0.5).rgbString(),
                      borderColor: window.chartColors.red,
                      fill: false,
                      data: fftChart1,
                  }, {
                      label: 'Chart2',
                      backgroundColor: color(window.chartColors.blue).alpha(0.5).rgbString(),
                      borderColor: window.chartColors.blue,
                      fill: false,
                      data: fftChart2,
                  }, {
                      label: 'Chart3',
                      backgroundColor: color(window.chartColors.green).alpha(0.5).rgbString(),
                      borderColor: window.chartColors.green,
                      fill: false,
                      data: fftChart3,
                  }]
              },
              options: {
                  title: {
                      text: 'FFT'
                  },
                  elements: {
                    point:{
                        radius: 0
                    }
                  },
                  scales: {
                      yAxes: [{
                          scaleLabel: {
                              display: true,
                              labelString: 'value'
                          }
                      }]
                  },
                  animation: {
                     duration: 0
                  },
                  events: []
              }
          };

          window.onload = function() {
              var ctx = document.getElementById('canvas').getContext('2d');
              var FFTctx = document.getElementById('FFTCanvas').getContext('2d');
              window.myLine = new Chart(ctx, config);
              window.FFTLine = new Chart(FFTctx, configFFTChart);
          };

          var myVar = setInterval(myTimer, 250);
           
          function myTimer() {
                if( (dataChart1.length !== 0) || (dataChart2.length !== 0) || (dataChart3.length !== 0) ) {
                 window.myLine.update();  
                 window.FFTLine.update();
                 console.log("window.myLine.update(); ");
                }
              
          }

          function testChartData() {
            var temp = [];
            for(var i=0; i<10; i++ ) {
                 temp.push(randomScalingFactor())
            }
            return temp;
          }

          function testLabelData() {
           var temp = [];
            for(var i=0; i<10; i++ ) {
                 temp.push("12:"+counter);
                 counter = parseInt(counter) + 1 ;
            }
            return temp; 
          }


// get new sensor data 
ipc.on('sensor-data',function(event,arg) {
  // console.log("sensor-data");console.log(arg);
  var dataNumber = 0; var flagPacketCounter = true; 
  arg = JSON.parse(arg);
  // chart 1
  if(hasProperty(arg, 'chart1')) {
     dataNumber = arg.chart1.length;
     for(var i=0; i<dataNumber; i++) {dataChart1.push(arg.chart1[i]);}
    if(packetCounter > numberOfDataShowing ) {
       while(dataChart1.length > numberOfDataShowing) {
        dataChart1.shift();
       }
    } else { if(flagPacketCounter) {packetCounter = packetCounter + arg.chart1.length ; flagPacketCounter = false;} }
    // console.log("chart 1 :");console.log(dataChart1);
  }
  // FFT chart 1
  if(hasProperty(arg, 'chart1FFT')) {
    var fftChartTemp = arg['chart1FFT'];
    var dataNumber2 = fftChartTemp.length;
    while(fftChart1.length > 0) {fftChart1.shift();}
    for(var i=0; i<maxFrequency; i++) {fftChart1.push(fftChartTemp[i]);}
  }
  // chart 2
  if(hasProperty(arg, 'chart2')) {
     dataNumber = arg.chart2.length;
     // dataChart2 = dataChart2.concat(arg.chart2);
     // while(dataChart2.length < numberOfDataShowing) {dataChart2.push(null);}
    for(var i=0; i<arg.chart2.length; i++) {dataChart2.push(arg.chart2[i]);}
    if(packetCounter > numberOfDataShowing ) {
       while(dataChart2.length > numberOfDataShowing) {
        dataChart2.shift();
       }
    } else { if(flagPacketCounter) {packetCounter = packetCounter + arg.chart2.length ; flagPacketCounter = false;} }
    // console.log("chart 2 :");console.log(dataChart2);
  }
  // FFT chart 2
  if(hasProperty(arg, 'chart2FFT')) {
    var fftChartTemp = arg['chart2FFT'];
    var dataNumber2 = fftChartTemp.length;
    while(fftChart2.length > 0) {fftChart2.shift();}
    for(var i=0; i<maxFrequency; i++) {fftChart2.push(fftChartTemp[i]);}
  }
  // chart 3
  if(hasProperty(arg, 'chart3')) {
    dataNumber = arg.chart3.length;
    // while(dataChart3.length < numberOfDataShowing) {dataChart3.push(null);}
    for(var i=0; i<arg.chart3.length; i++) {dataChart3.push(arg.chart3[i]);}
    if(packetCounter > numberOfDataShowing ) {
       while(dataChart3.length > numberOfDataShowing) {
        dataChart3.shift();
       }
    } else { if(flagPacketCounter) {packetCounter = packetCounter + arg.chart3.length ; flagPacketCounter = false;} }
    // console.log("chart 3 :");console.log(dataChart3);
  }
  // FFT chart 3
  if(hasProperty(arg, 'chart3FFT')) {
    // console.log
    var fftChartTemp = arg['chart3FFT'];
    var dataNumber2 = fftChartTemp.length;
    while(fftChart3.length > 0) {fftChart3.shift();}
    for(var i=0; i<maxFrequency; i++) {fftChart3.push(fftChartTemp[i]);}
  }
  // times
  if( (seconds < arg[packetsCode.Router_second]) || (miliSeconds < arg[packetsCode.Router_milisec]) 
    || (Math.abs(seconds - arg[packetsCode.Router_second]) > 50)  ) {
      for(var i =0; i<dataNumber; i++) {
        var timesString = arg[packetsCode.Router_second]+":"+arg[packetsCode.Router_milisec];
        times.push(timesString.substring(0, timesString.length-2) );
      }
      if(packetCounter > numberOfDataShowing ) {
        while(times.length > numberOfDataShowing) {
          times.shift();
        }
       // for(var i =0; i<arg.chart1.length; i++) {
       //    times.shift();
       // }
      }
   }
   // console.log("times :"); console.log(times);
   // update
   // window.myLine.update();
});

// FFT apply
// const chartFFTApply = document.getElementById('FFT');

// chartFFTApply.addEventListener('click', function () {

// });
// Chart apply
const chartApply = document.getElementById('applyChartSelect');

chartApply.addEventListener('click', function () {
  var charts = [];
  var chart1 = $('#firstChart').val();
  var chart1Bordar = $('#firstChartBordar').val();
  var chart2 = $('#secondChart').val();
  var chart2Bordar = $('#secondChartBordar').val();
  var chart3 = $('#thirdChart').val();
  var chart3Bordar = $('#thirdChartBordar').val();
  console.log(chart1 + ","+chart1Bordar+ "/"+chart2 + ","+chart2Bordar+ "/"+chart3 + ","+chart3Bordar);
  var router; 
  var sensor;
  var chartTemp;
  if(chart1 === '-') {
     chartTemp = {};
     // dataChart1 = [];
      while (dataChart1.length > 0) {dataChart1.shift();}
      while (fftChart1.length > 0) {fftChart1.shift();}
      // console.log("dataChart1");console.log(dataChart1);
   charts.push(chartTemp);
  } else {
   if(chart1.includes(",S")) { 
    router = chart1.split(",")[0].replace("R","");
    sensor = chart1.split(",")[1].replace("S",""); 
    chartTemp = { "R": router, "S": sensor, "B": chart1Bordar };
    charts.push(chartTemp);
   } else {
    chartTemp = { "description": chart1, "B": chart1Bordar };
    charts.push(chartTemp);
   }
   if( (dataChart2.length > 0) | (dataChart3.length > 0) ) {
        while( dataChart1.length < times.length ) { dataChart1.push(null); }
   }
  } 
  if(chart2 === '-') {
     chartTemp = {};
      while (fftChart2.length > 0) {fftChart2.shift(); }
      while (dataChart2.length > 0) {dataChart2.shift();}
      // console.log("dataChart2");console.log(dataChart2);
   charts.push(chartTemp);
  } else {
   if(chart2.includes(",S")) { 
    router = chart2.split(",")[0].replace("R","");
    sensor = chart2.split(",")[1].replace("S",""); 
    chartTemp = { "R": router, "S": sensor, "B": chart2Bordar };
    charts.push(chartTemp);
   } else {
    chartTemp = { "description": chart2 , "B": chart2Bordar};
    charts.push(chartTemp);
   }
   if( (dataChart1.length > 0) | (dataChart3.length > 0) ) {
      while( dataChart2.length < times.length ) { dataChart2.push(null); }
   }
  }
  if(chart3 === '-') {
     chartTemp = {};
      while (dataChart3.length > 0) {dataChart3.shift();}
      while (fftChart3.length > 0) {fftChart3.shift(); }
      // console.log("dataChart3");console.log(dataChart3);
   charts.push(chartTemp);
  } else {
    if(chart3.includes(",S")) { 
      router = chart3.split(",")[0].replace("R","");
      sensor = chart3.split(",")[1].replace("S",""); 
      chartTemp = { "R": router, "S": sensor, "B": chart3Bordar };
      charts.push(chartTemp);
    } else {
      chartTemp = { "description": chart3 , "B": chart3Bordar};
      charts.push(chartTemp);
    }  
   if( (dataChart1.length > 0) | (dataChart2.length > 0) ) {
        while( dataChart3.length < times.length ) { dataChart3.push(null); }
   }
  }
  if( (dataChart1.length === 0) && (dataChart2.length === 0) && (dataChart3.length === 0) ) {
      window.myLine.update();  
      window.FFTLine.update();
      console.log("window.myLine.update(); ");
   }
   var sendMessage = JSON.stringify({charts: charts, 'packetType': 'selectChartsType', 'fft_chart_second': $('#FFT').val() }) ;
   console.log(sendMessage);
  // console.log($('#FFT').val());
  ipc.send('chartsApply', sendMessage );
    notifier.notify({
        title: 'Notification',
        message: 'Chart Apply'
    });
});

function hasProperty(object, key) {
    return object ? hasOwnProperty.call(object, key) : false;
 }
