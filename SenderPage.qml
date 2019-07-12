import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import qt.cpp.ProcessingModel 1.0

import "./Components"
import "./Resources"

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 1280
    height: 800
    title: qsTr("Stack")
    Strings{id: stringsPool}

    ProcessingModel{
        id: myProcessingModel
        objectName: "myProcessingModel"
    }
    function refreshView(){
        view.model=myProcessingModel.thisModel;
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
                id: view
                anchors{fill:parent; topMargin:122}
                model: myProcessingModel.thisModel
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
                        font{family: "DengXian";pixelSize: 22}
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
            width: mainWindow.width-x-30
            height: mainWindow.height-60
        }
    }

}
