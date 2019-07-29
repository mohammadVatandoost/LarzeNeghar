const sqlite3 = require('sqlite3').verbose();
// dataBase
var db = new sqlite3.Database('./dataBase.db', (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the Sensors database.');
    // create sensors data base
    db.run('CREATE TABLE IF NOT EXISTS Sensors (id INTEGER PRIMARY KEY AUTOINCREMENT, router_number INTEGER NOT NULL, sensor_number INTEGER NOT NULL, discreption TEXT DEFAULT "", low_pass TEXT NOT NULL, high_pass TEXT NOT NULL, saving_local INTEGER DEFAULT 0, saving_web INTEGER DEFAULT 0, onRoof INTEGER DEFAULT 0, onGround INTEGER DEFAULT 0  )');
    // create sensors data
    // db.run('CREATE TABLE IF NOT EXISTS SensorsData (id INTEGER PRIMARY KEY AUTOINCREMENT, router_number INTEGER NOT NULL, sensor_number INTEGER NOT NULL, discreption TEXT, saving_local INTEGER DEFAULT 0, saving_web INTEGER DEFAULT 0 )');
    // create Earthquake table
    db.run('CREATE TABLE IF NOT EXISTS Earthquakes (id INTEGER PRIMARY KEY AUTOINCREMENT, year TEXT, month TEXT, day TEXT, hour TEXT, minute TEXT, second TEXT, estimated_magnitude TEXT, PGA_L1 TEXT, PGA_L2 TEXT, PGA_V TEXT, PBA_L1 TEXT, PBA_L2 TEXT, PBA_V TEXT, discreption TEXT )');
    //create Table for EEW configuration
    db.run('CREATE TABLE IF NOT EXISTS EEWConfig (id INTEGER PRIMARY KEY AUTOINCREMENT, accTreshold TEXT, highPass TEXT, lowPass TEXT, longPoint TEXT, shortPoint TEXT, staLtaTreshold TEXT, winLength TEXT, a1 TEXT, a2 TEXT, a3 TEXT, a4 TEXT )');

});

module.exports.dataBase = db;

// just use first row of config
module.exports.findEEWConfig = function () {
    console.log("findFirstConfig");
    var sql = 'SELECT * FROM EEWConfig WHERE id=1';
    return new Promise((resolve, reject) => {
        db.get(sql, [], (err, row) => {
          if (err) {
             reject(err); // optional: you might choose to swallow errors.
             // return console.log(err.message);
          } else {
            resolve(row);
          }
        });
    });
};
// insert config just for one time
module.exports.insertConfig = function (accTreshold, highPass, lowPass, longPoint, shortPoint, staLtaTreshold, winLength, a1, a2, a3, a4) {
    console.log("insertNewSensor");
    // insert one row into the langs table
    db.run(`INSERT INTO EEWConfig(accTreshold, highPass, lowPass, longPoint, shortPoint, staLtaTreshold, winLength, a1, a2, a3, a4) VALUES(`+accTreshold+`,`+highPass+`,`+lowPass+`,`+longPoint+`,`+shortPoint+`,`+staLtaTreshold+`,`+winLength+`,`+a1+`,`+a2+`,`+a3+`,`+a4+` )`, [], function(err) {
        if (err) {
            return console.log(err.message);
        }
        // get the last insert id
        console.log(`A row has been inserted with rowid ${this.lastID}`);
    });
};

module.exports.updateEEWConfig = function (accTreshold, highPass, lowPass, longPoint, shortPoint, staLtaTreshold, winLength, a1, a2, a3, a4) {
    console.log("findFirstConfig");
    var sql = `UPDATE EEWConfig SET accTreshold = "`+accTreshold+`", highPass = "`+highPass+`", lowPass = "`+lowPass+`", longPoint = "`+longPoint+`", shortPoint = "`+shortPoint+`", staLtaTreshold = "`+staLtaTreshold+`", winLength = "`+winLength+`", a1 = "`+a1+`", a2 ="`+a2+`", a3="`+a3+`", a4="`+a4+`" WHERE id = 1` ;
    console.log(sql);
    db.get(sql, [], (err, row) => {
        if (err) {
            return console.log(err.message);
        }
        console.log('EEWConfig updated');
    });
};

