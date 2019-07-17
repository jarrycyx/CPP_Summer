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
        //开始刷新QML
        onStartRefreshQml: {
            thisBusyIndicator.visible = true;
        }
        //QML刷新完成
        onRefreshQmlComplete: {
            console.log("refresh");
            thisBusyIndicator.visible = false;
            if (newSenderEditor.visible===false) blankText.visible=true;
            senderArticlesList.model=senderPageHandler.thisModel;
            otherArticlesList.model=senderPageHandler.otherModel;
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

            Rectangle {
                color: "#f2f2f2"
                x:0
                y:0
                z:0.5
                width:388
                height: 77
            }

            Rectangle {
                color: stringsPool.textGray3
                x:41
                y:76
                z:0.5
                width:305
                height: 1
            }

            ScrollView{
                width: 388
                height: parent.height-77
                y: 77
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                contentHeight: senderArticlesList.height + 47 + otherArticlesList.height + 47
                //z:0.2

                Text{
                    text: qsTr("我发布的文章")
                    x: 41
                    y: 16
                    //z:0.3
                    width: 261
                    height: 18
                    color: stringsPool.textGray1
                    font{family: "DengXian";pixelSize: 16}
                }

                ListView {
                    property int flag: 1
                    interactive: false
                    id: senderArticlesList
                    width: parent.width
                    y: 47
                    height: contentHeight + 40
                    model: senderPageHandler.thisModel
                    //populate: Transition {
                    //        NumberAnimation { properties: "x,y"; duration: 1000 }
                    //    }
                    delegate: dragDelegate
                    spacing: 32.5
                    cacheBuffer: 50
                }

                Text{
                    text: qsTr("其他文章")
                    x: 41
                    y: senderArticlesList.height + 16 + 30
                    //z:0.3
                    width: 261
                    height: 18
                    color: stringsPool.textGray1
                    font{family: "DengXian"; pixelSize: 16}
                }

                ListView {
                    property int flag: 2
                    interactive: false
                    id: otherArticlesList
                    width: parent.width
                    y: senderArticlesList.height + 47 + 30
                    height: contentHeight+40
                    model: senderPageHandler.otherModel
                    delegate: dragDelegate
                    spacing: 32.5
                    cacheBuffer: 50
                }

            }

            Rectangle {
                x: 41
                y: 21
                z:1
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
                        senderArticlesList.currentIndex=-1;
                        otherArticlesList.currentIndex=-1;
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
            visible: false
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
