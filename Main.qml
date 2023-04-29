import QtQuick
import QtQuick.Window
import QtQuick.Controls
import br.com.colleter.ted

Window {
    id: root

    property int propBorderMargin: 5
    property int propButtonHeight: 40
    property int propButtonWidth: 200
    property color propBorderColor: "#21be2b"

    width: 900
    height: 720
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

        function onReplyReceivedReadDigitalInput(value: int) {
            txtDigitalInputValue.text = value;
        }
    }

    Rectangle {
        id: rectConnection

        width: parent.width
        height: 110

        //border.color: propBorderColor

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
            height: 50

            anchors {
                left: parent.left
                leftMargin: propBorderMargin
                verticalCenter: parent.verticalCenter
            }

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

            anchors {
                top: parent.top
                topMargin: propBorderMargin
                left: rectConnectedTEDs.left
            }
        }

        Rectangle {
            id: rectConnectedTEDs

            width: 400
            height: 80

            anchors {
                top: txtConnectedTEDs.bottom
                topMargin: propBorderMargin
                right: parent.right
                rightMargin: propBorderMargin
            }

            border.color: propBorderColor //"black"
            //border.width: 1

            ListView {
                id: listViewConnectedTEDs

                anchors.fill: parent
                anchors.margins: propBorderMargin
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
                            //addLog("clicked: " + tedItem.index + ", current item: " + listViewConnectedTEDs.currentIndex);
                        }
                    }
                }
            }

            ConnectedDeviceModel {
                id : listConnectedDevicesModel
            }
        }
    }

    Label {
        id: txtLogs

        text: "Logs:"

        color: "black"

        anchors {
            top: rectConnection.bottom
            topMargin: propBorderMargin
            left: rectLogs.left
        }
    }

    Rectangle {
        id: rectLogs

        width: parent.width
        height: 100

        anchors {
            top: txtLogs.bottom
            topMargin: propBorderMargin
            left: parent.left
            leftMargin: propBorderMargin
            right: parent.right
            rightMargin: propBorderMargin
        }

        border.color: propBorderColor //"black"
        //border.width: 1

        ListView {
            id: listViewLogs

            anchors.fill: parent
            anchors.margins: propBorderMargin
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
                    text: logItem.text
                    color: "lightseagreen"
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

    Label {
        id: txtCommands

        text: "Comandos:"

        color: "black"

        anchors {
            top: rectLogs.bottom
            topMargin: propBorderMargin
            left: parent.left
            leftMargin: propBorderMargin
        }
    }

    Rectangle {
        id: rectCommands

        width: parent.width
        height: 410

        anchors {
            top: txtCommands.bottom
            topMargin: propBorderMargin
            left: parent.left
            leftMargin: propBorderMargin
            right: parent.right
            rightMargin: propBorderMargin
        }

        border.color: propBorderColor

        enabled: (listViewConnectedTEDs.currentIndex !== -1)

        Button {
            id: btnSendTextToTed

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: parent.top
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Enviar texto"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.sendTextToTed(ip, 8090, txtAreaSendTextToTed.text);

                onCommandSent(ip);
            }
        }

        TextArea {
            id: txtAreaSendTextToTed

            width: 200
            height: btnSendTextToTed.height

            anchors {
                verticalCenter: btnSendTextToTed.verticalCenter
                left: btnSendTextToTed.right
                leftMargin: propBorderMargin
            }

            verticalAlignment: TextEdit.AlignVCenter
            color: "black"
            //cursorVisible: true
            wrapMode: TextEdit.Wrap
            placeholderText: "Digite aqui o texto"

            background: Rectangle {
                border.color: txtAreaSendTextToTed.enabled ? propBorderColor : "transparent"
            }
        }

        Button {
            id: btnClearDisplay

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnSendTextToTed.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Limpar display"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.clearDisplay(ip, 8090);

                onCommandSent(ip);
            }
        }

        Button {
            id: btnBeepInit

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnClearDisplay.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Beep de inicialização"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.beepInit(ip, 8090);

                onCommandSent(ip);
            }
        }

        Button {
            id: btnBeeps

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnBeepInit.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Emitir beeps"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.beeps(ip, 8090, spinBeeps.value);

                onCommandSent(ip);
            }
        }

        SpinBox {
            id: spinBeeps

            width: 100
            height: btnBeeps.height

            anchors {
                verticalCenter: btnBeeps.verticalCenter
                left: btnBeeps.right
                leftMargin: propBorderMargin
            }

            from: 1
            to: 255
            stepSize: 1
            value: 1
        }

        Button {
            id: btnSendToCOM1

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnBeeps.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Enviar para COM1"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.sendToCOM1(ip, 8090, txtAreaSendToCOM1.text);

                onCommandSent(ip);
            }
        }

        TextArea {
            id: txtAreaSendToCOM1

            width: 200
            height: btnSendToCOM1.height

            anchors {
                verticalCenter: btnSendToCOM1.verticalCenter
                left: btnSendToCOM1.right
                leftMargin: propBorderMargin
            }

            verticalAlignment: TextEdit.AlignVCenter
            color: "black"
            //cursorVisible: true
            wrapMode: TextEdit.Wrap
            placeholderText: "Digite aqui o texto"

            background: Rectangle {
                border.color: txtAreaSendToCOM1.enabled ? propBorderColor : "transparent"
            }
        }

        Button {
            id: btnSendToCOM2

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnSendToCOM1.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Enviar para COM2"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.sendToCOM2(ip, 8090, txtAreaSendToCOM2.text);

                onCommandSent(ip);
            }
        }

        TextArea {
            id: txtAreaSendToCOM2

            width: 200
            height: btnSendToCOM2.height

            anchors {
                verticalCenter: btnSendToCOM2.verticalCenter
                left: btnSendToCOM2.right
                leftMargin: propBorderMargin
            }

            verticalAlignment: TextEdit.AlignVCenter
            color: "black"
            //cursorVisible: true
            wrapMode: TextEdit.Wrap
            placeholderText: "Digite aqui o texto"

            background: Rectangle {
                border.color: txtAreaSendToCOM2.enabled ? propBorderColor : "transparent"
            }
        }

        Button {
            id: btnReadDigitalInput

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnSendToCOM2.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Ler entrada digital"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.readDigitalInput(ip, 8090);

                onCommandSent(ip);
            }
        }

        Text {
            id: txtDigitalInputValue

            width: 50
            height: btnReadDigitalInput.height

            anchors {
                verticalCenter: btnReadDigitalInput.verticalCenter
                left: btnReadDigitalInput.right
                leftMargin: propBorderMargin
            }

            verticalAlignment: TextEdit.AlignVCenter
            horizontalAlignment: TextEdit.AlignHCenter
            color: "black"
            text: "-"
        }

        Button {
            id: btnDigitalInputOn

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnReadDigitalInput.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Ligar saída digital"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.turnOnDigitalInput(ip, 8090);

                onCommandSent(ip);
            }
        }

        Button {
            id: btnDigitalInputOff

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnDigitalInputOn.bottom
                topMargin: propBorderMargin
                left: parent.left
                leftMargin: propBorderMargin
            }

            text: "Desligar saída digital"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.turnOffDigitalInput(ip, 8090);

                onCommandSent(ip);
            }
        }

        //-----------------

        Button {
            id: btnClearShortcutMenu

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: parent.top
                topMargin: propBorderMargin
                left: txtAreaSendTextToTed.right
                leftMargin: propBorderMargin * 2
            }

            text: "Limpar menu de atalhos"

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;
                TedManager.clearShortcutMenu(ip, 8090);

                onCommandSent(ip);
            }
        }

        Button {
            id: btnAddShortcutPage

            property list<string> pagesList: ["", "", "", "", "", "", ""]

            width: propButtonWidth
            height: propButtonHeight

            anchors {
                top: btnClearShortcutMenu.bottom
                topMargin: propBorderMargin
                left: btnClearShortcutMenu.left
            }

            text: "Adicionar página..."

            onClicked: {
                let activeConnectedTed = listViewConnectedTEDs.currentItem;
                let ip = activeConnectedTed.ip;

                pagesList[0] = txtAddShortcutPageLine1.text;
                pagesList[1] = txtAddShortcutPageLine2.text;
                pagesList[2] = txtAddShortcutPageLine3.text;
                pagesList[3] = txtAddShortcutPageLine4.text;
                pagesList[4] = txtAddShortcutPageLine5.text;
                pagesList[5] = txtAddShortcutPageLine6.text;
                pagesList[6] = txtAddShortcutPageLine7.text;

                TedManager.addPageToShortcutMenu(ip, 8090, pagesList);

                onCommandSent(ip);
            }
        }

        Label {
            id: txtPageContents

            text: "Conteúdo da página:"

            color: "black"

            anchors {
                top: btnAddShortcutPage.top
                left: btnAddShortcutPage.right
                leftMargin: propBorderMargin
            }
        }

        Rectangle {

            id: rectPages

            width: 250
            height: 330

            anchors {
                top: txtPageContents.bottom
                topMargin: propBorderMargin
                left: txtPageContents.left
            }

            border.color: propBorderColor

            enabled: (listViewConnectedTEDs.currentIndex !== -1)

            TextEdit {
                id: txtAddShortcutPageLine1

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: parent.top
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "1 15 caracteres"
            }

            TextEdit {
                id: txtAddShortcutPageLine2

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: txtAddShortcutPageLine1.bottom
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "2 15 caracteres"
            }

            TextEdit {
                id: txtAddShortcutPageLine3

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: txtAddShortcutPageLine2.bottom
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "3 15 caracteres"
            }

            TextEdit {
                id: txtAddShortcutPageLine4

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: txtAddShortcutPageLine3.bottom
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "4 15 caracteres"
            }

            TextEdit {
                id: txtAddShortcutPageLine5

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: txtAddShortcutPageLine4.bottom
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "5 15 caracteres"
            }

            TextEdit {
                id: txtAddShortcutPageLine6

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: txtAddShortcutPageLine5.bottom
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "6 15 caracteres"
            }

            TextEdit {
                id: txtAddShortcutPageLine7

                width: 50
                height: btnAddShortcutPage.height

                anchors {
                    top: txtAddShortcutPageLine6.bottom
                    topMargin: propBorderMargin
                    left: parent.left
                    leftMargin: propBorderMargin
                }

                verticalAlignment: TextEdit.AlignVCenter

                text: "7 15 caracteres"
            }
        }
    }
}
