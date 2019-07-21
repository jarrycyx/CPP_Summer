import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

import "./Components"
import "./Resources"

ApplicationWindow {
    id: senderWindow
    objectName: "senderWindow"
    visible: true
    width: 1280
    height: 800
    title: qsTr("Stack")
    Strings{id: stringsPool}

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
            property int columnNum: 1

            id: articlesRect
            width: 350*articlesRect.columnNum-45 + 41 + 41 +12//1133//388
            height: parent.height
            color: "#f2f2f2"

            ScrollView{
                width: articlesRect.width - 24
                height: parent.height
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                contentHeight: senderArticlesList.height + 47 + otherArticlesList.height + 47
                //z:0.2

                Text{
                    text: qsTr("我负责的文章")
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
                    id: senderArticlesList
                    width: parent.width
                    x: 41-15
                    y: 43
                    height: contentHeight + 40
                    model: regulatorArticleList
                    delegate: dragDelegate

                    cellWidth: 350
                    cellHeight: 227
                    cacheBuffer: 50
                }

                Text{
                    text: qsTr("其他正在招募负责人的文章")
                    x: 41
                    y: senderArticlesList.height + 16 + 41
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
                    y: senderArticlesList.height + 32 + 41
                    height: contentHeight+40
                    model: allSeekingRegulatorArticle
                    delegate: dragDelegate

                    cellWidth: 350
                    cellHeight: 227

                    cacheBuffer: 50
                }

            }

        }

        RegulatorViewer {
            x: articlesRect.width+30
            y: 30
            id: newEditor
            visible: false
            width: senderWindow.width-x-30
            height: senderWindow.height-60
        }

        Text {
            id: blankText
            visible: true
            y: (senderWindow.height-80)/2
            x: (senderWindow.width-388-160)/2+388
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



}
