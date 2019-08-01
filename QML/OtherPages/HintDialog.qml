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
    title: "提示"
    width: childRect.width + 40
    height: childRect.height + 40

    onVisibleChanged: {
        if (visible) startAnimation.start();
    }

    SequentialAnimation{
        id: startAnimation
        NumberAnimation {
            target: commentWindow
            property: "opacity"
            duration: 100
            from: 0
            to: 1
        }
    }

    property string titleOfDialog
    property string contentOfDialog
    property bool cancelButtonVisible

    function onAccept(){}//用于调用时重写
    function onCancel(){}

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            commentWindow.close();
        }
    }

    Strings{
        id: stringsPool
    }

    visible: true

    childCont.children: [
        Rectangle{
            id: childRect
            height:  contentText.height + 24 + 28 + 24 + 24 + 40 + 24
            width: 500
            clip: true


            Text {
                id: titleText
                clip: true
                enabled: false
                x: 24
                y: 24 + 28 + 24
                width: 480 - 24*2
                //height: 480 - 24 - 28 - 24 - 24 - 40 - 24
                text: contentOfDialog
                focus: true
                wrapMode: TextEdit.Wrap
                font{family: "DengXian";pixelSize: 17}
            }


            Text {
                id: contentText
                x: 24
                y: 28
                width: 282
                height: 24
                text: titleOfDialog
                font.pixelSize: 24
                font.family: "DengXian"
            }

            Button {
                id: button1
                x: cancelButtonVisible ? 24 : 24 + button1.width + 8
                y: childRect.height-64
                width: (childRect.width - 48 - 8)/2
                height: 40
                text: qsTr("确定")
                font.family: "DengXian"
                onClicked: onAccept()
            }

            Button {
                id: button2
                x: 24 + button1.width + 8
                y: childRect.height-64
                width: (childRect.width - 48 - 8)/2
                height: 40
                text: qsTr("取消")
                font.family: "DengXian"
                visible: cancelButtonVisible
                onClicked: onCancel()
            }
        }]

    //childCont.children:
}
