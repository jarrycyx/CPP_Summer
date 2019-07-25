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
    height: 478 * 1.2 + 40

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            //userInfoWindow.close();
        }
    }
    Component.onCompleted: {
        console.log("userinfo");
        usernameText.text = userPageHandler.getUsername();
    }

    Strings{
        id: stringsPool
    }



    Component{
        id: messageDelegate
        Rectangle {
            height: 72
            width: 360
            id: mainRectInChoose

            Text {
                id: timeText
                x: 23
                y: 13
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
                y: 47
                width: 310
                height: 14
                maximumLineCount: 1
                text: messageContent
                font{
                    family: "DengXian";
                    pixelSize: 14
                }
            }
        }
    }


    visible: true

    childCont.children: [
        Rectangle {
            id: userRect
            height: 478 * 1.2
            width: 400 * 1.2
            color: "#000000"
            Image {
                height: 478 * 1.2
                width: 400 * 1.2
                sourceSize.height: 478 * 1.2
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
                font{family: "DengXian";pixelSize: 20;bold: true}
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
                    font{family: "DengXian";pixelSize: 17;bold: true}
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
                y: 422 * 1.2
                width: 168 * 1.2
                height: 32 * 1.2
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
                y: 422 * 1.2
                width: 168 * 1.2
                height: 32 * 1.2
                text: qsTr("充值")
                onClicked: {

                }
                font.family: "DengXian"
            }

        }, Rectangle{
            x: 400 * 1.2
            width: 300 * 1.2
            height: 478 * 1.2
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

    //childCont.children:
}
