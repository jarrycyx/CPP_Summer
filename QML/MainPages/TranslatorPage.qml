import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Window 2.0

import "../MyWidgets"
import "../../Resources"

ApplicationWindow {
    property string name: "translatorpage"

    id: mainWindow
    objectName: "mainWindow"
    visible: true
    visibility: Window.Maximized
    title: qsTr("译林")
    Strings{id: stringsPool}
    minimumHeight: 800
    minimumWidth: 1280

    function foldList(){
        editorCover.color="#00000000"
        articlesRect.columnNum=1;
        expandImage.rotation=0;
    }

    Connections {
        target: translatorPageHandler
        onSendErrorMessage: {
            console.log(errStr);
            messageText.text=errStr;
            messageText.color="#d13438"
            messageBoxAnimation.start();
        }
        onSendSuccessMessage: {
            console.log(successStr);
            messageText.text=successStr;
            messageText.color="#10893e"
            messageBoxAnimation.start();
        }
    }


    Rectangle {
        id: root
        anchors.fill: parent
        z: 1
        MiniArticleBlock {
            id: dragDelegate1
        }

        OtherUserArticleBlock {
            id: dragDelegate2
        }

        RectangularGlow {
            id: effect
            anchors.fill: articlesRect
            glowRadius: 10
            z: 1
            spread: 0
            color: "#66999999"
            cornerRadius: articlesRect.radius + glowRadius
        }

        Rectangle {
            property int columnNum: 1
            z:1
            id: articlesRect
            width: 350*articlesRect.columnNum-45 + 41 + 41 + 8//1133//388
            Behavior on width {
                NumberAnimation{duration: 80}
            }

            height: parent.height
            color: "#f2f2f2"


            ScrollView{
                width: articlesRect.width - 24
                height: parent.height
                clip: true
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                contentHeight: senderSubarticlesList.height + 47 + otherArticlesList.height + 30
                //z:0.2

                Text {
                    text: qsTr("我正在翻译的文章")
                    x: 41
                    y: 16
                    //z:0.3
                    width: 261
                    height: 18
                    color: stringsPool.textGray1
                    font{family: "DengXian";pixelSize: 16}
                }

                GridView {
                    property int flag: 1
                    interactive: false
                    id: senderSubarticlesList
                    width: parent.width
                    x: 41-15
                    y: 32
                    height: contentHeight + 40
                    model: translatorSubarticleList
                    delegate: dragDelegate1
                    cellWidth: 350
                    cellHeight: 92
                    cacheBuffer: 50
                    currentIndex: -1
                    add: Transition {
                        NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 200 }
                    }
                    displaced: Transition {
                        NumberAnimation { properties: "x,y"; duration: 200 }
                    }
                }

                Text {
                    text: qsTr("其他正在招募的文章")
                    x: 41
                    y: senderSubarticlesList.height + 16 + 19
                    //z:0.3
                    width: 261
                    height: 18
                    color: stringsPool.textGray1
                    font{family: "DengXian"; pixelSize: 16}
                }

                GridView {
                    property int flag: 2
                    interactive: false
                    id: otherArticlesList
                    width: parent.width
                    x:41-15
                    y: senderSubarticlesList.height + 32 + 19
                    height: contentHeight+40
                    model: allSeekingTranslatorArticle
                    delegate: dragDelegate2
                    currentIndex: -1

                    cellWidth: 350
                    cellHeight: 227

                    cacheBuffer: 50

                }

            }
            ToolButton {
                id: expandButton
                x: articlesRect.width-21
                y: (mainWindow.height-36)/2
                width: 21
                height: 36
                Image {
                    id: expandImage
                    anchors.fill: parent
                    sourceSize.width: 21
                    sourceSize.height: 36
                    source: "../../Resources/unfold.svg"
                }
                onClicked: {
                    if (articlesRect.columnNum<2){
                        editorCover.color="#55000000"
                        articlesRect.columnNum=3;
                        expandImage.rotation=180;
                    }else if (articlesRect.columnNum>2){
                        editorCover.color="#00000000"
                        articlesRect.columnNum=1;
                        expandImage.rotation=0;
                    }
                }
            }

        }

        TranslatorEditor {
            x: 388+30
            y: 30
            z: 0
            id: newEditor
            visible: false
            width: mainWindow.width-x-30
            height: mainWindow.height-60
        }

        Rectangle {
            id: editorCover
            x: 388+30
            y: 0
            z: 0.1
            width: mainWindow.width
            height: mainWindow.height
            color: "#00000000"
            Behavior on color {
                ColorAnimation { duration: 300 }
            }
        }

        Text {
            id: blankText
            visible: true
            y: (mainWindow.height-80)/2
            x: (mainWindow.width-388-160)/2+388
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



    Image {
        id: messageBoxImg
        x: (mainWindow.width-160)/2
        y: -60
        z: 10
        height: 35
        width: messageText.width+30
        sourceSize.height: 35
        sourceSize.width: messageText.width+30
        source: "../../Resources/messagebox.svg"

        SequentialAnimation {
            id: messageBoxAnimation
            running: false
            NumberAnimation { target: messageBoxImg; property: "y"; to: 0; duration: 200 }
            PauseAnimation { duration: 2000 }
            NumberAnimation { target: messageBoxImg; property: "y"; to: -60; duration: 200 }
        }

        Text {
            id: messageText
            text: qsTr("操作成功")
            color: "#10893e"
            font{
                family: "DengXian";
                pixelSize: 16
            }
            anchors.centerIn: parent
        }
    }
    DropShadow {
        anchors.fill: messageBoxImg
        radius: 10
        samples: 5
        visible: messageBoxImg.visible
        z: 10
        color: "#66999999"
        source: messageBoxImg
    }

    ToolButton {
        width: moreUserInfoText.width + 42
        height: 32
        x: parent.width-35-width
        y: 32
        z: 40
        Text {
            id: moreUserInfoText
            x: 8
            y: 9
            text: qsTr("我的用户信息")
            font.family: "DengXian";
        }
        onClicked: {
            translatorPageHandler.showUserInfo();
        }
        Image {
            x: parent.width - 28
            y: 8
            z: 40
            height: 16
            width: 16
            sourceSize.height: 16
            sourceSize.width: 16
            source: "../../Resources/moreuserinfo.svg"
        }
    }
}
