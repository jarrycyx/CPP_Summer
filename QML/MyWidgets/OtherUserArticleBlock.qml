//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
文件名：    OtherUserArticleBlock
功能：      实现Fluent Design效果的中间层控件，使用了AcrylicBlock的封装
日期：      20190710 基本实现
           20190711 实现拖动效果
           20190715 分拆成Mini，This，Other三个控件，用于分离调用
************************************************************************************************************************/


import QtQuick 2.0
import "../../Resources"

Component {
    MouseArea {
        Strings{id: stringsPool}

        z: 2
        id: dragArea
        property bool released: false
        property bool movedToTarget: false
        property int indexOfThisDelegate: index
        default property bool selected: GridView.isCurrentItem//ListView.isCurrentItem
        property int codeOfThisBlock: statusCodeOfArticle

        onCodeOfThisBlockChanged: {
            console.log("changed "+titleOfArticle+" "+codeOfThisBlock);

            newEditor.refreshEdit(articleId, titleOfArticle, contentOfArticle,
                                          translatedTitle, translatedContent,
                                          statusCodeOfArticle, indexOfThisDelegate, typeOfList);
        }

        height: content.height+45
        width: content.width+30

        signal selectedIndexChange(int idx)

        onPressed: {
            released = false;
            console.log("p");
            content.changeStatus(content.pressed);
        }
        onReleased: {
            if (dragArea.mouseX>500) {
                movedToTarget=true;
                senderPageHandler.deleteSenderArticle(indexOfThisDelegate);
            }
            else movedToTarget=false;

            console.log("r"+ListView.delegate+ListView.flag);

            if (typeof senderArticlesList!=='undefined')
                senderArticlesList.currentIndex=-1;
            if (typeof otherArticlesList!=='undefined')
                otherArticlesList.currentIndex=indexOfThisDelegate;
            if (typeof senderSubarticlesList!=='undefined')
                senderSubarticlesList.currentIndex=-1;

            content.changeStatus(content.released);
            mainWindow.foldList();

            foldTimer.running=true;//开始计时
        }
        //文章列表折叠后，选中指定文章的计时器，防止错乱
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

        onMouseXChanged: content.chaseLight(mouseX,-1)
        onMouseYChanged: content.chaseLight(-1,mouseY)
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

            color: "#fdfdfd"
            Behavior on color {
                ColorAnimation { duration: 100 }
            }

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
