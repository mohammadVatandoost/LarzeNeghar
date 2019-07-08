﻿import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtCharts 2.0
import QtQuick 2.6
import QtQuick.Window 2.2
import SensorModel 1.0
import QtQuick.Controls 2.4


Item {
    ScrollView
    {
        id: scrollView
        anchors.fill: parent
        Component.onCompleted: {

        }
        property int count: 0
        property var lineSeriesList: []
        property int chartsWidth: 1100
        property int checkBowContainerWidth: 400
        property int chartControlWidth: 200
        ColumnLayout{
            property int zoomScaleX: 1
            anchors.fill: parent
            id: bordarX
            spacing: 5
            RowLayout {
                spacing: 5
                ChartView {
                    title: "X"
                    width: scrollView.chartsWidth
                    height: 500
                    antialiasing: true
                    id: chartX
                    DateTimeAxis {
                        id: axisXTime
                        Component.onCompleted: {
                           console.log("set axisXTime ")
                           BackEnd.setAxisXTime(axisXTime);
                        }
                        format: "hh:mm:ss:zzz"
                        tickCount: 6
                    }
                    ValueAxis {
                        id: axisXData
                        min: 0
                        max: 1
                    }
                }
                GridLayout {
                    columns: 3
                    rowSpacing: 3
                    columnSpacing: 3
                    id: xGridLayout
                    width: scrollView.checkBowContainerWidth
                    property int counter: 0
                    Repeater {
                        model: SensorModel {
                            list: SensorsList
                        }
                        Loader {
                            property string routerNumber: model.routerNumber
                            property string sensorNumber: model.sensorNumber
                            property string bordar: model.bordar
                            property int counterLoader: xGridLayout.counter+3
                            sourceComponent: if(model.bordar==="x") { checkComponent }
                        }

                    }
                }
                ColumnLayout{
                    width: scrollView.chartControlWidth
                    spacing: 5
                    Button {
                        text: "zoom+"
                        onClicked: {
                           SensorsList.zoomPlus("x");
                        }
                    }
                    Button {
                        text: "zoom-"
                        onClicked: {
                           SensorsList.zoomMines("x");
                        }
                    }
                    Button {
                        text: "scrollY+"
                        onClicked: {
                           SensorsList.scrollDataPlus("x");
                        }
                    }
                    Button {
                        text: "scrollY-"
                        onClicked: {
                           SensorsList.scrollDataMines("x");
                        }
                    }
                    Button {
                        text: "scrollX+"
                        onClicked: {
                           SensorsList.scrollTimePlus("x");
                        }
                    }
                    Button {
                        text: "scrollX-"
                        onClicked: {
                           SensorsList.scrollTimeMines("x");
                        }
                    }
                }
            }
            RowLayout {
                spacing: 5
                ChartView {
                    title: "Y"
                    width: scrollView.chartsWidth
                    height: 500
                    antialiasing: true
                    id: chartY
                    DateTimeAxis {
                        id: axisYTime
                        Component.onCompleted: {
                           console.log("set axisXTime ")
                           BackEnd.setAxisYTime(axisYTime);
                        }
                        format: "hh:mm:ss:zzz"
                        tickCount: 6
                    }
                    ValueAxis {
                        id: axisYData
                        min: 0
                        max: 1
                    }
                }
                GridLayout {
                    columns: 3
                    rowSpacing: 3
                    columnSpacing: 3
                    width: scrollView.checkBowContainerWidth
                    Repeater {
                        model: SensorModel {
                            list: SensorsList
                        }
                        Loader {
                            property string routerNumber: model.routerNumber
                            property string sensorNumber: model.sensorNumber
                            property string bordar: model.bordar
                            sourceComponent: if(model.bordar==="y") { checkComponent }
                        }

                    }
                }
                ColumnLayout{
                    width: scrollView.chartControlWidth
                    spacing: 5
                    Button {
                        text: "zoom+"
                        onClicked: {
                           SensorsList.zoomPlus("y");
                        }
                    }
                    Button {
                        text: "zoom-"
                        onClicked: {
                           SensorsList.zoomMines("y");
                        }
                    }
                    Button {
                        text: "scrollY+"
                        onClicked: {
                           SensorsList.scrollDataPlus("y");
                        }
                    }
                    Button {
                        text: "scrollY-"
                        onClicked: {
                           SensorsList.scrollDataMines("y");
                        }
                    }
                    Button {
                        text: "scrollX+"
                        onClicked: {
                           SensorsList.scrollTimePlus("y");
                        }
                    }
                    Button {
                        text: "scrollX-"
                        onClicked: {
                           SensorsList.scrollTimeMines("y");
                        }
                    }
                }
            }
            RowLayout {
                spacing: 5
                ChartView {
                    title: "Z"
                    width: scrollView.chartsWidth
                    height: 500
                    antialiasing: true
                    id: chartZ
                    DateTimeAxis {
                        id: axisZTime
                        Component.onCompleted: {
                           console.log("set axisXTime ")
                           BackEnd.setAxisZTime(axisZTime);
                        }
                        format: "hh:mm:ss:zzz"
                        tickCount: 6
                    }
                    ValueAxis {
                        id: axisZData
                        min: 0
                        max: 1
                    }

                }
                GridLayout {
                    columns: 3
                    rowSpacing: 3
                    columnSpacing: 3
                    width: scrollView.checkBowContainerWidth
                    Repeater {
                        model: SensorModel {
                            list: SensorsList
                        }
                        Loader {
                            property string routerNumber: model.routerNumber
                            property string sensorNumber: model.sensorNumber
                            property string bordar: model.bordar
                            sourceComponent: if(model.bordar==="z") { checkComponent }
                        }

                    }
                }
                ColumnLayout{
                    width: scrollView.chartControlWidth
                    spacing: 5
                    Button {
                        text: "zoom+"
                        onClicked: {
                           SensorsList.zoomPlus("z");
                        }
                    }
                    Button {
                        text: "zoom-"
                        onClicked: {
                           SensorsList.zoomMines("z");
                        }
                    }
                    Button {
                        text: "scrollY+"
                        onClicked: {
                           SensorsList.scrollDataPlus("z");
                        }
                    }
                    Button {
                        text: "scrollY-"
                        onClicked: {
                           SensorsList.scrollDataMines("z");
                        }
                    }
                    Button {
                        text: "scrollX+"
                        onClicked: {
                           SensorsList.scrollTimePlus("z");
                        }
                    }
                    Button {
                        text: "scrollX-"
                        onClicked: {
                           SensorsList.scrollTimeMines("z");
                        }
                    }
                }
            }
        }
        Component {
            id: checkComponent
            CheckBox {
                text: routerNumber+" , "+sensorNumber+" , "+bordar
                onCheckStateChanged: scrollView.removeOrAddSeries(routerNumber, sensorNumber, bordar, checked)
                Component.onCompleted: {
                }
            }
        }
        function plotDataX(routerNumber, sensorNumber, bordar, checked) {
            if(bordar === "x" && checked) {
                var line = chartX.createSeries(ChartView.SeriesTypeLine, routerNumber+" , "+sensorNumber+" , "+bordar,
                                               axisXTime, axisXData);
                line.useOpenGL = true;

                lineSeriesList.push(line)
            }
        }
        function plotDataY(routerNumber, sensorNumber, bordar, checked) {
            if(bordar === "y" && checked) {
                var line = chartY.createSeries(ChartView.SeriesTypeLine,routerNumber+" , "+sensorNumber+" , "+bordar,
                                               axisYTime, axisYData);
                line.useOpenGL = true;

                lineSeriesList.push(line)
            }
        }
        function plotDataZ(routerNumber, sensorNumber, bordar, checked) {
            if(bordar === "z" && checked) {
                var line = chartZ.createSeries(ChartView.SeriesTypeLine,routerNumber+" , "+sensorNumber+" , "+bordar,
                                               axisZTime, axisZData);
                line.useOpenGL = true;
                lineSeriesList.push(line)
            }
       }
        function removeOrAddSeries(routerNumber, sensorNumber, bordar, checked) {
            if(checked) {
               plotDataX(routerNumber, sensorNumber, bordar, checked)
               plotDataY(routerNumber, sensorNumber, bordar, checked)
               plotDataZ(routerNumber, sensorNumber, bordar, checked)
            } else {
                console.log("go to find :"+routerNumber+" , "+sensorNumber+" , "+bordar);
                for(var i=0;i<scrollView.lineSeriesList.length;i++) {
                    if(scrollView.lineSeriesList[i].name === routerNumber+" , "+sensorNumber+" , "+bordar) {
                        if(bordar === "x") {chartX.removeSeries(scrollView.lineSeriesList[i]);console.log("x removed Line series :"+scrollView.lineSeriesList[i].name);}
                        if(bordar === "y") {chartY.removeSeries(scrollView.lineSeriesList[i]);console.log("y removed Line series :"+scrollView.lineSeriesList[i].name);}
                        if(bordar === "z") {chartZ.removeSeries(scrollView.lineSeriesList[i]);console.log("z removed Line series :"+scrollView.lineSeriesList[i].name);}
                         scrollView.lineSeriesList = removeFromArray(scrollView.lineSeriesList,i)
                        for(var j=0;j<scrollView.lineSeriesList.length;j++) {
                           console.log(scrollView.lineSeriesList[j].name);

                        }
                    }
                }
            }
        }
        function removeFromArray(temp,index) {
           if(index+1 === temp.length ) {
               temp.pop();
           } else if(index < temp.length) {
               for(var i=index;i<temp.length-1;i++) {
                   temp[i] = temp[i+1]
               }
               temp.pop()
           }
           return temp
       }

        Timer {
            id: refreshTimer
            interval: 100//30 // 60 Hz
            running: true
            repeat: true
            onTriggered: {
                for(var i=0;i<scrollView.lineSeriesList.length;i++) {
                   BackEnd.updateChart(scrollView.lineSeriesList[i]);
                }
                axisXData.min = SensorsList.getSensorXmin();
                axisXData.max = SensorsList.getSensorXmax();

                axisYData.min = SensorsList.getSensorYmin();
                axisYData.max = SensorsList.getSensorYmax();

                axisZData.min = SensorsList.getSensorZmin();
                axisZData.max = SensorsList.getSensorZmax();
            }
        }

    }
}
