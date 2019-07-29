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
    width: childRect.width + 40
    height: childRect.height + 40

    property int currentArticleIndex: -1

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            commentWindow.close();
        }
    }

    onCurrentArticleIndexChanged: {  //已设置文章id，则向CPP请求内容
        contentEdit.text=translatorPageHandler.getRegulatorComment(currentArticleIndex);
        contentEdit.cursorPosition=contentEdit.text.length-1;
    }


    Strings{
        id: stringsPool
    }

    visible: true

    childCont.children: [
        Rectangle{
            id: childRect
            height:  contentEdit.height+24 + 28 + 24 + 24 + 40 + 24
            width: 640
            clip: true


            TextEdit {
                id: contentEdit
                clip: true
                enabled: false
                x: 24
                y: 24 + 28 + 24
                width: 480 - 24*2
                //height: 480 - 24 - 28 - 24 - 24 - 40 - 24
                focus: true
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                selectByMouse: true
                font{
                    family: "DengXian";
                    pixelSize: 17;
                    wordSpacing: 4;
                }
                property string placeholderText: "暂时还没有反馈"
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
                text: qsTr("来自负责人的反馈信息")
                font.pixelSize: 24
                font.family: "DengXian"
            }

            Button {
                id: button
                x: 24
                y: childRect.height-64
                width: childRect.width - 48
                height: 40
                text: qsTr("确定")
                font.family: "DengXian"
                onClicked: {
                    commentWindow.close();
                }
            }


        }]

    //childCont.children:
}
