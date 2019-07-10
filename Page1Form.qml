import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    width: 1280
    height: 720

    title: qsTr("Page 1")

    Row {
        id: row
        x: 314
        y: 0
        width: 200
        height: 400
        Rectangle{
            color: "#eeeeee"
            width: 200
            height: 400
        }

    }

    Button {
        id: button
        x: 58
        y: 49
        width: 173
        height: 74
        text: qsTr("Button")

        MouseArea {
            id: mouseArea
            width: 173
            height: 74
            x: 0
            y: -2
            property real lastX: 0
            property real lastY: 0
            onPressed: {
                lastX=mouseX;
                lastY=mouseY;
            }
            onReleased: {
                if (button.x<=314){
                    button.x=58
                    button.y=49
                }
            }

            onPositionChanged: {
                if (pressed){
                    button.x+=mouseX-lastX;
                    button.y+=mouseY-lastY;
                }
            }
        }
    }

    ListModel {
        id: fruitModel

        ListElement {
            name: "Apple"
            cost: 2.45
            attributes: [
                ListElement { description: "Core" },
                ListElement { description: "Deciduous" }
            ]
        }
        ListElement {
            name: "Orange"
            cost: 3.25
            attributes: [
                ListElement { description: "Citrus" }
            ]
        }
        ListElement {
            name: "Banana"
            cost: 1.95
            attributes: [
                ListElement { description: "Tropical" },
                ListElement { description: "Seedless" }
            ]
        }
    }
    Component {
            id: fruitDelegate
            Row {
                spacing: 10
                Text { text: name }
                Text { text: '$' + cost }
            }
        }

        ListView {
            anchors.fill: parent
            model: fruitModel
            delegate: fruitDelegate
        }

}
