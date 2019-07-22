import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import QtQuick.Controls.Universal 2.12

import "../Resources"


Rectangle {
    property int mode: 0 //0: new 1:edit/view
    property int articleStatus: 0
    property int indexInList: -1
    id: senderEditorRect

    function editOrViewAnArticle(title, content, statusCode, index){
        mode=1;
        blankText.visible=false;
        visible=true;
        titleEdit.text=title;
        contentEdit.text=content;
        articleStatus=statusCode;
        indexInList=index;

        switch (senderEditorRect.articleStatus){
        case 100:
            statusText.text="已上传，招募负责人开始";
            element.text="翻译需求详情";
            button.text="报名";
            button2.visible=false;
            break;
        case 110:
            statusText.text="已标记负责人，招募负责人结束";
            element.text="我负责的翻译需求";
            button.text="保存";
            button2.visible=true;
            button2.text="开始招募译者";
            break;
        case 120:
            statusText.text="开始招募译者";
            element.text="我负责的翻译需求";
            button.text="停止招募";
            button2.visible=false;
        case 130:
            statusText.text="已进行拆分，招募译者结束";
        }
    }

    Strings{id: stringsPool}

    Text {
        id: element
        x: 5
        y: 2
        width: 224
        height: 28
        text: qsTr("翻译需求详情")
        color: stringsPool.textGray1
        font{
            bold: true
            family: "DengXian";
            pixelSize: 28
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
        y: parent.height-13-40
        height: 40
        width: 120
        text: qsTr("报名")
        font{family: "DengXian"}
        onClicked: {
            switch (senderEditorRect.articleStatus){
            case 100:
                regulatorPageHandler.signForRegulatorArticle(indexInList, titleEdit.text, contentEdit.text)
                break;
            case 110:
                regulatorPageHandler.editArticle(indexInList, titleEdit.text, contentEdit.text)
                break;
            }
        }
    }

    Button {
        id: button2
        x: 136
        y: parent.height-13-40
        width: 120
        height: 40
        text: qsTr("选择负责人")
        font.family: "DengXian"
        onClicked: {
             switch (senderEditorRect.articleStatus){
             case 0:
             case 110:
                 regulatorPageHandler.startRecruitingTranslatorForArticle(indexInList);
                 break;
             }
         }
    }

    Text {
        x: button2.visible ? 275 : 142
        y: senderEditorRect.height - 25 - 16
        width: 74
        height: 16
        text: qsTr("当前状态:")
        font{
            family: "DengXian";
            pixelSize: 16
        }
    }

    Text {
        id: statusText
        x: button2.visible ? 349 : 216
        y: senderEditorRect.height - 25 - 16
        width: 200
        height: 16
        text: qsTr("正在招募译者")
        font{
            bold: true;
            family: "DengXian";
            pixelSize: 16
        }
    }
}