module.exports.insertNewSensor = function (routerNumber, sensorNumber, low_pass, high_pass ) {
    console.log("insertNewSensor");
    // insert one row into the langs table
    db.run(`INSERT INTO Sensors(router_number, sensor_number, low_pass, high_pass) VALUES(`+routerNumber+`,`+sensorNumber+`,`+low_pass+`,`+high_pass+` )`, [], function(err) {
        if (err) {
            return console.log(err.message);
        }
        // get the last insert id
        console.log(`A row has been inserted with rowid ${this.lastID}`);
    });
};

module.exports.checkNewSensor = function (sensorData ) {
    console.log("checkNewSensor");
    var sql = 'SELECT * FROM Sensors WHERE router_number='+sensorData.router_number+` and `+`sensor_number=`+sensorData.sensor_number;
    return new Promise((resolve, reject) => {
        db.get(sql, [], (err, row) => {
          if (err) {
             reject(err); // optional: you might choose to swallow errors.
             // return console.log(err.message);
          } else {
            var response = {row: row, sensorData: sensorData}
            resolve(response);
          }
        });
    });
};

module.exports.findSensorWithDes = function (des, bordar ) {
    console.log("findSensorWithDes");
    var sql = 'SELECT * FROM Sensors WHERE discreption="'+des+'"';
    return new Promise((resolve, reject) => {
        db.get(sql, [], (err, row) => {
          if (err) {
             reject(err); // optional: you might choose to swallow errors.
             // return console.log(err.message);
          } else {
            var response = {sensorData: row, Bordar: bordar}
            resolve(response);
          }
        });
    });
};

module.exports.updateSensorInfo = function (routerNumber, sensorNumber, discreption, low_pass, high_pass, saving_local, saving_web, onRoof, onGround) {
   console.log('updateSensorInfo');
   var sql = `UPDATE Sensors SET discreption = "`+discreption+`", low_pass = "`+low_pass+`", high_pass = "`+high_pass+`", saving_local = `+saving_local+`, onRoof = `+onRoof+`, onGround = `+onGround+`, saving_web = `+saving_web+` WHERE router_number = `+routerNumber+` and sensor_number = `+sensorNumber ;
   console.log(sql);
   db.get(sql, [], (err, row) => {
        if (err) {
            return console.log(err.message);
        }
        console.log('sensor info updated');
    });
};

module.exports.insertEarthquake = function (year, month, day, hour, minute, second, estimated_magnitude, PGA_L1, PGA_L2, PGA_V, PBA_L1, PBA_L2, PBA_V) {
    console.log("insertEarthquake");
    console.log(`INSERT INTO Earthquakes(year, month, day, hour, minute, second, estimated_magnitude, PGA_L1, PGA_L2, PGA_V, PBA_L1, PBA_L2, PBA_V) VALUES(`+year+`,`+month+`,`+day+`,`+hour+`,`+minute+`,`+second+`,`+estimated_magnitude+`,`+PGA_L1+`,`+PGA_L2+`,`+PGA_V+`,`+PBA_L1+`,`+PBA_L2+`,`+PBA_V+`)`);
    // insert one row into the langs table
    db.run(`INSERT INTO Earthquakes(year, month, day, hour, minute, second, estimated_magnitude, PGA_L1, PGA_L2, PGA_V, PBA_L1, PBA_L2, PBA_V) VALUES(`+year+`,`+month+`,`+day+`,`+hour+`,`+minute+`,`+second+`,`+estimated_magnitude+`,`+PGA_L1+`,`+PGA_L2+`,`+PGA_V+`,`+PBA_L1+`,`+PBA_L2+`,`+PBA_V+`)`, [], function(err) {
        if (err) {
            return console.log(err.message);
        }
        // get the last insert id
        console.log(`new Earthquake added  ${this.lastID}`);
    });
};

module.exports.getEarthquakes = function () {
    console.log("getEarthquakes");
    var sql = 'SELECT * FROM Earthquakes ';
    return new Promise((resolve, reject) => {
        db.get(sql, [], (err, row) => {
          if (err) {
             reject(err); // optional: you might choose to swallow errors.
             // return console.log(err.message);
          } else {
            resolve(row);
          }
        });
    });
};


