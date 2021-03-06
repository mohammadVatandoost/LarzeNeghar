const { app, BrowserWindow } = require('electron')
const electron = require('electron');
const dialog = electron.dialog;
const ipc = electron.ipcMain;
const path = require('path');  
const packetsCode = require('./js/packetsCode');
const db = require('./js/dataBase');
const { exec } = require('child_process');
const { spawn } = require('child_process');
var sensors = [];
var activateAlgorithm = true;
var eewConfigBuffer;
runDBTest(db);


// const NativeImage = require('native-image');

var net = require('net');

var HOST = '127.0.0.1';
var PORT = 6969;

var serverSocket;
var EarthquakeDetection;
var stdDataBuf = "";
// Create a server instance, and chain the listen function to it
// The function passed to net.createServer() becomes the event handler for the 'connection' event
// The sock object the callback function receives UNIQUE for each connection
net.createServer(function(sock) {
  serverSocket = sock;
  serverSocket.write(eewConfigBuffer + "***");
  // We have a connection - a socket object is assigned to the connection automatically
 console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);

  // Add a 'data' event handler to this instance of socket
  sock.on('data', function(data) {
    decodeSocketPacket(data);
    // console.log('DATA ' + sock.remoteAddress + ': ' + data);
    // Write the data back to the socket, the client will receive it as data from the server
    // sock.write('You said "' + data + '"');
  });
  // Add a 'close' event handler to this instance of socket
 sock.on('close', function(data) {
   console.log('CLOSED: ' + sock.remoteAddress +' '+ sock.remotePort);
 });

}).listen(PORT, HOST);

console.log('Server listening on ' + HOST +':'+ PORT);


  let sp;
  // Keep a global reference of the window object, if you don't, the window will
  // be closed automatically when the JavaScript object is garbage collected.
  let win;
  
  function createWindow () {
    // Create the browser window.
    win = new BrowserWindow({ 
       width: 1600, height: 800,
       webPreferences: {
            nodeIntegration: true
        }

       }) //, icon: __dirname+'/icons/raiwan.png'
    win.maximize();
    // and load the index.html of the app.
    win.loadFile('index.html')
  
    // Open the DevTools.
    //win.webContents.openDevTools()

    win.webContents.on('did-finish-load', () => {
      db.findEEWConfig().then((response) => {
         if (typeof response !== 'undefined') {
          console.log("eewConfig");console.log(response);
          var temp = response; temp["packetType"] = "eewConfigType";
          eewConfigBuffer = JSON.stringify(temp);
          if(win.webContents !== null) {
              win.webContents.send('set-EEWConfig', JSON.stringify(response));
          }
         } else {
           var sendmessage= {};
           sendmessage["accTreshold"] = "5"; sendmessage["highPass"] = "0.1"; sendmessage["lowPass"] = "15" ;
           sendmessage["longPoint"] = "100"; sendmessage["shortPoint"] = "10"; sendmessage["staLtaTreshold"] = "5";
           sendmessage["winLength"] = "3"; sendmessage["a1"] = "1"; sendmessage["a2"] = "1.02";
            sendmessage["a3"] = "1";  sendmessage["a4"] = "1.02";sendmessage["packetType"] = "eewConfigType"; 
           eewConfigBuffer = JSON.stringify(sendmessage);
           db.insertConfig(sendmessage["accTreshold"], sendmessage["highPass"], sendmessage["lowPass"],
            sendmessage["longPoint"], sendmessage["shortPoint"], sendmessage["staLtaTreshold"], 
            sendmessage["winLength"], sendmessage["a1"], sendmessage["a2"], sendmessage["a3"], sendmessage["a4"]);
           if(win.webContents !== null) {
              win.webContents.send('set-EEWConfig', JSON.stringify(sendmessage));
          }
         }

        // EarthquakeDetection = spawn('./EarthquakeDetection');
        // EarthquakeDetection.stdout.on('data', (data) => {
        //     // console.log(`-------------stdout : ${data}`);
        //     // decodeStdPacket(data);
        // });
        // EarthquakeDetection.stderr.on('data', (data) => {
        //    // console.error(`stderr: ${data}`);
        // });

        // EarthquakeDetection.on('close', (code) => {
        //      console.log(`child process exited with code ${code}`);
        // });
        //  exec('./EarthquakeDetection', (err, stdout, stderr) => {
        //   console.log("run exe");
        //   if (err) {
        //       console.error(err);
        //       return;
        //   }
        //    console.log(stdout);
        // });

      }).catch((err)=> {console.log(err);});
      db.getEarthquakes().then((response) => {
        console.log("getEarthquakes");console.log(response.length);
        for(var i=0; i< response.length; i++) {
          var temp = response[i];
          if(temp.discreption === null) {temp['discreption']="";}
          win.webContents.send('earthquake', JSON.stringify(temp));
        }
      }).catch((err)=> {console.log(err);});

  });
  
    // Emitted when the window is closed.
    win.on('closed', () => {
      // Dereference the window object, usually you would store windows
      // in an array if your app supports multi windows, this is the time
      // when you should delete the corresponding element.
      //   serverSocket.close();
      // serverSocket.close();
      console.log("window closed");
      // EarthquakeDetection.stdin.pause();
      // EarthquakeDetection.kill();
      win = null;
      db.dataBase.close((err) => {
          if (err) {
              return console.error(err.message);
          }
          console.log('Close the database connection.');
       });
    })

  }
  
  // This method will be called when Electron has finished
  // initialization and is ready to create browser windows.
  // Some APIs can only be used after this event occurs.
  app.on('ready', createWindow)
  
  // Quit when all windows are closed.
  app.on('window-all-closed', () => {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
      app.quit()
    }
  })
  
  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (win === null) {
      createWindow();
    }

  })

