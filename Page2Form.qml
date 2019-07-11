import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
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
                    if (dragArea.mouseX>500) {
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
                    height: 197
                    width: 290
                    outsideColor: "#00f2f2f2"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }

                    color: "#fdfdfd"//dragArea.held ? "#bbbbbb" : "#dddddd"
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
                        anchors.fill: parent
                        anchors.margins: 20
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


        RectangularGlow {
           id: effect
           anchors.fill: articlesRect
           glowRadius: 10
           spread: 0
           color: "#66999999"
           cornerRadius: rect.radius + glowRadius
        }

        Rectangle {
            id: articlesRect
            width: 320
            height: parent.height+50
            color: "#f2f2f2"

            ListView {
                id: view
                anchors{fill:parent; topMargin:80}
                model: myProcessingModel.thisModel
                delegate: dragDelegate
                spacing: 9
                cacheBuffer: 50
            }
            Rectangle {
                width: 320
                height: 80
                color: "#f2f2f2"
            }
        }



        Row {
            id: row
            x: 572
            y: 59
            width: 200
            height: 298
        }
    }

}
