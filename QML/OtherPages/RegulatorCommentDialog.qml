import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import "../MyWidgets"
import "../../Resources"

FramlessWindow {
    id: commentWindow
    objectName: "chooseRegulatorWindow"
    width: 640 + 40
    height: 480 + 40

    property int currentArticleIndex: -1

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            commentWindow.close();
        }
    }

    onCurrentArticleIndexChanged: {
        contentEdit.text = Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss");
        contentEdit.text += ": \n\t"
        console.log(Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss.zzz ddd"));
    }


    Strings{
        id: stringsPool
    }

    visible: true

    childCont.children: [
        Rectangle{
            height: 480
            width: 640
            clip: true


            TextEdit {
                id: contentEdit
                clip: true
                x: 24
                y: 24 + 28 + 24
                width: 640 - 24*2
                height: 480 - 24 - 28 - 24 - 24 - 40 - 24
                focus: true
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                selectByMouse: true
                font{family: "DengXian";pixelSize: 17}
                property string placeholderText: "在此输入反馈内容"


                Text {
                    text: contentEdit.placeholderText
                    color: stringsPool.textGray3
                    visible: !contentEdit.text
                    font{family: "DengXian";pixelSize: 17}
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.IBeamCursor
                    onPressed: mouse.accepted=false
                    onReleased:  mouse.accepted=false
                }
            }


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
                    regulatorPageHandler.commentToTranslator(currentArticleIndex, contentEdit.text);
                    commentWindow.close();
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