// get eewConfig
ipc.on('eewConfig', function(event,arg) {
  var temp = arg ;
  temp[packetsCode.packetType] = packetsCode.eewConfigType ;
  console.log("eewConfig : " + arg);
  db.updateEEWConfig(temp["accTreshold"], temp["highPass"], temp["lowPass"], temp["longPoint"], 
    temp["shortPoint"], temp["staLtaTreshold"], temp["winLength"], temp["a1"], temp["a2"], temp["a3"], temp["a4"]);
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

// get charts
ipc.on('chartsApply', function(event,arg) {
  console.log("chartsApply");console.log(arg);
  var temp = JSON.parse(arg);
  var charts = temp.charts;
  var sendmessage = {'packetType': 'selectChartsType', 'fft_chart_second': temp['fft_chart_second']};
  var chartsTemp = [];
  for(var i=0;i<charts.length; i++) {
    if(hasProperty(charts[i], "description")) {
      // console.log("has description");
      //console.log(charts[i]);console.log(charts[i]["B"]);console.log(charts[i].B);
       db.findSensorWithDes(charts[i].description, charts[i].B).then((response) => {
        console.log(response);
         chartsTemp.push({"R": response.sensorData.router_number+'', "S": response.sensorData.sensor_number+'', "B": response.Bordar+''});
         if(charts.length === chartsTemp.length) {
           sendmessage["charts"] = chartsTemp;
           console.log(JSON.stringify(sendmessage) + "***");
           if(serverSocket !== null) {
              serverSocket.write(JSON.stringify(sendmessage) + "***");
           }
         }
       }).catch((err) => {
            console.log(err.message);
       });
    } else {chartsTemp.push(charts[i]);}
  }
  if(charts.length === chartsTemp.length) {
    sendmessage["charts"] = chartsTemp;
    console.log(JSON.stringify(sendmessage) + "***");
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(sendmessage) + "***");
    }
  }
});
// chooseFolder
ipc.on('chooseFolder', function() {
    console.log('chooseFolder');

    dialog.showOpenDialog(win, {
       properties: ['openFile', 'openDirectory']
    }).then(result => {
       console.log("dir");
       if(!result.canceled) {
         var temp = {};
         temp[packetsCode.packetType] = packetsCode.chooseFile;
         temp["path"] = result.filePaths[0];
         console.log("chooseFile : "+JSON.stringify(temp)+"***");
         if(serverSocket !== null) {
           serverSocket.write(JSON.stringify(temp) + "***");
         }
       }
    }).catch(err => {
       console.log("dir");
       console.log(err)
    });

});

