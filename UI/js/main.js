const electron = require("electron");
const notifier = require('node-notifier');
var sensorCounter = 0;
var connected = false;
// String
//
// var antenSignal = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

const ipc = electron.ipcRenderer;
var sensorsList = [] ;
var earthquakesData = [] ;
var earthquakeCounter = 0;
// const dsp = require("./dsp.js");
// var fft = new dsp.FFT(8, 1);
//  console.log(fft.forward([1, 2, 3, 4, 5, 6, 7, 8]));
//    var spectrum = fft.spectrum;
//     console.log("FFT :");
//     console.log(spectrum);

// Get the modal
var modal = document.getElementById("myModal");

// Get the button that opens the modal
var btn = document.getElementById("myBtn");

// Get the <span> element that closes the modal
var span = document.getElementsByClassName("close")[0];

// When the user clicks the button, open the modal 
// btn.onclick = function() {
//   modal.style.display = "block";
// }

// When the user clicks on <span> (x), close the modal
// span.onclick = function() {
//   modal.style.display = "none";
// }

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
}


function showMessage(text) {
    $("#modalMessage").text(text);
    modal.style.display = "block";
    setTimeout(function(){ modal.style.display = "none"; }, 2000);
}

// get new sensor data 
ipc.on('new-sensor',function(event,arg) {
	console.log("new-sensor");console.log(arg);
	arg = JSON.parse(arg);
	sensorsList.push(arg);
	if(arg.discreption === "") {
        $( "#chart1Option" ).after( "<option>R"+arg.router_number+",S"+arg.sensor_number+"</option>" );
        $( "#chart2Option" ).after( "<option>R"+arg.router_number+",S"+arg.sensor_number+"</option>" );
        $( "#chart3Option" ).after( "<option>R"+arg.router_number+",S"+arg.sensor_number+"</option>" );
    } else {
        $( "#chart1Option" ).after( "<option>"+arg.discreption+"</option>" );
        $( "#chart2Option" ).after( "<option>"+arg.discreption+"</option>" );
        $( "#chart3Option" ).after( "<option>"+arg.discreption+"</option>" );
        // $( "#groundPosition1Option" ).after( "<option>"+arg.discreption+"</option>" );
        // $( "#roofPositionOption" ).after( "<option>"+arg.discreption+"</option>" );
    }
    $( "#groundPositionOption" ).after( "<option>R"+arg.router_number+",S"+arg.sensor_number+"</option>" );
    $( "#roofPositionOption" ).after( "<option>R"+arg.router_number+",S"+arg.sensor_number+"</option>" );

    if(arg.onRoof === 1) {console.log("onRoof"); $("#roofPosition").val("R"+arg.router_number+",S"+arg.sensor_number);}
    if(arg.onGround === 1) {console.log("onGround");$("#groundPosition").val("R"+arg.router_number+",S"+arg.sensor_number);}

// <td><input id="low_pass`+sensorCounter+`" type="text" value="`+arg.low_pass+`" class="form-control"></td>
//         <td><input id="high_pass`+sensorCounter+`" type="text" value="`+arg.high_pass+`" class="form-control"></td>

	var temp = `
		<tr class="sensorInfo">
         <td class="text-center">`+arg.id+`</td><td class="text-center">`+arg.router_number+`</td><td class="text-center">`+arg.sensor_number+`</td>
         <td><input id="discreption`+sensorCounter+`" type="text" value="`+arg.discreption+`" class="form-control" style="width: 300px;"></td><td id="routerBatteryLevel`+sensorCounter+`">`+arg.routerBatteryLevel+`</td><td id="sensorBatteryLevel`+sensorCounter+`">`+arg.sensorBatteryLevel+`</td>
        <td id="antenSignal`+sensorCounter+`">`+arg.antenSignal+`</td>
        <td style="text-align: center;">
         <div class="form-check">
          <label class="form-check-label" style="padding-bottom: 5px;">`;
          if(parseInt(arg.saving_local) === 0) {
            temp = temp + `<input id="saving_local`+sensorCounter+`" type="checkbox" class="form-check-input"  >` ;
          } else {
            temp = temp + `<input id="saving_local`+sensorCounter+`" type="checkbox" class="form-check-input" checked >` ;
          }
            
        temp = temp + `</label></div></td></tr>` ;
	$("#tableBody").append(temp);
	sensorCounter = parseInt(sensorCounter) + 1 ;
})

// get sensors data 
ipc.on('sensors-list',function(event,arg) {
	// console.log("receive-ports");console.log(arg);
	document.getElementById('ports').innerHTML = document.getElementById('recieveText').innerHTML + arg;
	portName = arg;
});

