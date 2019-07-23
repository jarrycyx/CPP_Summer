import QtQuick 2.0


import "../Resources"

Component {
    //id: dragDelegate


    MouseArea {
        Strings{id: stringsPool}
        //FontLoader { id: pingfangFont; source: "../Resources/PingFang Regular.ttf" }

        z: 2
        id: dragArea
        property bool held: false
        property bool released: false
        property bool movedToTarget: false
        property int indexOfThisDelegate: index
        default property bool selected: GridView.isCurrentItem//ListView.isCurrentItem


        //anchors { left: content.left; right: content.right }
        //anchors.centerIn: parent
        height: content.height+45
        width: content.width+30

        signal selectedIndexChange(int idx)

        drag.axis: typeOfList===1 ? Drag.XAndYAxis : Drag.None
        drag.target: content

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
                movedToTarget=true;
                senderPageHandler.deleteSenderArticle(indexOfThisDelegate);
            }
            else movedToTarget=false;

            console.log("r"+ListView.delegate+ListView.flag);

            if (typeof senderArticlesList!=='undefined')
                senderArticlesList.currentIndex=-1;
            if (typeof otherArticlesList!=='undefined')
                otherArticlesList.currentIndex=-1;
            if (typeof senderSubarticlesList!=='undefined')
                senderSubarticlesList.currentIndex=indexOfThisDelegate;

            content.changeStatus(2);
            newEditor.editOrViewAnArticle(titleOfArticle, contentOfArticle, translatedTitle, translatedContent,
                                          statusCodeOfArticle, indexOfThisDelegate, typeOfList);
            mainWindow.foldList();

        }
        onSelectedChanged: {
            content.selected=GridView.isCurrentItem;
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
            height: 76
            width: 305
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
                    //ParentChange { target: content; parent: rect }
                }]
            childCont.children:  [
                Image {
                    id: borderImage
                    x: 12
                    y: 13
                    width: 45
                    height: 45
                    sourceSize.width: 45
                    sourceSize.height: 45
                    source: "../Resources/status-code/"+statusCodeOfArticle+".svg"
                },
                Text{
                    id: titleText
                    text: titleOfArticle+"/"+statusCodeOfArticle
                    x: 73
                    y: 16
                    width: 210
                    elide: Text.ElideRight
                    height: 14
                    color: stringsPool.textGray1
                    font {
                        family: "DengXian"
                        pixelSize: 20
                    }
                },
                Text{
                    id: previewText
                    text: contentOfArticle
                    x: 73
                    y: 33
                    width: 210
                    height: 24
                    wrapMode: Text.WrapAnywhere
                    color: stringsPool.textGray3
                    maximumLineCount: 1
                    verticalAlignment: Text.AlignBottom
                    font{
                        weight: Font.Light
                        family: "DengXian"//pingfangFont.name
                        bold: false
                        pixelSize: 12
                    }
                }
            ]
        }


    }

}
