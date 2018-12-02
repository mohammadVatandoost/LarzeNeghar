import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import SensorModel 1.0
//import BackEnd 1.0

//ColumnLayout {
Item {
        id: monitoringTab
        property int columnNum: 18
        property int baseWidth: 30
        property int md_1: 1
        property int md_2: 2
        property int md_3: 3
        property int md_4: 4
        property int md_5: 5
        property int textFieldWidth: columnNum*2
//        BackEnd {
//            id: backend
//        }

        Layout.fillWidth: true
        Component.onCompleted: {
//            backend.setSensorsList(SensorsList)
//            console.log("SensorsList.sensorData")
//            console.log(BackEnd.getSenorsX(0))
//            console.log(BackEnd.getSenorsY(0))
        }
//        Rectangle
//            {
//                id: background
//                anchors.fill: parent
//                color: "blue"
//            }

//        Rectangle {
//            color: "red"
        ListView {
            implicitWidth: monitoringTab.width
            implicitHeight: 500
            Layout.fillWidth: false
            clip: true
//            highlight: highlightBar
            id: listView
            headerPositioning: ListView.OverlayHeader
            spacing: 10
            model: SensorModel {
                list: SensorsList
            }

            header: RowLayout {
//               width: parent.width
               height: 80
               spacing: 10
               Rectangle {
                   id: header_cn
//                 width:
                   width: baseWidth*md_4
//                 Layout.width: listView.width/columnNum
//                 Layout.minimumWidth:
                 Text {
                   anchors.centerIn: parent
                   text: "Channel Number"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_un
                   anchors.centerIn: parent
                   text: "Router Number"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_sn
                   anchors.centerIn: parent
                   text: "Sensor Number"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_2
                 Text {
                   id: header_bn
                   anchors.centerIn: parent
                   text: "Bordar"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_4
                 Text {
                   id: header_des
                   anchors.centerIn: parent
                   text: "Description"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_bl
                   anchors.centerIn: parent
                   text: "Battery Level"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_sr
                   anchors.centerIn: parent
                   text: "Sample Rate"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_bf
                   anchors.centerIn: parent
                   text: "Bandpass Filter"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_2
                 Text {
                   id: header_story
                   anchors.centerIn: parent
                   text: "Story"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_5
                 Text {
                   id: header_sol
                   anchors.centerIn: parent
                   text: "Saving on Local"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_5
                 Text {
                   id: header_sow
                   anchors.centerIn: parent
                   text: "Saving on Web"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_5
                 Text {
                   id: header_stw
                   anchors.centerIn: parent
                   text: "Sending to Web"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_st
                   anchors.centerIn: parent
                   text: "Sensor Type"
                 }
               }
            }

            delegate: Rectangle {
//                color: if(model.channelNumber%2 == 0){"red"}
                height: 30
                RowLayout {
                    Layout.fillWidth: false
                    spacing: 10
                    Layout.topMargin: 100
                    Rectangle {
                        width: monitoringTab.baseWidth*monitoringTab.md_4
                        Text {
                            anchors.centerIn: parent
                            text: model.channelNumber
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: monitoringTab.baseWidth*monitoringTab.md_3
                        Text {
                            anchors.centerIn: parent
                            text: model.routerNumber
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: monitoringTab.baseWidth*monitoringTab.md_3
                        Text {
                            anchors.centerIn: parent
                            text: model.sensorNumber
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: monitoringTab.baseWidth*monitoringTab.md_2
                        Text {
                            anchors.centerIn: parent
                            text: model.bordar
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: monitoringTab.baseWidth*monitoringTab.md_4
                        Layout.fillWidth: false
                        TextInput {
                            anchors.centerIn: parent
                            text: model.description
                            onEditingFinished: model.description = text
                            color: "green"
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_3
                        Text {
                            anchors.centerIn: parent
                            text: model.batteryLevel
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_3
                        TextInput {
                            anchors.centerIn: parent
                            text: model.sampleRate
                            onEditingFinished: model.sampleRate = text
                            color: "green"
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_3
                        TextInput {
                            anchors.centerIn: parent
                            text: model.bandpassFilter
                            onEditingFinished: model.bandpassFilter = text
                            color: "green"
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_2
                        TextInput {
                            anchors.centerIn: parent
                            text: model.story
                            color: "green"
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_5
                        CheckBox {
                            text: "savingOnLocal"
                            checked: model.savingOnLocal
                            anchors.centerIn: parent
                            onCheckStateChanged: model.savingOnLocal = checked
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_5
                        CheckBox {
                            text: "savingOnWeb"
                            checked: model.savingOnWeb
                            anchors.centerIn: parent
                            onCheckStateChanged: model.savingOnWeb = checked
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_5
                        CheckBox {
                            text: "sendingToWeb"
                            checked: model.sendingToWeb
                            anchors.centerIn: parent
                            onCheckStateChanged: model.sendingToWeb = checked
                        }
                    }
                    ToolSeparator {}
                    Rectangle {
                        width: baseWidth*md_3
                        TextInput {
                            anchors.centerIn: parent
                            text: model.sensorType
                            color: "green"
                        }
                    }
                }
            }
        }

        Button {
            text: qsTr("Apply")
//            anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: listView.bottom
            onClicked: {
               console.log("Apply");
                BackEnd.sendSettings();
//               console.log(SensorsList)
            }
        }
}
//    RowLayout {
//        Button {
//            text: qsTr("Add new Item  ")
//            Layout.fillWidth: false
//            onClicked: {
//                SensorsList.appendItem()
////                console.log("SensorsList.sensorData")
////                console.log(SensorsList.getSensorsList())
//            }
//        }
//        Button {
//            text: qsTr("Add new Item Backend")
//            Layout.fillWidth: false
//            onClicked: backend.appendItem()
//        }
//    }
//}
