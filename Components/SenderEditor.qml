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

    function editOrViewAnArticle(title, content, translatedTitle, translatedContent, statusCode, index, typeOfArticle){
        //type: 1,自己的需求 2,别人的需求
        mode=1;
        blankText.visible=false;
        visible=true;
        titleEdit.text=title;
        contentEdit.text=content;
        articleStatus=statusCode;
        indexInList=index;
        element.text="修改翻译需求"

        switch (senderEditorRect.articleStatus){
        case 0:
            statusText.text="未上传";
            button.text="发送";
            button2.visible=true;
            break;
        case 100:
            statusText.text="已上传，招募负责人开始";
            button.text="修改";
            button2.visible=true;
            button2.text="报名状态";
            break;
        case 110:
            statusText.text="已标记负责人，招募负责人结束";
            button.text="修改";
            button2.visible=false;
            break;
        case 120:
            statusText.text="开始招募译者";
            button.text="修改";
            button2.visible=false;
            break;
        case 130:
            statusText.text="招募译者结束";
            break;
        case 140:
            statusText.text="已拆分";
            button.enabled=false;
            button2.visible=false;
            break;
        }

        if (typeOfArticle===2){
            element.text="查看需求";
            button.enabled=false;
            button2.enabled=false;
            titleEdit.enabled=false;
            contentEdit.enabled=false;
        }else {
            button.enabled=true;
            button2.enabled=true;
            titleEdit.enabled=true;
            contentEdit.enabled=true;
        }
    }

    function addAnArticle(){
        mode=0;
        blankText.visible=false;
        visible=true;
        contentEdit.text="新文章的内容，是一大段需要翻译的不知道在说什么的文字。"+Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss.zzz ddd");
        titleEdit.text="新文章"+Qt.formatDateTime(new Date(), "yyyy-MM-dd");
        element.text="发布翻译需求"
        senderEditorRect.articleStatus=0;
        button.text="发布"
        statusText.text="未上传";
    }

    Strings{id: stringsPool}

    Text {
        id: element
        x: 5
        y: 2
        width: 224
        height: 28
        text: qsTr("发布翻译需求")
        color: stringsPool.textGray1
        font{
            family: "DengXian";
            pixelSize: 28;
            bold: true;
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
        text: qsTr("上传")
        font{family: "DengXian"}
        onClicked: {
            switch (senderEditorRect.articleStatus){
            case 0:
                senderPageHandler.addSenderArticle(titleEdit.text, contentEdit.text);
                senderEditorRect.articleStatus=100;
                break;
            case 100:
                senderPageHandler.editSenderArticle(indexInList, titleEdit.text, contentEdit.text)
                break;
            case 110:
                senderPageHandler.editSenderArticle(indexInList, titleEdit.text, contentEdit.text)
                break;
            case 120:
                senderPageHandler.editSenderArticle(indexInList, titleEdit.text, contentEdit.text)
                break;
            case 130:
                senderPageHandler.editSenderArticle(indexInList, titleEdit.text, contentEdit.text)
                break;
            case 140:
                break;
            }
        }
        highlighted: true
        Universal.foreground: "#ffffff"
    }

    Button {
        id: button2
        x: 136
        y: parent.height-17-40
        width: 120
        height: 40
        text: qsTr("选择负责人")
        font.family: "DengXian"
        onClicked: {
            switch (senderEditorRect.articleStatus){
            case 0:
            case 100:
                senderPageHandler.chooseRegulator(indexInList);
                break;
            }
        }
    }

    CheckBox {
        id: checkBox
        x: parent.width-5-width
        y: parent.height-17-height
        height:40
        text: qsTr("可被拆分")
        font{family: "DengXian"}
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
