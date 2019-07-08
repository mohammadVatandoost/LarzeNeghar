import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import SensorModel 1.0

Item {
        id: monitoringTab
        property int columnNum: 18
        property int baseWidth: 30
        property int firstColumnWidth: 35
        property int md_1: 1
        property int md_2: 2
        property int md_3: 3
        property int md_4: 4
        property int md_5: 5
        property int textFieldWidth: columnNum*2

        Layout.fillWidth: true
        Component.onCompleted: {
        }
        ListView {
            implicitWidth: monitoringTab.width
            implicitHeight: 600
            Layout.fillWidth: false
            clip: true
            id: listView
            headerPositioning: ListView.OverlayHeader
            spacing: 0
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
                   width: firstColumnWidth*md_4
//                 Layout.width: listView.width/columnNum
//                 Layout.minimumWidth:
                 Text {
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Channel Number"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_un
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Router Number"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_sn
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Sensor Number"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_2
                 Text {
                   id: header_bn
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Bordar"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_4
                 Text {
                   id: header_des
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Description"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_bl
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Battery Level"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_sr
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Sample Rate"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_bf
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Bandpass Filter"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_2
                 Text {
                   id: header_story
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Story"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_5
                 Text {
                   id: header_sol
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Saving on Local"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_5
                 Text {
                   id: header_sow
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Saving on Web"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_5
                 Text {
                   id: header_stw
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Sending to Web"
                 }
               }
               ToolSeparator {}
               Rectangle {
                 width: baseWidth*md_3
                 Text {
                   id: header_st
                   font.bold: true
                   anchors.centerIn: parent
                   text: "Sensor Type"
                 }
               }
            }

            delegate: Rectangle {
                width: parent.width
                height: 50
                color: if(model.channelNumber%2 == 0){"#e6e6e6"} else {"#f2f2f2"}
                RowLayout {
                    Layout.fillWidth: false
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10
                    Rectangle {
                        width: monitoringTab.firstColumnWidth*monitoringTab.md_4
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
            id: applyButton
            text: qsTr("Apply")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: listView.bottom
            onClicked: {
               console.log("Apply");
                BackEnd.sendSettings();
            }
            palette {
                    button: "#5cb85c"
            }
        }
        RowLayout {
            anchors.topMargin: 10
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: applyButton.bottom
          ComboBox {
            id: comox
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.top: applyButton.bottom
            width: 200
            model: BackEnd.availablePorts()
          }

          Button {
            text: qsTr("Connect")
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.top: comox.bottom
            onClicked: {
               console.log("Connect");
                console.log(comox.currentText);
                BackEnd.connectSerialPort(comox.currentText);
            }
            palette {
                    button: "#5cb85c"
            }
          }
        }
}