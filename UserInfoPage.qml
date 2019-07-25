import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import "./Components"
import "./Resources"

FramlessWindow {
    id: userInfoWindow
    objectName: "userInfoWindow"
    width: 700 * 1.2 + 40
    height: 500 * 1.2 + 40


    /* 存放两个输入框数据的结构
     * 需要说明的是，由于我自己实现了符合Fluent Design设计规范的许多控件
     * 其中输入框控件的“聚焦”操作依赖于ListView，也就需要相应的model
     */
    ListModel {
        id: updateModel
        objectName: "loginModel"
        ListElement {
            textInEdit: "regulator1"
            placeHolderText: "请输入用户名"
        }
        ListElement {
            textInEdit: "regulator1pswd"
            placeHolderText: "请输入密码"
        }
    }

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            //userInfoWindow.close();
        }
    }
    Component.onCompleted: {
        console.log("userinfo");
        usernameText.text = userPageHandler.getUsername();
        moneyText.text = userPageHandler.getMoney();
        creditText.text = userPageHandler.getCredit();
        qualiText.text = userPageHandler.getQualification();
        var resStr = userPageHandler.getMultiuserStatus();
        console.log(resStr);
        if (resStr[0] === '1') senderStatus.text = "未激活";
        else if (resStr[0] === '2') senderStatus.text = "已激活";
        else if (resStr[0] === '3') senderStatus.text = "当前用户";

        if (resStr[1] === '1') regulatorStatus.text = "未激活";
        else if (resStr[1] === '2') regulatorStatus.text = "已激活";
        else if (resStr[1] === '3') regulatorStatus.text = "当前用户";

        if (resStr[2] === '1') translatorStatus.text = "未激活";
        else if (resStr[2] === '2') translatorStatus.text = "已激活";
        else if (resStr[2] === '3') translatorStatus.text = "当前用户";
    }

    Strings{
        id: stringsPool
    }



    Component{
        id: messageDelegate
        Rectangle {
            height: 80
            width: 360
            id: mainRectInChoose

            Text {
                id: timeText
                x: 23
                y: 9
                width: 200
                height: 20
                text: messageTime
                font{
                    family: "DengXian";
                    pixelSize: 20
                }
            }

            Text {
                id: usernameText
                x: 23
                y: 36
                width: 310
                height: 32
                wrapMode: Text.WrapAnywhere
                clip: true
                maximumLineCount: 2
                text: messageContent
                font{
                    family: "DengXian";
                    pixelSize: 15
                }
            }
        }
    }


    visible: true

    childCont.children: [
        Rectangle {
            id: userRect
            height: 500 * 1.2
            width: 400 * 1.2
            color: "#000000"
            Image {
                height: 500 * 1.2
                width: 400 * 1.2
                sourceSize.height: 500 * 1.2
                sourceSize.width: 400 * 1.2
                source: "Resources/userinfo.svg"
            }

            Text {
                id: usernameText
                x: 68 * 1.2
                y: 72 * 1.2
                width: 147 * 1.2
                height: 16 * 1.2
                text: qsTr("")
                color: stringsPool.textGray1
                font{
                    family: "DengXian";
                    pixelSize: 20;
                    bold: true
                }
            }

            Rectangle{
                x: 63 * 1.2
                y: 195 * 1.2
                z: 1
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text {
                    id: moneyText
                    text: qsTr("3,600")
                    color: stringsPool.textGray1
                    font{
                        family: "DengXian";
                        pixelSize: 17;
                        bold: true
                    }
                    anchors.centerIn: parent
                }
            }

            Rectangle{
                x: 177 * 1.2
                y: 195 * 1.2
                z: 1
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text {
                    id: creditText
                    text: qsTr("13")
                    color: stringsPool.textGray1
                    font{family: "DengXian";pixelSize: 17;bold: true}
                    anchors.centerIn: parent
                }
            }

            Rectangle {
                z: 1
                x: 260 * 1.2
                y: 195 * 1.2
                width: 105 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: qualiText
                    text: qsTr("英语专业八级")
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 16;bold: true}
                }
            }


            Rectangle {
                z: 1
                x: 50 * 1.2
                y: 287 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: senderStatus
                    text: qsTr("当前用户")
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }

            Rectangle {
                z: 1
                x: 136 * 1.2
                y: 287 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: regulatorStatus
                    text: qsTr("已注册")
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }

            Rectangle {
                z: 1
                x: 220 * 1.2
                y: 287 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: translatorStatus
                    text: qsTr("已激活")
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }
            Rectangle {
                z: 1
                x: 305 * 1.2
                y: 287 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: supervisorStatus
                    text: qsTr("无权限")
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }

            Button {
                id: button1
                x: 29 * 1.2
                y: 442 * 1.2
                width: 168 * 1.2
                height: 34 * 1.2
                text: qsTr("保存信息")
                onClicked: {

                }
                font.family: "DengXian"
                highlighted: true
                Universal.foreground: "#ffffff"
            }

            Button {
                id: button2
                x: 203 * 1.2
                y: 442 * 1.2
                width: 168 * 1.2
                height: 34 * 1.2
                text: qsTr("充值")
                onClicked: {

                }
                font.family: "DengXian"
            }

            Component{
                id: updateDelegate
                AcrylicBlock {
                    id: editBox
                    property int indexOfThisDelegate: index
                    property string placeholderText: placeHolderText
                    property var view: ListView.view
                    outsideColor: "#66000000"
                    hoverBorderColor: "#9A000000"

                    default property bool thisIsSelected: ListView.isCurrentItem
                    borderWidth: 2
                    height: 38 * 1.2
                    width: 342 * 1.2

                    color: "#fdfdfd"
                    Behavior on color {
                        ColorAnimation { duration: 100 }
                    }
                    onThisIsSelectedChanged: {
                        selected=ListView.isCurrentItem;
                        console.log(indexOfThisDelegate+" is "+selected);
                    }

                    MouseArea {
                        cursorShape: Qt.IBeamCursor
                        id: dragArea
                        height: parent.height
                        width: parent.width
                        onPressed: {
                            editBox.changeStatus(1);
                            view.currentIndex=indexOfThisDelegate;
                            console.log("p"+loginList.currentIndex);
                            mouse.accepted=false;
                        }
                        onReleased: {
                            editBox.changeStatus(2);
                            mouse.accepted=false;
                        }
                        hoverEnabled: true
                        onEntered: {
                            console.log("en");
                            editBox.changeStatus(3);
                        }
                        onExited: {
                            console.log("ex");
                            editBox.changeStatus(4);
                        }
                    }

                    childCont.children:
                        TextEdit {
                        selectByMouse: true
                        verticalAlignment: TextEdit.AlignVCenter
                        id: editName
                        text: textInEdit
                        onTextChanged: {
                            textInEdit=text
                        }
                        anchors.fill: parent
                        anchors.margins: 5
                        anchors.leftMargin: 10
                        font.family: "DengXian"
                        font.pixelSize: 14 * 1.2
                        Text {
                            text: editBox.placeholderText
                            color: stringsPool.textGray3
                            visible: !editName.text
                            height: parent.height
                            verticalAlignment: TextEdit.AlignVCenter
                            font{family: "DengXian"}
                            font.pixelSize: 14 * 1.2
                        }
                    }


                }
            }

            ListView {
                id: loginList
                height: 85 * 1.2
                width: 342 * 1.2
                x: 30 * 1.2
                y: 340 * 1.2
                model: updateModel
                delegate: updateDelegate
                spacing: 9 * 1.2
                cacheBuffer: 50
                currentIndex: -1

            }

        }, Rectangle{
            x: 400 * 1.2
            width: 300 * 1.2
            height: 500 * 1.2
            color: "#ffffff"

            Text {
                x: 23
                y: 30
                width: 157 * 1.2
                height: 19 * 1.2
                text: qsTr("消息盒子")
                color: stringsPool.textGray1
                font{family: "DengXian";pixelSize: 24*1.2;bold: true}
            }
            ListView{
                y: 56 * 1.2
                width: 360
                clip: true
                height: parent.height-y
                delegate: messageDelegate
                model: messageList
            }
        }

    ]
}
