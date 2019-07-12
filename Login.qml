import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import QtQuick.Controls.Universal 2.12

import "./Components"

ApplicationWindow {
    id: loginWindow
    objectName: "loginWindow"
    visible: true
    width: 532
    height: 383
    title: qsTr("Stack")

    ListModel {
        id: loginModel
        objectName: "loginModel"
        ListElement {
            name: "Polly"
            type: "Parrot"
            age: 12
            size: "Small"
        }
        ListElement {
            name: "Polly"
            type: "Parrot"
            age: 12
            size: "Small"
        }
    }

    Component {
        id: loginDelegate

        MouseArea {
            id: dragArea
            property bool held: false
            property bool movedToTarget: false
            property int indexOfThisDelegate: index
            default property bool selected: ListView.isCurrentItem

            anchors { left: parent.left; right: parent.right }
            height: content.height

            signal selectedIndexChange(int idx)

            onPressed: {
                held = true;
                console.log("p");
                content.changeStatus(1);
                view.currentIndex=indexOfThisDelegate;
                editName.focus=true;
                editName.forceActiveFocus();
            }
            onReleased: {
                held = false;
                content.changeStatus(2);
            }
            onSelectedChanged: {
                content.selected=ListView.isCurrentItem;
                console.log(indexOfThisDelegate+" is "+selected);
            }
            hoverEnabled: true
            onEntered: {
                console.log("en");
                content.changeStatus(3);
            }
            onExited: {
                console.log("ex");
                content.changeStatus(4);
            }

            LightBlock {
                id: content
                borderWidth: 3
                height: 48
                width: 444
                //outsideColor: "#55f2f2f2"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                color: "#fdfdfd"//dragArea.held ? "#bbbbbb" : "#dddddd"
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
                childCont.children:
                        TextEdit {
                            verticalAlignment: TextEdit.AlignVCenter
                            id: editName
                            onFocusChanged: {
                                console.log("f");
                                if (focus) {
                                    held = true;
                                    content.changeStatus(1);
                                    view.currentIndex=indexOfThisDelegate;
                                }
                                else {
                                    held = false;
                                    content.changeStatus(2);
                                }
                            }
                            text: qsTr("Text Edit")
                            anchors.fill: parent
                            anchors.margins: 5
                            anchors.leftMargin: 10
                            font{
                                weight: Font.Light
                                family: "Segoe UI"
                                bold: false
                            }
                        }

            }

        }
    }
    Rectangle{
        height: 114
        width: parent.width
        y:110
        ListView {
            id: view
            height: 114
            model: loginModel
            delegate: loginDelegate
            anchors.fill: parent
            spacing: 18
            cacheBuffer: 50
        }

    }


    Button {
        id: button
        x: 275
        y: 251
        width: 213
        height: 48
        text: qsTr("Button")
    }

    Button {
        id: button1
        x: 44
        y: 251
        width: 213
        height: 48
        text: qsTr("Button")
    }

    Image {
        id: borderImage
        x: 44
        y: 55
        width: 232
        height: 34
        sourceSize.width: 232
        sourceSize.height: 35
        source: "Resources/logintext.svg"
    }

    Text {
        id: element
        x: 44
        y: 317
        width: 154
        height: 28
        color: Universal.accent
        text: qsTr("Forget Password?")
        font{
          weight: Font.Light
          family: "Segoe UI"
          bold: false
        }
        font.pixelSize: 14
    }





}
