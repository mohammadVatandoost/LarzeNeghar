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
    db.run('CREATE TABLE IF NOT EXISTS Earthquakes (id INTEGER PRIMARY KEY AUTOINCREMENT, date_time TEXT, estimated_magnitude TEXT, PGA_L1 TEXT, PGA_L2 TEXT, PGA_V TEXT, PBA_L1 TEXT, PBA_L2 TEXT, PBA_V TEXT, discreption TEXT )');
});

module.exports.dataBase = db;

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
          // if (typeof row !== 'undefined') {
          //   console.log("checkNewSensor sensorInfo is in database");
          //   console.log(row);
          //   return row;
          // } else {
          //   console.log('sensor does not exist');
          //   return false;
          // } 
        });
        // const db = new sqlite3.Database(database);
        // const queries = [];
        // db.each(`SELECT rowid as key, * FROM ${table}`, (err, row) => {
        //     if (err) {
        //         reject(err); // optional: you might choose to swallow errors.
        //     } else {
        //         queries.push(row); // accumulate the data
        //     }
        // }, (err, n) => {
        //     if (err) {
        //         reject(err); // optional: again, you might choose to swallow this error.
        //     } else {
        //         resolve(queries); // resolve the promise
        //     }
        // });
    });
    // db.get(sql, [], (err, row) => {
    //     if (err) {
    //         return console.log(err.message);
    //     }
    //     if (typeof row !== 'undefined') {
    //       console.log("checkNewSensor sensorInfo is in database");
    //       console.log(row);
    //       return row;
    //     } else {
    //       console.log('sensor does not exist');
    //       return false;
    //     }
    // });
};

module.exports.updateSensorInfo = function (routerNumber, sensorNumber, discreption, low_pass, high_pass, saving_local, saving_web) {
   console.log('updateSensorInfo');
   var sql = `UPDATE Sensors SET discreption = "`+discreption+`", low_pass = "`+low_pass+`", high_pass = "`+high_pass+`", saving_local = `+saving_local+`, saving_web = `+saving_web+` WHERE router_number = `+routerNumber+` and sensor_number = `+sensorNumber ;
   console.log(sql);
   db.get(sql, [], (err, row) => {
        if (err) {
            return console.log(err.message);
        }
        console.log('sensor info updated');
    });
};

module.exports.insertEarthquake = function (date_time, estimated_magnitude, PGA_L1, PGA_L2, PGA_V, PBA_L1, PBA_L2, PBA_V, discreption) {
    console.log("insertEarthquake");
    // insert one row into the langs table
    db.run(`INSERT INTO Earthquakes(date_time, estimated_magnitude, PGA_L1, PGA_L2, PGA_V, PBA_L1, PBA_L2, PBA_V, discreption) VALUES(`+date_time+`,`+estimated_magnitude+`,`+PGA_L1+`,`+PGA_L2+`,`+PGA_V+`,`+PBA_L1+`,`+PBA_L2+`,`+PBA_V+`,`+discreption+`)`, [], function(err) {
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
    db.get(sql, [], (err, row) => {
        if (err) {
            return console.log(err.message);
        }
        if (typeof row !== 'undefined') {
          console.log(row);
          return row;
        } else {
          console.log('Earthquakes does not exist');
          return false;
        }
    });
};


