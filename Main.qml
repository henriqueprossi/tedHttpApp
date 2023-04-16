import QtQuick
import QtQuick.Window
import QtQuick.Controls
import br.com.colleter.ted

Window {
    id: root

    width: 640
    height: 480
    visible: true
    title: qsTr("Aplicação de testes do TED HTTP")

    Connections {
        target: NetworkManager

        function onConnected(ip: string) {
            addLog("connected: " + ip);
            let connectedTED = { ip: ip };
            listConnectedTEDsModel.append(connectedTED);
        }

        function onDisconnected(ip: string) {
            addLog("disconnected: " + ip);
            let disconnectedTED = { ip: ip };
            //listConnectedTEDsModel.remove() append(connectedTED);
        }
    }

    Rectangle {
        id: rectConnection

        width: parent.width
        height: 200

        state: "stateBlockConnections"

        states: [
            State {
                name: "stateAcceptConnections"
                PropertyChanges {
                    target: btnAcceptBlockConnections
                    text: "Bloquear conexões"
                }
            },
            State {
                name: "stateBlockConnections"
                PropertyChanges {
                    target: btnAcceptBlockConnections
                    text: "Aceitar conexões"
                }
            }
        ]

        onStateChanged: {
            addLog("state changed to: " + state);
        }

        Button {
            id: btnAcceptBlockConnections

            width: 200
            height: 100

            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                if (parent.state === "stateAcceptConnections") {
                    parent.state = "stateBlockConnections";
                } else {
                    parent.state = "stateAcceptConnections";
                }
            }
        }

        Label {
            id: txtConnectedTEDs
            //text: (listViewConnectedTEDs.count === 0) ? "TEDs conectados:" : "TEDs conectados (selecione um IP):"
            text: "TEDs conectados:"

            color: "black"

            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: rectConnectedTEDs.left
        }

        Rectangle {
            id: rectConnectedTEDs

            width: 400
            height: 150

            anchors.top: txtConnectedTEDs.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10

            border.color: "black"
            border.width: 1

            ListView {
                id: listViewConnectedTEDs

                anchors.fill: parent
                anchors.margins: 10
                clip: true
                spacing: 5
                focus: true

                model: listConnectedTEDsModel
                delegate: connectedTEDItemDelegate
                //highlight: highlightedItemDelegate
            }

            Component {
                id: connectedTEDItemDelegate
                Rectangle {
                    id: tedItem
                    required property int index
                    required property string ip

                    width: ListView.view.width
                    height: 20

                    color: ListView.isCurrentItem ? "burlywood" : "cyan"
                    border.color: Qt.lighter(color, 1.1)
                    border.width: 2

                    Text {
                        anchors.centerIn: parent
                        text: "TED " + tedItem.ip
                        color: (tedItem.index === listViewConnectedTEDs.currentIndex) ? "black" : "darkgray"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listViewConnectedTEDs.currentIndex = tedItem.index
                            addLog("clicked: " + tedItem.index + ", current item: " + listViewConnectedTEDs.currentIndex);
                        }
                    }
                }
            }

//            Component {
//                id: highlightedItemDelegate
//                Rectangle {
//                    color: "lightsteelblue"
//                    width: ListView.view ? ListView.view.width : 0
//                }
//            }

            ListModel {
                id: listConnectedTEDsModel
                //ListElement { ip: "192.168.0.1" }
                //ListElement { ip: "192.168.0.2" }
                //ListElement { ip: "192.168.0.3" }
            }

//            DeviceModel {
//                id : listConnectedTEDsModel
//            }
        }
    }

    Rectangle {
        id: rectLogs

        width: parent.width
        height: 200

        anchors.top: rectConnection.bottom
        anchors.topMargin: 5

        border.color: "black"
        border.width: 1

        ListView {
            id: listViewLogs

            anchors.fill: parent
            anchors.margins: 10
            clip: true
            spacing: 1
            currentIndex: -1

            highlightFollowsCurrentItem: true

            model: listLogsModel
            delegate: logItemDelegate
            highlight: highlightedLogDelegate
        }

        Component {
            id: logItemDelegate
            Item {
                id: logItem
                required property string text

                width: ListView.view ? ListView.view.width : 0
                height: 20

                Text {
                    anchors.left: parent.left
                    text: "log: " + logItem.text
                }
            }
        }

        ListModel {
            id: listLogsModel
        }

        Component {
            id: highlightedLogDelegate
            Rectangle {
                color: "cornsilk"
                width: ListView.view ? ListView.view.width : 0
            }
        }
    }

    function addLog(logText: string) {
        let log = {
            text: logText
        }
        listLogsModel.append(log);
        listViewLogs.currentIndex = listViewLogs.count - 1
    }

//    function findConnectedDevice(ip: string) {

//    }
}
