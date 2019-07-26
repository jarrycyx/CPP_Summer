import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.12

import QtGraphicalEffects 1.0
import "../../Resources"

ApplicationWindow {
    id:mainWindow
    visible: true
    color: "#00000000"
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinMaxButtonsHint

    property alias childCont: mainRec

    MouseArea {
        x: 10
        y: 10
        height: 50
        width: parent.width-20
        acceptedButtons: Qt.LeftButton
        property point clickPos: "0,0"
        onPressed: {
            clickPos  = Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            mainWindow.setX(mainWindow.x+delta.x)
            mainWindow.setY(mainWindow.y+delta.y)
        }
    }

    Rectangle {
        id:mainRec
        width: parent.width-40
        height: parent.height-40
        anchors.centerIn: parent
        clip: true
    }

    DropShadow {
        anchors.fill: mainRec
        radius: 20
        samples: 25
        color: "#20000000"
        spread: 0.0
        source: mainRec
    }
    DropShadow {
        anchors.fill: mainRec
        radius: 12.0
        samples: 25
        color: "#20000000"
        spread: 0.0
        source: mainRec
    }

}