// connected
ipc.on('connected',function(event,arg) {
    
    if(!connected) {
        console.log("connected");
        notifier.notify({
            title: 'Notification',
            message: 'Device connected'
        });
    }
    connected = true;
});

// disconnected
ipc.on('disconnected',function(event,arg) {
    
    if(connected) {
      console.log("disconnected");
      notifier.notify({
        title: 'Notification',
        message: 'Device disconnected'
      });
    }
    connected = false;
});

// earthquake
ipc.on('earthquake',function(event,arg) {
    var earthquakeData = JSON.parse(arg);
    earthquakesData.push(earthquakeData);
    console.log(earthquakeData);
    var earthquakeId = "earthquake"+earthquakeCounter; earthquakeCounter = earthquakeCounter + 1;
    var temp = `
        <tr id="`+earthquakeId+`Row" >
          <td class="text-center"><div class="form-check-inline"><label class="form-check-label"><input id="`+earthquakeId+`" type="checkbox" class="form-check-input" /></label></div></td>
          <td class="text-center">`+earthquakeData.year+'/'+earthquakeData.month+'/'+earthquakeData.day+', '+earthquakeData.hour+':'+earthquakeData.minute+':'+earthquakeData.second+
         `</td><td class="text-center">`+earthquakeData.estimated_magnitude+`</td><td class="text-center">`+earthquakeData.PGA_L1+`</td>
         <td class="text-center">`+earthquakeData.PGA_L2+`</td><td class="text-center">`+earthquakeData.PGA_V+`</td><td class="text-center">`+earthquakeData.PBA_L1+`</td>
        <td class="text-center">`+earthquakeData.PBA_L2+`</td><td class="text-center">`+earthquakeData.PBA_V+`</td><td class="text-center">`+earthquakeData.discreption+`</td></tr>`;
    $("#tableEarthquake").append(temp);
});

// set EEWConfig
ipc.on('set-EEWConfig',function(event,arg) {
    var eew = JSON.parse(arg);
    // $( "input[name='man']" ).val( "has man in it!" );
    Object.keys(eew).forEach(function(key) {
       $( "input[name='"+key+"']" ).val( eew[key] ); 
       console.log(key+":"+eew[key]+ " , "+"input[name='"+key+"']");
    });
});

// update-battery-signal
ipc.on('update-battery-signal',function(event,arg) {
    // console.log("update-battery-signal");console.log(arg);
    var deviceInfo = JSON.parse(arg);
    var sensorsInfo = deviceInfo.sensorsInfoArray;
    for(var j=0; j<sensorsInfo.length; j++) {
        for (var i = 0; i < sensorsList.length; i++) {
            if ((deviceInfo.router_number === sensorsList[i].router_number) && (sensorsInfo[j].sensor_number === sensorsList[i].sensor_number)) {
                // console.log(i+":"+sensorsInfo[j].Router_Battery_Level+","+sensorsInfo[j].Sensor_Battery_Level+","+sensorsInfo[j].antenSignal)
                $( "#routerBatteryLevel"+i).text(deviceInfo["RBL"]);
                $( "#sensorBatteryLevel"+i).text(sensorsInfo[j]["SBL"]);
                if(parseInt(sensorsInfo[j].antenSignal) <4) {
                    $( "#antenSignal"+i).text("High");
                } else if(parseInt(sensorsInfo[j].antenSignal) <8) {
                    $( "#antenSignal"+i).text("Low");
                } else {
                    $( "#antenSignal"+i).text("Disconnected");
                }
            }
        }
    }
});



// config submit
const configSubmit = document.getElementById('configSubmit');

configSubmit.addEventListener('click', function () {
	var eewConfig = {}
	console.log("configSubmit");
	console.log($('#Config input').val());
	$("#Config input").each(function( index ) {
		eewConfig[$( this ).attr('name')] = $(this).val();
      console.log( index + ": " + $( this ).attr('name') + " , " + $(this).val() );
    });
    console.log('eewConfig');
    console.log(eewConfig);
    ipc.send('eewConfig', eewConfig);
    notifier.notify({
        title: 'Notification',
        message: 'new config submit'
    });
    showMessage("config submited");
});


// run tests
const runTest = document.getElementById('runTest');

runTest.addEventListener('click', function () {
	ipc.send('runTest');

    notifier.notify({
        title: 'Notification',
        message: 'Start Test'
    });
    showMessage("run test");
    
});

// stop tests
const stopTest = document.getElementById('stopTest');

stopTest.addEventListener('click', function () {
	ipc.send('stopTest');
    notifier.notify({
        title: 'Notification',
        message: 'Stop Test'
    });
    showMessage("stop test");
});

