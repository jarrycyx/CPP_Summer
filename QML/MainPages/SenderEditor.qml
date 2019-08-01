//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
名称：     SenderEditor
功能：     上层控件，发布者页面查看、修改文章状态的组件
日期：     20190708 实现基本功能
          2019071X 扩充功能
          20190721 增加悬浮按钮和提示框
************************************************************************************************************************/


import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import "../MyWidgets"
import "../OtherPages"
import "../../Resources"

Rectangle {
    property int mode: 0 //0: new 1:edit/view
    property int articleStatus: 0
    property int indexInList: -1
    id: senderEditorRect

    property string thisTitle
    property string thisContent
    property string thisTrTitle
    property string thisTrContent
    property int typeOfThis
    property int idOfThis

   function refreshEdit(id, title, content, translatedTitle,
                         translatedContent, statusCode, index, typeOfArticle){

        idOfThis=id;
        //type: 1,自己的需求 2,别人的需求
        mode=1;
        titleEdit.text=title;
        contentEdit.text=content;
        articleStatus=statusCode;
        indexInList=index;
        element.text="修改翻译需求"


        thisTitle=title;
        thisContent=content;
        thisTrTitle=translatedTitle;
        thisTrContent=translatedContent;
        typeOfThis=typeOfArticle;

        switch (senderEditorRect.articleStatus){
        case 0:
            statusText.text="未上传";
            button.text="发送";
            button.enabled=true;
            button2.visible=true;
            element.text="我发布的任务";
            break;
        case 100:
            statusText.text="已上传，招募负责人开始";
            button.text="修改";
            button.enabled=true;
            button2.visible=true;
            button2.text="报名状态";
            element.text="我发布的任务";
            break;
        case 110:
            statusText.text="已标记负责人，招募负责人结束";
            button.text="修改";
            button.enabled=true;
            button2.visible=false;
            element.text="我发布的任务";
            break;
        case 120:
            statusText.text="开始招募译者";
            button.text="修改";
            button.enabled=true;
            button2.visible=false;
            element.text="我发布的任务";
            break;
        case 130:
            statusText.text="招募译者结束";
            button.enabled=false;
            button2.visible=false;
            element.text="我发布的任务";
            break;
        case 140:
            statusText.text="已拆分，译者紧张工作中";
            button.enabled=false;
            button2.visible=false;
            element.text="我发布的任务";
            break;
        case 300:
            statusText.text="翻译工作完成，负责人审核中";
            button.enabled=false;
            button2.visible=false;
            element.text="我发布的任务";
            break;
        case 310:
            statusText.text="翻译内容已提交";
            button.text="确认完成";
            button.enabled=true;
            button2.visible=true;
            button2.text="重新翻译";
            element.text="我发布的任务";
            break;
        case 400:
            statusText.text="确认完成，即将删除";
            button.text="确认完成";
            button.enabled=false;
            button2.visible=false;
            element.text="我发布的任务";
            break;
        }

        if (typeOfArticle===2){
            element.text="查看需求";
            button.enabled=false;
            button2.enabled=false;
            titleEdit.enabled=false;
            contentEdit.enabled=false;
            //若为浏览他人文章，则只显示部分内容
            //var contentStr = content;
            //contentEdit.text=content.length()>50 ? content.slice(0, 50) : content;
        }else {
            button2.enabled=true;
            titleEdit.enabled=true;
            contentEdit.enabled=true;
            contentEdit.text=content;
        }
    }

   function editOrViewAnArticle(id, title, content, translatedTitle,
                                translatedContent, statusCode, index, typeOfArticle){
       visible=true;
       blankText.visible=false;
       refreshEdit(id, title, content, translatedTitle,
                   translatedContent, statusCode, index, typeOfArticle);
   }

    function addAnArticle(){
        mode=0;
        blankText.visible=false;
        visible=true;
        contentEdit.text="新文章的内容，是一大段需要翻译的不知道在说什么的文字。"
                +Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss.zzz ddd");
        titleEdit.text="新文章"+Qt.formatDateTime(new Date(), "yyyy-MM-dd");
        element.text="发布翻译需求"
        senderEditorRect.articleStatus=0;
        button.text="发布"
        button.enabled=true;
        button2.visible=false;
        statusText.text="未上传";
        contentEdit.enabled=true;
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
            font{
                family: "DengXian";
                pixelSize: 17;
                wordSpacing: 4
            }
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
                var moneyWindow=Qt.createComponent("../OtherPages/DetermineMoneyDialog.qml")
                                    .createObject(SenderEditor);
                //senderEditorRect.articleStatus=100;
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
            case 310:
                senderPageHandler.confirmAcceptArticle(indexInList);
            }



        }
        highlighted: true
        Universal.foreground: "#ffffff"
    }

    HintDialog {
        id: warningReTranslateBox
        visible: false
        titleOfDialog: "对翻译不满意？"
        x: 700
        y: 500
        contentOfDialog: "将要交回负责人重新进行翻译，当前译稿作废。"
        cancelButtonVisible: true
        function onAccept() {
            senderPageHandler.reTranslate(indexInList);
            close();
        }
        function onCancel() {
            close();
        }
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
            case 310:
                warningReTranslateBox.visible = true;
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

    ToolButton {
        width: moreUserInfoText.width + 42
        height: 32
        x: parent.width-150-width
        y: 2
        z: 40
        Text {
            id: moreUserInfoText
            x: 8
            y: 9
            text: qsTr("更多信息")
            font.family: "DengXian";
        }
        onClicked: {
            senderPageHandler.showArticleInfo(idOfThis);
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

}
