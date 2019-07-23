import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

import "./Components"
import "./Resources"

ApplicationWindow {
    property string name: "senderpage"

    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 1280
    height: 800
    title: qsTr("Stack")
    Strings{id: stringsPool}


    function foldList(){
        editorCover.color="#00000000"
        articlesRect.columnNum=1;
        expandImage.rotation=0;
    }




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
        z: 1
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
                NumberAnimation{duration: 150}
            }

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
                width: 350*articlesRect.columnNum-45
                height: 1
            }

            ScrollView{
                width: articlesRect.width - 24
                height: parent.height-77
                clip: true
                y: 77
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

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

                GridView{
                    property int flag: 1
                    interactive: false
                    id: senderArticlesList
                    width: parent.width
                    x: 41-15
                    y: 32
                    height: contentHeight + 40
                    model: senderArticleList
                    delegate: dragDelegate1
                    currentIndex: -1

                    cellWidth: 350
                    cellHeight: 227
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

                GridView {
                    property int flag: 2
                    interactive: false
                    id: otherArticlesList
                    width: parent.width
                    x:41-15
                    y: senderArticlesList.height + 32 + 30
                    height: contentHeight+40
                    model: allUserArticleList
                    delegate: dragDelegate2
                    currentIndex: -1

                    cellWidth: 350
                    cellHeight: 227

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
                        newEditor.addAnArticle();
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

        SenderEditor {
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
