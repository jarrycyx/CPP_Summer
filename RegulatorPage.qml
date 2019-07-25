import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Window 2.0

import "./Components"
import "./Resources"

ApplicationWindow {
    property string name: "regulatorpage"

    id: mainWindow
    objectName: "mainWindow"
    visible: true
    visibility: Window.Maximized
    title: qsTr("Stack")
    Strings{id: stringsPool}

    function foldList(){
        editorCover.color="#00000000"
        articlesRect.columnNum=1;
        expandImage.rotation=0;
    }

    Connections {
        target: regulatorPageHandler
        //开始刷新QML
        onStartRefreshQml: {
            thisBusyIndicator.visible = true;
        }
        //QML刷新完成
        onRefreshQmlComplete: {
            console.log("refresh");
            thisBusyIndicator.visible = false;
            if (newEditor.visible===false) blankText.visible=true;
            senderArticlesList.model=senderPageHandler.thisModel;
            otherArticlesList.model=senderPageHandler.otherModel;
        }

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

    Connections{
        target: loginPageHandler
        onRequireComplete: {
            if (flag==1) mainWindow.visible=true;
        }
    }




    Rectangle {
        id: root
        anchors.fill: parent

        MiniArticleBlock {
            id: dragDelegate0
        }

        ThisUserArticleBlock {
            id: dragDelegate1
        }

        OtherUserArticleBlock {
            id: dragDelegate2
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
            property int columnNum: 1

            z: 2
            id: articlesRect
            width: 350*articlesRect.columnNum-45 + 41 + 41 +12//1133//388

            Behavior on width {
                NumberAnimation{duration: 150}
            }

            height: parent.height
            color: "#f2f2f2"

            ScrollView{
                width: articlesRect.width - 24
                height: parent.height
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                contentHeight: senderArticlesList.height + 47 + otherArticlesList.height + 47
                //z:0.2

                Text{
                    text: qsTr("我负责的文章和子任务")
                    x: 41
                    y: 27
                    //z:0.3
                    width: 261
                    height: 18
                    color: stringsPool.textGray1
                    font{family: "DengXian";pixelSize: 16}
                }

                GridView{
                    property int flag: 1
                    interactive: false
                    id: senderSubarticlesList
                    width: parent.width
                    x: 41-15
                    y: 43
                    height: contentHeight + 40
                    model: regulatorSubarticleList
                    delegate: dragDelegate0
                    cellWidth: 350
                    cellHeight: 92
                    cacheBuffer: 50
                    currentIndex: -1
                }

                GridView{
                    property int flag: 1
                    interactive: false
                    id: senderArticlesList
                    width: parent.width
                    x: 41-15
                    y: senderSubarticlesList.height + 22
                    height: contentHeight + 40
                    model: regulatorArticleList
                    delegate: dragDelegate1
                    cellWidth: 350
                    cellHeight: 227
                    cacheBuffer: 50
                    currentIndex: -1
                }


                Text{
                    text: qsTr("其他正在招募负责人的文章")
                    x: 41
                    y: senderArticlesList.height + senderSubarticlesList.height + 41
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
                    x: 41 - 15
                    y: senderArticlesList.height + senderSubarticlesList.height + 41
                    height: contentHeight+40
                    model: allSeekingRegulatorArticle
                    delegate: dragDelegate2
                    cellWidth: 350
                    cellHeight: 227
                    cacheBuffer: 50
                    currentIndex: -1
                }
                Rectangle{
                    x: 41 - 15
                    y: senderArticlesList.height + senderSubarticlesList.height + 41 + otherArticlesList.height
                    height: 100
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
                    source: "Resources/unfold.svg"
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

        RegulatorViewer {
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
            text: "可以在左侧查看翻译需求"
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
        y: -40
        z: 10
        height: 35
        width: messageText.width+30
        sourceSize.height: 35
        sourceSize.width: messageText.width+30
        source: "Resources/messagebox.svg"

        SequentialAnimation {
            id: messageBoxAnimation
            running: false
            NumberAnimation { target: messageBoxImg; property: "y"; to: 0; duration: 200 }
            PauseAnimation { duration: 2000 }
            NumberAnimation { target: messageBoxImg; property: "y"; to: -40; duration: 200 }
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

}
