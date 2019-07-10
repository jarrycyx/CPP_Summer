import QtQuick 2.12
import QtQuick.Controls 2.5

import qt.cpp.ProcessingModel 1.0

Page {
    width: 1280
    height: 720
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
        width: 1280; height: 720
        Component {
            id: dragDelegate
            MouseArea {
                id: dragArea
                property bool held: false
                property bool released: false
                property bool movedToTarget: false
                property int indexOfThisDelegate: index
                default property bool selected: ListView.isCurrentItem

                anchors { left: parent.left; right: parent.right }
                height: content.height

                signal selectedIndexChange(int idx)

                drag.target: held ? content : row
                drag.axis: Drag.XAndYAxis

                onPressed: {
                    held = true;
                    released = false;
                    console.log("p");
                    content.changeStatus(1);
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

                    console.log("r");
                    view.currentIndex=indexOfThisDelegate;
                    content.changeStatus(2);
                }
                onSelectedChanged: {
                    content.selected=ListView.isCurrentItem;
                    console.log(indexOfThisDelegate+" is "+selected);
                }
                hoverEnabled: true

                onMouseXChanged: content.chaseLight(mouseX,-1)
                onMouseYChanged: content.chaseLight(-1,mouseY)
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
                    height: 100
                    width: 200
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }

                    color: "#f2f2f2"//dragArea.held ? "#bbbbbb" : "#dddddd"
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
                    childCont.children:  Column {
                        id: column
                        height: parent.height
                        width: parent.width
                        Text {
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
            width: 200
            anchors.rightMargin: 1016
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
