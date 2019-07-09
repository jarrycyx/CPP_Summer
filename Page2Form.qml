import QtQuick 2.12
import QtQuick.Controls 2.5

import qt.cpp.ProcessingModel 1.0

Page {
    width: 600
    height: 400

    title: qsTr("Page 2")

    ProcessingModel{
        id: myProcessingModel
        objectName: "myProcessingModel"
    }

    function refreshView(){
        view.model=myProcessingModel.thisModel;
    }



    Rectangle {
        id: root

        width: 500; height: 400

        Component {
            id: dragDelegate

            MouseArea {
                id: dragArea


                property bool held: false
                property bool released: false
                property bool movedToTarget: false
                property int indexOfThisDelegate: index

                anchors { left: parent.left; right: parent.right }
                height: content.height

                drag.target: held ? content : row
                drag.axis: Drag.XAndYAxis

                onPressed: {
                    held = true;
                    released = false
                }
                onReleased: {
                    if (held) released=true;
                    held = false;
                    if (dragArea.mouseX>250) {
                        console.log(dragArea.mouseX+" "+indexOfThisDelegate+" "+myModel)
                        movedToTarget=true;
                        myProcessingModel.itemMove(indexOfThisDelegate);
                        refreshView();
                    }
                    else movedToTarget=false;

                }

                Rectangle {
                    id: content
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    width: dragArea.width + 4;
                    height: column.implicitHeight + 4

                    color: dragArea.held ? "#bbbbbb" : "#dddddd"
                    Behavior on color {
                        ColorAnimation { duration: 100 }
                    }
                    states: [State {
                        when: dragArea.held
                        ParentChange { target: content; parent: root }
                        AnchorChanges {
                            target: content
                            anchors { horizontalCenter: undefined; verticalCenter: undefined }
                        }
                    },State {
                         when: dragArea.released&&dragArea.movedToTarget
                         ParentChange { target: content; parent: rect }
                    }]
                    Column {
                        id: column
                        anchors { fill: parent; margins: 2 }

                        Text {
                            //anchors.margins: 10
                            //verticalAlignment: Text.AlignHCenter
                            horizontalAlignment: Text.AlignHCenter
                            width: parent.width
                            height: 30
                            text: modelData.name
                        }
                    }
                }
            }
        }

        ListView {
            id: view
            width: 150
            anchors.rightMargin: 302

            anchors { fill: parent; margins: 2 }

            model: myProcessingModel.thisModel
            delegate: dragDelegate

            spacing: 4
            cacheBuffer: 50
        }

        Row {
            id: row
            x: 281
            y: 8
            width: 200
            height: 298
            Rectangle{
                id: rect
                color: "#eeeeee"
                width: 200
                height: 298
            }
        }
    }

}
