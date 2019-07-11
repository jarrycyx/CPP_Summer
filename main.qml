import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0


ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 1280
    height: 800
    title: qsTr("Stack")


    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                console.log(toolButton.implicitHeight);
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: mainWindow.width * 0.66
        height: mainWindow.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Page 1")
                width: parent.width
                onClicked: {
                    stackView.push("Page1Form.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Page 2")
                width: parent.width
                onClicked: {
                    stackView.push("Page2Form.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        width: 1280
        height: 960
        objectName: "stackView"
        initialItem: "HomeForm.qml"
        anchors.fill: parent
    }

}