//  run tests
ipc.on('runTest', function(event,arg) {
  var temp = {};
  temp[packetsCode.packetType] = packetsCode.runTestType;
  console.log("runTest : "+JSON.stringify(temp)+"***");
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

// stop tests
ipc.on('stopTest', function(event,arg) {
    var temp = {};
    temp[packetsCode.packetType] = packetsCode.stopTestType;
    console.log("stopTest : "+JSON.stringify(temp)+"***");
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

// release alarm
ipc.on('releaseAlarm', function(event,arg) {
    var temp = {};
    temp[packetsCode.packetType] = packetsCode.releaseAlarmType;
    console.log("releaseAlarm : "+JSON.stringify(temp)+"***");
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

// Earth Data
ipc.on('deleteErth', function(event,arg) {
    var temp = JSON.parse(arg);
    var data = temp.data;
    for(var i=0; i< data.length; i++) {
      db.deleteEarthquake(data[i].id);
    }
});

// stop alarm
ipc.on('stopAlarm', function(event,arg) {
    var temp = {};
    temp[packetsCode.packetType] = packetsCode.stopAlarmType;
    console.log("stopAlarm : "+JSON.stringify(temp)+"***");
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

// stop alarm
ipc.on('colibrate', function(event,arg) {
    var temp = {};
    temp[packetsCode.packetType] = packetsCode.colibrateType;
    console.log("colibrate : "+JSON.stringify(temp)+"***");
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

// save sensor info
ipc.on('saveSensorInfo', function(event,arg) {
  console.log("saveSensorInfo :"+arg);
  var temp = JSON.parse(arg);
  var sensorsInfo = temp.sensorsInfo;
  // var groundPosition = temp["onGround"];   var roofPosition = temp["onRoof"];
    for(var i=0; i<sensorsInfo.length ; i++) {
       db.updateSensorInfo(sensorsInfo[i].router_number, sensorsInfo[i].sensor_number, sensorsInfo[i].discreption,
        sensorsInfo[i].low_pass, sensorsInfo[i].high_pass, sensorsInfo[i].saving_local, 0,
         sensorsInfo[i].onRoof, sensorsInfo[i].onGround);  
    }
    activateAlgorithm = temp["activateAlgorithm"]
    temp[packetsCode.packetType] = packetsCode.sensorsInfo;
    if(serverSocket !== null) {
        serverSocket.write(JSON.stringify(temp) + "***");
    }
});

function decodeStdPacket(data) {

  data = data + ''; //convert to string
  stdDataBuf = stdDataBuf + data;
  // console.log("decodeStdPacket : "+stdDataBuf);
  if(stdDataBuf.includes("***")) {
    // console.log("data.includes(***) :");
    var packets = stdDataBuf.split("***");
    if(packets.length > 0) {
      stdDataBuf = "";
    }
    // console.log("--------------packets.length:"+packets.length);
   for(var i=0; i<packets.length; i++) {  // -1 for split last
     if(IsJsonString(packets[i])) {
      var dataTemp = JSON.parse(packets[i]) ;
      if(dataTemp.packetType === packetsCode.receiveChartDataType) {
          if(win.webContents !== null) {
              win.webContents.send('sensor-data', packets[i]);
          }
      }
     } else {
      if(i >0) {
         stdDataBuf = stdDataBuf + packets[i];
      }
      // console.log(i + ", "+packets.length+",decodeStdPacket is not Json:"+packets[i]);
     }
   }
  } else {
     // console.log("-------------------decodeStdPacket does not contain ****");
  }
}

// socket packet controller
function decodeSocketPacket(data) {
  data = data + ''; //convert to string
  // console.log("decodeSocketPacket : "+data);
  if(data.includes("***")) {
    // console.log("data.includes(***) :");
    var packets = data.split("***");
    // console.log(packets.length);
   for(var i=0; i<packets.length-1; i++) {  // -1 for split last
     if(IsJsonString(packets[i])) {
      var dataTemp = JSON.parse(packets[i]) ;
      // console.log(i+" packet : "+packets[i]);
      // console.log("decodeSocketPacket dataTemp.packetType: "+dataTemp.packetType + " packetsCode.newSensor :"+packetsCode.newSensorPacket);
      if(dataTemp.packetType === packetsCode.connectionStateType) {
        
      } else if(dataTemp.packetType === packetsCode.receiveChartDataType) {
        // console.log("receiveChartDataType");console.log(packets[i]);
          if(win.webContents !== null) {
              win.webContents.send('sensor-data', packets[i]);
          }
      } else if(dataTemp.packetType === packetsCode.newSensorPacket) {
       // delete data.packetType;
       console.log("newSensor packet");
       if(checkSensorNew(dataTemp)) {
           db.checkNewSensor(dataTemp, dataTemp).then((response) => {
               console.log("response");
           console.log(response);
           var row = response.row;
           var sensorData = response.sensorData;
           if (typeof row !== 'undefined') {
               console.log("checkNewSensor sensorInfo is in database");
               console.log(row);
               // var sensorInfoFromDevice = dataTemp;
               // console.log(sensorInfoFromDevice);
               row["sensorBatteryLevel"] = sensorData.sensorBatteryLevel;
               row["routerBatteryLevel"] = sensorData.routerBatteryLevel;
               row["antenSignal"] = sensorData.antenSignal;
               var sensorsInfo = [];
               sensorsInfo.push(row);
               var sendData = {"sensorsInfo": sensorsInfo};
               sendData[packetsCode.packetType] = packetsCode.sensorsInfo;
               sendData["activateAlgorithm"] = activateAlgorithm;
               sensors.push(row);
               console.log("sensorInfo from dataBase : " + JSON.stringify(row));
               var tcpSocketData = JSON.stringify(sendData) + "***";
               console.log(tcpSocketData);
               if(serverSocket !== null) {
                   serverSocket.write(tcpSocketData);
               }
               if(win.webContents !== null) {
                   win.webContents.send('new-sensor', JSON.stringify(row));
               }
           } else {
               console.log('sensor does not exist');
               sensorData["discreption"] = "";
               sensorData[packetsCode.onRoof] = 0;
               sensorData[packetsCode.onGround] = 0;
               sensors.push(sensorData);
               db.insertNewSensor(sensorData.router_number, sensorData.sensor_number, '15', '0.1');
               console.log("newSensorInfo : " + JSON.stringify(sensorData));
               if(win.webContents !== null) {
                   win.webContents.send('new-sensor', JSON.stringify(sensorData));
               }
           }
        }).catch((err) => {
               console.log(err.message);
        });
       }
      } else if(dataTemp.packetType === packetsCode.connectionStateType) {
          console.log("connectionStateType");
          if(dataTemp.connectionState === packetsCode.connected) {
              if(win.webContents !== null) {
                  win.webContents.send('connected');
              }
          } else {
              if(win.webContents !== null) {
                  win.webContents.send('disconnected');
              }
          }
      } else if(dataTemp.packetType === packetsCode.sensorsInfoType) {
          // console.log("sensorsInfoType");
          if(win.webContents !== null) {
              win.webContents.send('update-battery-signal', packets[i]);
          }
      } else if(dataTemp.packetType === packetsCode.earthquakeType) {
          console.log("earthquakeType");console.log(dataTemp);
          db.insertEarthquake(dataTemp.year, dataTemp.month, dataTemp.day, dataTemp.hour, dataTemp.minute,
          dataTemp.second, dataTemp.estimated_magnitude+'', dataTemp.PGA_L1+'',
            dataTemp.PGA_L2+'', dataTemp.PGA_V+'', dataTemp.PBA_L1+'', dataTemp.PBA_L2+'', dataTemp.PBA_V+'');
          dataTemp["discreption"] = "";
          if(win.webContents !== null) {
              win.webContents.send('earthquake', JSON.stringify(dataTemp));
          }
      }
     }
    } 
  }
}

// check sensors is new 
function checkSensorNew(temp) {
    for(var i=0; i<sensors.length; i++) {
        if( (sensors[i].router_number === temp.router_number) && (sensors[i].sensor_number === temp.sensor_number)) {
            return false;
        }
    }
    return true;
}

// run test
function runDBTest(dataBase) {
    // dataBase.insertNewSensor(1,1);
    // dataBase.insertNewSensor(1,2);
    // dataBase.insertNewSensor(1,3);
    // dataBase.insertNewSensor(2,1);
    // dataBase.insertNewSensor(2,2);
    // dataBase.insertNewSensor(2,3);
    // dataBase.checkNewSensor(1,1);
   // console.log("**********runTest*************"); 
   // db.insertEarthquake("11", '123', '123',
   //          '123', '123', '123', '123', '123', "456");
}

function hasProperty(object, key) {
    return object ? hasOwnProperty.call(object, key) : false;
}

 function IsJsonString(str) {
    try {
        JSON.parse(str);
    } catch (e) {
        return false;
    }
    return true;
}
