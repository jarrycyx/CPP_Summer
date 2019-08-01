//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
文件名：    ThisUserArticleBlock
功能：      实现Fluent Design效果的中间层控件，使用了AcrylicBlock的封装
日期：      20190710 基本实现
           20190711 实现拖动效果
           20190715 分拆成Mini，This，Other三个控件，用于分离调用
************************************************************************************************************************/


import QtQuick 2.0
import QtGraphicalEffects 1.12
import "../../Resources"
import "../OtherPages"

Component {
    //id: dragDelegate
    MouseArea {
        Strings{id: stringsPool}
        //FontLoader { id: pingfangFont; source: "../../Resources/PingFang Regular.ttf" }

        z: 2
        id: dragArea
        property bool held: false
        property bool released: false
        property bool movedToTarget: false
        property int indexOfThisDelegate: index
        default property bool selected: GridView.isCurrentItem//ListView.isCurrentItem
        property int codeOfThisBlock: statusCodeOfArticle

        onCodeOfThisBlockChanged: {
            console.log("changed "+codeOfThisBlock+" "+held);

            newEditor.refreshEdit(articleId, titleOfArticle, contentOfArticle,
                                  translatedTitle, translatedContent,
                                  statusCodeOfArticle, indexOfThisDelegate, typeOfList);
        }
        //anchors { left: content.left; right: content.right }
        //anchors.centerIn: parent
        height: content.height+45
        width: content.width+30

        signal selectedIndexChange(int idx)

        drag.axis: typeOfList===1 ? Drag.XAndYAxis : Drag.None
        drag.target: content

        onPressed: {
            held = true;
            released = false;
            console.log("p");
            content.changeStatus(content.pressed);
        }



        HintDialog {
            id: warningBox
            visible: false
            titleOfDialog: "警告"
            contentOfDialog: "确定要删除吗？"
            cancelButtonVisible: true
            function onAccept() {
                senderPageHandler.deleteSenderArticle(indexOfThisDelegate);
                close();
            }
            function onCancel() {
                close();
            }
        }

        onReleased: {
            if (held) released=true;
            held = false;
            if (dragArea.mouseX>1600) {
                movedToTarget=true;
                if (typeof senderPageHandler !== "undefined"){
                    warningBox.visible=true;
                }
            }
            else movedToTarget=false;

            console.log("r"+ListView.delegate+ListView.flag);

            if (typeof senderArticlesList!=='undefined')
                senderArticlesList.currentIndex=indexOfThisDelegate;
            if (typeof otherArticlesList!=='undefined')
                otherArticlesList.currentIndex=-1;
            if (typeof senderSubarticlesList!=='undefined')
                senderSubarticlesList.currentIndex=-1;

            content.changeStatus(content.released);
            mainWindow.foldList();

            foldTimer.running=true;//开始计时
        }
        //文章列表折叠后，选中指定文章，防止错乱
        //目前也找不到更好的解决这个BUG的方法了
        Timer {
            id: foldTimer
            interval: 81; running: false; repeat: false
            onTriggered:
                newEditor.editOrViewAnArticle(articleId, titleOfArticle, contentOfArticle,
                                              translatedTitle, translatedContent,
                                              statusCodeOfArticle, indexOfThisDelegate, typeOfList);
        }
        onSelectedChanged: {
            content.selected=GridView.isCurrentItem;
            console.log(indexOfThisDelegate+" is "+selected);
        }
        hoverEnabled: true

        property int lastX: -1

        onHeldChanged: {
            if (!held) {
                content.layer.enabled = false;
                dragTargetImage.visible = false;
            }
        }

        onMouseXChanged: {
            content.chaseLight(mouseX,-1);
            if (held) {
                if (lastX != -1 && (mouseX - lastX > 1 || mouseX - lastX < -1)){
                    content.layer.enabled = true;
                    if (typeof dragTargetImage!=='undefined')
                        if (typeof senderPageHandler!=='undefined'){
                            dragTargetImage.visible = true;
                            if (mouseX > 1600)
                                dragTargetImage.imageSource = "../../Resources/deletedrag.svg";
                            else
                                dragTargetImage.imageSource = "../../Resources/delete.svg";
                        }
                }
            }else {
                if (typeof dragTargetImage!=='undefined')
                    dragTargetImage.visible = false;
                content.layer.enabled = false;
            }
            lastX = mouseX;
        }
        onMouseYChanged: {
            content.chaseLight(-1,mouseY);
        }
        onEntered: {
            console.log("en");
            content.changeStatus(content.hovered);
        }
        onExited: {
            console.log("ex");
            content.changeStatus(content.none);
        }

        AcrylicBlock {
            id: content
            height: 197
            width: 305
            outsideColor: "#00f2f2f2"
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }

            layer.enabled: false
            layer.effect: DropShadow {
                id: acrylicBlockShadow
                radius: 15
                visible: false
                samples: 17
                z: 50
                color: "#66999999"
            }

            color: "#fdfdfd"//dragArea.held ? "#bbbbbb" : "#dddddd"
            Behavior on color {
                ColorAnimation { duration: 100 }
            }
            states: [State {
                    when: dragArea.held
                    ParentChange { target: content; parent: root }
                    AnchorChanges {
                        target: content
                        anchors { horizontalCenter: undefined; verticalCenter: undefined }
                    }
                },State {
                    when: dragArea.released&&dragArea.movedToTarget
                    //ParentChange { target: content; parent: rect }
                }]
            childCont.children:  [
                Image {
                    id: borderImage
                    x: 112
                    y: 32
                    width: 80
                    height: 80
                    sourceSize.width: 80
                    sourceSize.height: 80
                    source: "../../Resources/status-code/"+statusCodeOfArticle+".svg"
                },
                Text{
                    id: titleText
                    text: titleOfArticle+"/"+statusCodeOfArticle
                    x: 21
                    y: 127
                    width: 261
                    elide: Text.ElideRight
                    height: 14
                    color: stringsPool.textGray1
                    font {
                        family: "DengXian"
                        pixelSize: 20
                    }
                },
                Text{
                    id: previewText
                    text: contentOfArticle
                    x: 21
                    y: 150
                    width: 261
                    height: 24
                    wrapMode: Text.WrapAnywhere
                    color: stringsPool.textGray3
                    maximumLineCount: 1
                    verticalAlignment: Text.AlignBottom
                    font{
                        weight: Font.Light
                        family: "DengXian"//pingfangFont.name
                        bold: false
                        pixelSize: 12
                    }
                }
            ]
        }

    }

}
