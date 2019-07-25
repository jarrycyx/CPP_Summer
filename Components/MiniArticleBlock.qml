//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
文件名：    MiniArticleBlock
功能：      实现Fluent Design效果的中间层控件，使用了AcrylicBlock的封装
日期：      20190710 基本实现
           20190711 实现拖动效果
           20190715 分拆成Mini，This，Other三个控件，用于分离调用
************************************************************************************************************************/


import QtQuick 2.0
import "../Resources"

Component {
    MouseArea {
        Strings{id: stringsPool}
        z: 2
        id: dragArea
        property bool held: false
        property bool released: false
        property bool movedToTarget: false
        property int indexOfThisDelegate: index
        default property bool selected: GridView.isCurrentItem//ListView.isCurrentItem

        //计算边距
        height: content.height+45
        width: content.width+30

        signal selectedIndexChange(int idx)

        //定义拖拽方向和对象
        drag.axis: typeOfList===1 ? Drag.XAndYAxis : Drag.None
        drag.target: content

        onPressed: {
            held = true;
            released = false;
            console.log("p");
            //通知AcrylicBlock改变状态
            content.changeStatus(content.pressed);
        }
        onReleased: {
            if (held) released=true;
            held = false;
            if (dragArea.mouseX>500) {
                movedToTarget=true;
                senderPageHandler.deleteSenderArticle(indexOfThisDelegate);
            }
            else movedToTarget=false;

            console.log("r"+ListView.delegate+ListView.flag);

            //为了能够被多处调用，通过判断是否存在来调用不同来源的类似方法
            if (typeof senderArticlesList!=='undefined')
                senderArticlesList.currentIndex=-1;
            if (typeof otherArticlesList!=='undefined')
                otherArticlesList.currentIndex=-1;
            if (typeof senderSubarticlesList!=='undefined')
                senderSubarticlesList.currentIndex=indexOfThisDelegate;

            //通知AcrylicBlock改变状态
            content.changeStatus(content.released);
            newEditor.editOrViewAnArticle(titleOfArticle, contentOfArticle, translatedTitle, translatedContent,
                                          statusCodeOfArticle, indexOfThisDelegate, typeOfList);
            mainWindow.foldList();

        }
        onSelectedChanged: {
            //更新选中状态
            //通知AcrylicBlock组件进行高亮显示
            content.selected=GridView.isCurrentItem;
            console.log(indexOfThisDelegate+" is "+selected);
        }
        hoverEnabled: true

        //通知AcrylicBlock进行追光
        onMouseXChanged: content.chaseLight(mouseX,-1)
        onMouseYChanged: content.chaseLight(-1,mouseY)
        onEntered: {
            console.log("en");
            //通知AcrylicBlock改变状态
            content.changeStatus(content.hovered);
        }
        onExited: {
            console.log("ex");
            //通知AcrylicBlock改变状态
            content.changeStatus(content.none);
        }



        AcrylicBlock {
            id: content
            height: 76
            width: 305
            outsideColor: "#00f2f2f2"
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
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
                    x: 12
                    y: 13
                    width: 45
                    height: 45
                    sourceSize.width: 45
                    sourceSize.height: 45
                    source: "../Resources/status-code/"+statusCodeOfArticle+".svg"
                },
                Text{
                    id: titleText
                    text: titleOfArticle+"/"+statusCodeOfArticle
                    x: 73
                    y: 16
                    width: 210
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
                    x: 73
                    y: 33
                    width: 210
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
