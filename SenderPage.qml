import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

import "./Components"
import "./Resources"

ApplicationWindow {
    id: senderWindow
    objectName: "senderWindow"
    visible: false
    width: 1280
    height: 800
    title: qsTr("Stack")
    Strings{id: stringsPool}

    Connections {
        target: senderPageHandler
        onRefreshQml: {
            console.log("refresh");
            thisBusyIndicator.visible = false;
            senderArticlesList.model=senderPageHandler.thisModel;
        }
        onLoadArticlesComplete: {
            thisBusyIndicator.visible = false;
        }
    }

    Connections{
        target: loginPageHandler
        onRequireComplete: {
            if (flag==1) senderWindow.visible=true;
        }
    }


    Rectangle {
        id: root
        anchors.fill: parent
        ArticleBlock {
            id: dragDelegate
        }
        RectangularGlow {
            id: effect
            anchors.fill: articlesRect
            glowRadius: 10
            spread: 0
            color: "#66999999"
            cornerRadius: articlesRect.radius + glowRadius
        }

        Rectangle {
            id: articlesRect
            width: 388
            height: parent.height
            color: "#f2f2f2"

            ListView {
                id: senderArticlesList
                anchors{fill:parent; topMargin:122}
                model: senderPageHandler.thisModel
                remove: Transition {
                        ParallelAnimation {
                            NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                            NumberAnimation { properties: "x,y"; to: 100; duration: 1000 }
                        }
                    }
                delegate: dragDelegate
                spacing: 32.5
                cacheBuffer: 50
            }

            Rectangle {
                color: "#f2f2f2"
                x:0
                y:0
                width:388
                height: 122
            }
            Rectangle {
                color: stringsPool.textGray3
                x:41
                y:76
                width:305
                height: 1
            }

            Text{
                text: qsTr("我发布的文章")
                x: 41
                y: 95
                width: 261
                height: 18
                color: stringsPool.textGray1
                font{family: "DengXian";pixelSize: 16}
            }

            Rectangle{
                x: 41
                y: 21
                width: 305
                height: 48
                color: "#f2f2f2"
                ToolButton {
                    id: toolButton
                    x: 0
                    y: 0
                    anchors.fill: parent
                    Text{
                        x: 60
                        y: 13
                        height: 22
                        text: qsTr("新的文章需求")
                        font{
                            family: "DengXian";
                            pixelSize: 22
                        }
                    }
                    onClicked: {
                        //blankText.visible=false;
                        //newSenderEditor.visible=true;
                        newSenderEditor.addAnArticle();
                    }
                }

                Image {
                    id: borderImage
                    x: 8
                    y: 8
                    width: 32
                    height: 32
                    sourceSize.width: 32
                    sourceSize.height: 32
                    source: "Resources/add.svg"
                }
            }


        }

        SenderEditor {
            x: articlesRect.width+30
            y: 30
            id: newSenderEditor
            visible: false
            width: senderWindow.width-x-30
            height: senderWindow.height-60
        }

        BusyIndicator {
            id: thisBusyIndicator
            y: (senderWindow.height-80)/2
            x: (senderWindow.width-388-80)/2+388
            //visible: false
            width: 80
            height: 80
        }

        Text {
            id: blankText
            y: (senderWindow.height-80)/2
            x: (senderWindow.width-388-160)/2+388
            text: "可以在左侧查看或添加翻译需求"
            color: stringsPool.textGray2
            width: 160
            height: 80
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font{
                family: "DengXian";
                pixelSize: 16
            }
        }
    }



}
