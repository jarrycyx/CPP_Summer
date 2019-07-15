import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import QtQuick.Controls.Universal 2.12

import "../Resources"


Rectangle {

    Strings{id: stringsPool}

    Text {
        id: element
        x: 5
        y: 5
        width: 224
        height: 24
        text: qsTr("发布翻译需求")
        color: stringsPool.textGray1
        font{
            family: "DengXian";
            pixelSize: 24
        }
    }

    Rectangle {
        color: stringsPool.textGray3
        x:5
        y:46
        width:parent.width-10
        height: 1
    }
    Text {
        x: 5
        y: 60
        width: 55
        height: 18
        color: stringsPool.textGray2
        text: qsTr("标题：")
        font{family: "DengXian";pixelSize: 20}
    }

    TextEdit {
        id: titleEdit
        x: 60
        y: 60
        width: parent.width-10
        height: 18
        selectByMouse: true
        font{family: "DengXian";pixelSize: 20}
        property string placeholderText: "在此输入标题"
        Text {
            text: titleEdit.placeholderText
            color: stringsPool.textGray3
            visible: !titleEdit.text
            font{family: "DengXian";pixelSize: 20}
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor
            onPressed: mouse.accepted=false
            onReleased:  mouse.accepted=false
        }
    }

    Rectangle {
        color: stringsPool.textGray3
        x:5
        y:95
        width:parent.width-10
        height: 1
    }


    Flickable {
         id: flick
         x: 5
         y: 120
         width: parent.width-10
         height: parent.height - 227
         contentWidth: contentEdit.paintedWidth
         contentHeight: contentEdit.paintedHeight
         clip: true

         function ensureVisible(r)
         {
             if (contentX >= r.x)
                 contentX = r.x;
             else if (contentX+width <= r.x+r.width)
                 contentX = r.x+r.width-width;
             if (contentY >= r.y)
                 contentY = r.y;
             else if (contentY+height <= r.y+r.height)
                 contentY = r.y+r.height-height;
         }

         TextEdit {
             id: contentEdit
             width: flick.width
             height: flick.height
             focus: true
             wrapMode: TextEdit.Wrap
             onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
             selectByMouse: true
             font{family: "DengXian";pixelSize: 17}
             property string placeholderText: "在此输入内容"
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
     }
    Rectangle {
        color: stringsPool.textGray3
        x:5
        y:parent.height-77
        width:parent.width-10
        height: 1
    }

    Button {
        id: button
        x: 5
        y: parent.height-17-40
        height: 40
        width: 120
        text: qsTr("选择负责人")
        font{family: "DengXian"}
    }

    CheckBox {
        id: checkBox
        x: parent.width-5-width
        y: parent.height-17-height
        height:40
        text: qsTr("可被拆分")
        font{family: "DengXian"}
    }


}
