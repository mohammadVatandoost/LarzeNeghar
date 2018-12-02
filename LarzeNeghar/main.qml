import QtQuick 2.6
import QtQuick.Window 2.2
//import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4
import QtCharts 2.0

Window {
    visible: true
    width: 1800
    height: 760
    title: qsTr("Hello World")
    TabView {
        anchors.fill: parent
        Tab {
             title: "Channels Configuration/Monitoring"
             ChannelsList {
                 anchors.centerIn: parent
             }
        }
        Tab {
            title: "EEW Configuration"
        }
        Tab {
            title: "SHM Configuration"
        }
        Tab {
            title: "Charts"
            Charts {
               anchors.centerIn: parent
            }
        }
    }

}