// release alarm
const releaseAlarm = document.getElementById('releaseAlarm');

releaseAlarm.addEventListener('click', function () {
	ipc.send('releaseAlarm');
    notifier.notify({
        title: 'Notification',
        message: 'Release Alarm'
    });
    showMessage("release alarm");
});

// stop alarm
const stopAlarm = document.getElementById('stopAlarm');

stopAlarm.addEventListener('click', function () {
	ipc.send('stopAlarm');
    notifier.notify({
        title: 'Notification',
        message: 'Stop Alarm'
    });
    showMessage("stop alarm");
});

// colibrate
const colibrate = document.getElementById('colibrate');

colibrate.addEventListener('click', function () {
    ipc.send('colibrate');
    notifier.notify({
        title: 'Notification',
        message: 'colibrate'
    });
    showMessage("colibrate");
});

// delete earthquake
const deleteSelectedItem = document.getElementById('deleteSelectedItem');

deleteSelectedItem.addEventListener('click', function () {
    var sendData = [];
    for(var i=0; i< earthquakesData.length; i++) {
       if($("#earthquake"+i).is(':checked')) {
         sendData.push(earthquakesData[i]);
         $("#earthquake"+i+'Row').remove();
       }
    }
    // console.log("deleteSelectedItem : "); console.log(sendData);
    ipc.send('deleteErth', JSON.stringify({data: sendData}));
    notifier.notify({
        title: 'Notification',
        message: 'delete selected earthquake'
    });
    showMessage("delete selected item");
});

// save sensor info
const saveSensorInfo = document.getElementById('saveSensorInfo');

saveSensorInfo.addEventListener('click', function () {
	console.log('saveSensorInfo');
	var sensorsInfo = [];
    var groundPosition = $('#groundPosition').val();  var roofPosition = $('#roofPosition').val();
    if( (groundPosition === '-') || (roofPosition === '-') ) {
       // show error
       notifier.notify({
        title: 'Error',
        message: 'You must select on roof sensor and on ground sensor'
       });
    } else {
         var routerOnGround = groundPosition.split(",")[0].replace("R", "");
         var sensorOnGround = groundPosition.split(",")[1].replace("S", "");
         var routerOnRoof = roofPosition.split(",")[0].replace("R", "");
         var sensorOnRoof = roofPosition.split(",")[1].replace("S", "");
        for (var i = 0; i < sensorsList.length; i++) {
            var temp = {};
            temp['router_number'] = sensorsList[i].router_number;
            temp['sensor_number'] = sensorsList[i].sensor_number;
            temp['discreption'] = $("#discreption" + i).val();
            if ($("#saving_local" + i).is(':checked')) {
                temp['saving_local'] = 1;
            } else {
                temp['saving_local'] = 0;
            }
            // temp['low_pass'] = $("#low_pass" + i).val();
            // temp['high_pass'] = $("#high_pass" + i).val();
            temp['low_pass'] = 0;
            temp['high_pass'] = 0;
            if( (parseInt(routerOnGround) === parseInt(sensorsList[i].router_number) ) && ( parseInt(sensorOnGround) === parseInt(sensorsList[i].sensor_number) ) ) {
               temp['onGround'] = 1 ; 
            } else {temp['onGround'] = 0 ;}
            if( (parseInt(routerOnRoof) === parseInt(sensorsList[i].router_number) ) && (parseInt(sensorOnRoof) === parseInt(sensorsList[i].sensor_number) ) ) {
               temp['onRoof'] = 1 ; 
            } else {temp['onRoof'] = 0 ;}
            sensorsInfo.push(temp);
        }
        var sendData = {"sensorsInfo": sensorsInfo, "activateAlgorithm": $("#activateAlgorithm").is(':checked')};

        // if(groundPosition === '-') {
        //
        // } else {
            // var router = groundPosition.split(",")[0].replace("R", "");
            // var sensor = groundPosition.split(",")[1].replace("S", "");
            // groundPosition = {"R": router, "S": sensor};
            // sendData["onGround"] = groundPosition;
        // }
        // if(roofPosition === '-') {
        //
        // } else {
            // var router = roofPosition.split(",")[0].replace("R", "");
            // var sensor = roofPosition.split(",")[1].replace("S", "");
            // roofPosition = {"R": router, "S": sensor};
            // sendData["onRoof"] = roofPosition;
        // }
        console.log(sensorsInfo);
        notifier.notify({
          title: 'Notification',
          message: 'Sensors info saved'
        });
        ipc.send('saveSensorInfo', JSON.stringify(sendData));
        showMessage("save sensor info");
    }
});


