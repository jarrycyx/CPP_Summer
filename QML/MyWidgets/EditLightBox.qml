import QtQuick 2.0

import "../../Resources"


Component {
    AcrylicBlock {
        Strings{id: stringsPool}
        id: editBox
        property int indexOfThisDelegate: index
        property string placeholderText: placeHolderText
        property var view: ListView.view


        outsideColor: "#66000000"
        hoverBorderColor: "#9A000000"


        default property bool thisIsSelected: ListView.isCurrentItem
        borderWidth: 2
        height: 48
        width: indexOfThisDelegate===0 ? 305 : 444

        color: "#fdfdfd"
        Behavior on color {
            ColorAnimation { duration: 100 }
        }


        onThisIsSelectedChanged: {
            selected=ListView.isCurrentItem;
            console.log(indexOfThisDelegate+" is "+selected);
        }

        MouseArea {
            cursorShape: Qt.IBeamCursor
            id: dragArea
            height: parent.height
            width: parent.width
            onPressed: {
                editBox.changeStatus(1);
                view.currentIndex=indexOfThisDelegate;
                console.log("p"+loginList.currentIndex);
                mouse.accepted=false;
            }
            onReleased: {
                editBox.changeStatus(2);
                mouse.accepted=false;
            }


            hoverEnabled: true
            onEntered: {
                console.log("en");
                editBox.changeStatus(3);
            }
            onExited: {
                console.log("ex");
                editBox.changeStatus(4);
            }
        }


        childCont.children:
            TextEdit {
                selectByMouse: true
                verticalAlignment: TextEdit.AlignVCenter
                id: editName
                text: textInEdit
                onTextChanged: {
                    textInEdit=text
                }
                anchors.fill: parent
                anchors.margins: 5
                anchors.leftMargin: 10
                font.family: "DengXian"
                Text {
                    text: editBox.placeholderText
                    color: stringsPool.textGray3
                    visible: !editName.text
                    height: parent.height
                    verticalAlignment: TextEdit.AlignVCenter
                    font{family: "DengXian"}
                }
            }
    }
}
