//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
名称：     TranslatorEditor
功能：     上层控件，翻译者页面查看、修改文章状态的组件
日期：     20190723 实现基本功能
************************************************************************************************************************/


import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12
import "../MyWidgets"
import "../../Resources"


Rectangle {
    property int mode: 0 //0: new 1:edit/view
    property int articleStatus: 0
    property int indexInList: -1

    property string thisTitle
    property string thisContent
    property string thisTrTitle
    property string thisTrContent
    property int typeOfThis
    property int idOfThis

    id: translatorEditorRect


    function refreshEdit(id, title, content, translatedTitle,
                         translatedContent, statusCode, index, typeOfArticle){
        idOfThis=id;thisTitle=title;
        thisContent=content;
        thisTrTitle=translatedTitle;
        thisTrContent=translatedContent;

        button3.text="查看原文";
        mode=1;
        titleEdit.text=title;
        contentEdit.text=content;
        articleStatus=statusCode;
        indexInList=index;
        typeOfThis=typeOfArticle;

        //针对不同状态号来定制组件
        switch (translatorEditorRect.articleStatus){
        case 120:
            statusText.text="正在招募译者";
            element.text="我负责的翻译需求";
            button.text="报名";
            button.enabled=true;
            button2.visible=false;
            element.text="翻译需求详情";
            button3.visible=false;
            button4.visible=false;
            break;
        case 130:
            statusText.text="招募译者结束，即将拆分";
            button.text="退出报名";
            button.enabled=false;
            button2.visible=false;
            element.text="我负责的翻译需求";
            button3.visible=false;
            button4.visible=false;
            break;
        case 140:
            statusText.text="已被拆分，等待分配子任务";
            button.text="已拆分";
            button.enabled=false;
            button2.visible=false;
            element.text="我负责的翻译需求（已拆分为子任务）";
            button3.visible=false;
            button4.visible=false;
            break;
        case 210:
        case 215:
        case 220://三种状态相同处理
            statusText.text="已分配译者,正在翻译";
            button.text="上传";
            button.enabled=true;
            button2.visible=false;
            element.text="我负责的子任务";
            button3.visible=true;
            button4.visible=true;
            titleEdit.text=translatedTitle;
            contentEdit.text=translatedContent;
            break;
        case 230:
            statusText.text="子任务已完成，审核通过";
            button.text="上传"
            button.enabled=false;
            button2.visible=false;
            element.text="我负责的子任务";
            button3.visible=true;
            button4.visible=true;
            titleEdit.text=translatedTitle;
            contentEdit.text=translatedContent;
            break;
        case 240:
            statusText.text="子任务已合并，任务完成，等待发放酬金";
            button.text="上传"
            button.enabled=false;
            button2.visible=false;
            element.text="我负责的子任务";
            button3.visible=true;
            button4.visible=true;
            titleEdit.text=translatedTitle;
            contentEdit.text=translatedContent;
            break;
        }

        if (typeOfArticle===2){//如果是查看其他文章
            element.text="查看需求";
            button.enabled=true;//允许报名
            button2.enabled=false;
            titleEdit.enabled=false;
            contentEdit.enabled=false;
        }else {
            button2.enabled=true;
            titleEdit.enabled=true;
            contentEdit.enabled=true;
        }
    }

    function editOrViewAnArticle(id, title, content, translatedTitle,
                                 translatedContent, statusCode, index, typeOfArticle){
        visible=true;
        blankText.visible=false;
        refreshEdit(id, title, content, translatedTitle,
                    translatedContent, statusCode, index, typeOfArticle);
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
        font{
            family: "DengXian";
            pixelSize: 20
        }
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
        y: parent.height-13-40
        height: 40
        width: 120
        text: qsTr("报名")
        font{family: "DengXian"}
        onClicked: {
            switch (translatorEditorRect.articleStatus){
            case 120:
                translatorPageHandler.signForTranslatorArticle(indexInList);
                break;
            case 210:
            case 215:
            case 220://三种状态相同操作
                if (button3.text === "查看原文")//当前显示的是译文
                    translatorPageHandler.editTranslatedArticle(indexInList, titleEdit.text, contentEdit.text);
                break;
            }
        }

        highlighted: true
        Universal.foreground: "#ffffff"
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
            switch (translatorEditorRect.articleStatus){
            }

        }
    }

    Text {
        x: button2.visible ? 275 : 142
        y: translatorEditorRect.height - 25 - 16
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
        y: translatorEditorRect.height - 25 - 16
        width: 200
        height: 16
        text: qsTr("正在招募译者")
        font{
            bold: true;
            family: "DengXian";
            pixelSize: 16
        }
    }

    Button {
        id: button3
        x: parent.width-5-120
        y: parent.height-13-40
        visible: false;
        width: 120
        height: 40
        text: qsTr("查看原文")
        font.family: "DengXian"
        onClicked: {
            if (text==="查看原文"){
                thisTrTitle=titleEdit.text;
                thisTrContent=contentEdit.text;
                titleEdit.text=thisTitle;
                contentEdit.text=thisContent;
                titleEdit.enabled=false;
                contentEdit.enabled=false;
                text="查看译文";
            } else {
                titleEdit.text=thisTrTitle;
                contentEdit.text=thisTrContent;
                titleEdit.enabled=true;
                contentEdit.enabled=true;
                text="查看原文";
            }
        }
    }


    Button {
        id: button4
        x: parent.width-5-120-11-120
        y: parent.height-13-40
        visible: false;
        width: 120
        height: 40
        text: qsTr("查看反馈")
        font.family: "DengXian"
        onClicked: {
            var commentWindow=Qt.createComponent("../OtherPages/TranslatorCommentDialog.qml")
                                .createObject(RegulatorViewer)
            commentWindow.currentArticleIndex=indexInList;
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
            translatorPageHandler.showArticleInfo(idOfThis);
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
