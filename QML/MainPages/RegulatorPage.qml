//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
名称：     SenderEditor
功能：     顶层控件，负责人页面
日期：     20190710 实现基本功能
          201907XX 多次扩充功能
************************************************************************************************************************/

//绑定C++流程类：regulatorPageHandler
//绑定C++ Model：regulatorSubarticleList, regulatorArticleList, allSeekingRegulatorArticle

//SenderPage, SupervisorPage, translatorPage的控件结构相似，集中在此处注释



import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Window 2.0

import "../MyWidgets"
import "../../Resources"

ApplicationWindow {
    property string name: "regulatorpage"
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
        target: regulatorPageHandler
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

        //三个list各自的delegate
        MiniArticleBlock {
            id: dragDelegate0
        }

        ThisUserArticleBlock {
            id: dragDelegate1
        }

        OtherUserArticleBlock {
            id: dragDelegate2
        }

        //防止文章块的左栏阴影效果
        RectangularGlow {
            id: effect
            anchors.fill: articlesRect
            glowRadius: 10
            spread: 0
            color: "#66999999"
            cornerRadius: articlesRect.radius + glowRadius
        }

        //防止文章块的左栏
        Rectangle {
            property int columnNum: 1   //便于左栏大小变化，代表gridview的列数

            z: 2
            id: articlesRect
            width: 350*articlesRect.columnNum-45 + 41 + 41 +12 //运算得到左栏应有的宽度

            Behavior on width {
                NumberAnimation{duration: 80}   //左栏展开和收起的动画
            }

            height: parent.height
            color: "#f2f2f2"

            ScrollView{                         //左栏允许上下滚动
                width: articlesRect.width - 24
                height: parent.height
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                contentHeight: senderSubarticlesList.height + 47
                               + senderArticlesList.height + 47
                               + otherArticlesList.height + 47
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

                //子任务列表
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

                //任务列表
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

                //正在招募负责任的任务列表
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

                //防止列表显示不完全
                Rectangle{
                    x: 41 - 15
                    y: senderArticlesList.height + senderSubarticlesList.height + 41 + otherArticlesList.height
                    height: 100
                }

            }

            //点击展开和折叠左栏的按钮
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
                        expandImage.rotation=180;//按钮旋转，从“展开”变为“折叠”提示
                    }else if (articlesRect.columnNum>2){
                        editorCover.color="#00000000"
                        articlesRect.columnNum=1;
                        expandImage.rotation=0;
                    }
                }
            }
        }

        //调用上层控件
        RegulatorViewer {
            x: 388+30
            y: 30
            z: 0
            id: newEditor
            visible: false
            width: mainWindow.width-x-30
            height: mainWindow.height-60
        }

        //左栏展开时，暗化文章编辑器
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

        //未选择要查看的文章时的提示文字
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


    //错误和成功信息的提示框
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

        //上下滑动的动画
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

    //提示框的阴影
    DropShadow {
        anchors.fill: messageBoxImg
        radius: 10
        samples: 5
        visible: messageBoxImg.visible
        z: 10
        color: "#66999999"
        source: messageBoxImg
    }

    //显示更多用户信息的按钮
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
            regulatorPageHandler.showUserInfo();
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


    //拖动文章块时显示的操作按钮
    Rectangle{
        id: dragTargetImage
        x: mainWindow.width - 120 - 40
        y: (mainWindow.height - 120) / 2
        z: 50
        color: "#00000000"
        visible: false
        height: 120
        width: 120
        property string imageSource: "../../Resources/delete.svg"

        Image {
            height: 120
            width: 120
            visible: dragTargetImage.visible
            sourceSize.height: 120
            sourceSize.width: 120
            source: dragTargetImage.imageSource
        }
    }

    //按钮的阴影效果
    DropShadow {
        anchors.fill: dragTargetImage
        radius: 20
        samples: 17
        visible: dragTargetImage.visible
        z: 50
        color: "#88999999"
        source: dragTargetImage
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
