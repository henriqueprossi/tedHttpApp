import QtQuick
import QtQuick.Window
import QtQuick.Controls
import br.com.colleter.ted

Window {
    id: root

    width: 640
    height: 800
    visible: true
    title: qsTr("Aplicação de testes do TED HTTP")

    function addLog(logText: string) {
        let log = {
            text: logText
        }
        listLogsModel.append(log);
        listViewLogs.currentIndex = listViewLogs.count - 1
    }

    function onCommandSent(ip: string) {
        addLog("command sent to " + ip);
        popupWaitReply.open();
    }

    function onCommandReply(ip: string, body: string) {
        addLog("reply received from " + ip + ", body: " + body);
        popupWaitReply.close();
    }

    function onCommandTimeout(ip: string) {
        addLog("reply timeout from " + ip);
        popupWaitReply.close();
    }

    Popup {
        id: popupWaitReply

        width: 100
        height: 100
        anchors.centerIn: parent

        modal: true
        closePolicy: Popup.NoAutoClose
        visible: false

        BusyIndicator {
            id: busyWaitReply
            running: popupWaitReply.opened
            anchors.fill: parent
        }
    }

    Connections {
        target: TedManager

        function onConnected(ip: string) {
            addLog("connected: " + ip);
            listConnectedDevicesModel.append(ip);
        }

        function onDisconnected(ip: string) {
            addLog("disconnected: " + ip);
            listConnectedDevicesModel.remove(ip);
        }

        function onReplyReceived(ip: string, body: string) {
            onCommandReply(ip, body);
        }

        function onReplyTimeout(ip: string) {
            onCommandTimeout(ip);
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

            if (state === "stateAcceptConnections") {
                TedManager.startMonitoringConnection();
            } else {
                TedManager.stopMonitoringConnection();
            }
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

                model: listConnectedDevicesModel
                delegate: connectedTEDItemDelegate
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

            ConnectedDeviceModel {
                id : listConnectedDevicesModel
            }
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

    Rectangle {
        id: rectCommands

        width: parent.width
        height: 200

        enabled: (listViewConnectedTEDs.currentIndex !== -1)

        anchors.top: rectLogs.bottom
        anchors.topMargin: 10

        anchors.left: rectLogs.left
        anchors.leftMargin: 10

        Label {
            id: txtCommands

            text: "Comandos:"

            color: "black"

            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
        }

        Button {
            id: btnSendTextToTed

            width: 300
            height: 50

            anchors.top: txtCommands.bottom
            anchors.topMargin: 10

            text: "Enviar texto"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.sendTextToTed(ip, 8090, txtEditSendTextToTed.text);

                onCommandSent(ip);
            }
        }

        TextEdit {
            id: txtEditSendTextToTed

            anchors.top: btnSendTextToTed.bottom
            anchors.topMargin: 10

            cursorVisible: true

            text: "Digite aqui o texto"
        }

        Button {
            id: btnClearDisplay

            width: 300
            height: 50

            anchors.top: txtEditSendTextToTed.bottom
            anchors.topMargin: 10

            text: "Limpar display"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.clearDisplay(ip, 8090);

                onCommandSent(ip);
            }
        }
    }
}
