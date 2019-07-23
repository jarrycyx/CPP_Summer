import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

import "./Components"
import "./Resources"

FramlessWindow {
    id: commentWindow
    objectName: "chooseRegulatorWindow"
    width: 640 + 40
    height: 480 + 40

    property int currentArticleIndex: -1

    Strings{
        id: stringsPool
    }

    visible: true

    childCont.children: [
        Rectangle{
            height: 480
            width: 640

            Text {
                id: element
                x: 24
                y: 28
                width: 282
                height: 24
                text: qsTr("给译者的反馈信息")
                font.pixelSize: 24
                font.family: "DengXian"
            }

            Button {
                id: button
                x: 24
                y: 416
                width: 294
                height: 40
                text: qsTr("提交")
                font.family: "DengXian"
                onClicked: {
                    regulatorPageHandler.commentToTranslator(currentArticleIndex, "评论");
                }
            }

            Button {
                id: button1
                x: 322
                y: 416
                width: 294
                height: 40
                text: qsTr("取消")
                font.family: "DengXian"
                onClicked: {
                    commentWindow.close();
                }
            }
        }]

    //childCont.children:
}
