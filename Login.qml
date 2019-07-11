import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2


ApplicationWindow {
    id: loginWindow
    objectName: "loginWindow"
    visible: true
    width: 640
    height: 480
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
                height: 50
                width: 300
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
                        }

            }

        }
    }
    ListView {
        id: view
        anchors{fill:parent; topMargin:80}
        model: loginModel
        delegate: loginDelegate
        spacing: 9
        cacheBuffer: 50
    }

    TextEdit {
        id: textEdit
        x: 280
        y: 21
        width: 80
        height: 20
        text: qsTr("Text Edit")
        font.pixelSize: 12
    }


    
    
    
}
