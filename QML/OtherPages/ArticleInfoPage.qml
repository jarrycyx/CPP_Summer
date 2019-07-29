import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import "../MyWidgets"
import "../../Resources"

FramlessWindow {
    id: userInfoWindow
    objectName: "userInfoWindow"
    width: 400 * 1.2 + 40
    height: 250 * 1.2 + 40
    x: 1920 - 15 - width
    y: 80
    visible: true

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            userInfoWindow.close();
        }
    }

    Strings{
        id: stringsPool
    }

    Component.onCompleted: {
        moneyText.text = articleInfoPageHandler.getMoney()+" RMB"
        senderStatus.text = articleInfoPageHandler.getSenderName()
        regulatorStatus.text = articleInfoPageHandler.getRegulatorName()
        translatorStatus.text = articleInfoPageHandler.getTranslatorName()
    }


    childCont.children: [
        Rectangle {
            id: userRect
            height: 250 * 1.2
            width: 400 * 1.2
            color: "#00000000"
            Image {
                height: 250 * 1.2
                width: 400 * 1.2
                sourceSize.height: 250 * 1.2
                sourceSize.width: 400 * 1.2
                source: "../../Resources/articleinfo.svg"
            }

            Rectangle{
                x: 127 * 1.2
                y: 52 * 1.2
                z: 1
                width: 200 * 1.2
                height: 45
                color: "#00000000"
                Text {
                    id: moneyText
                    color: stringsPool.textGray1
                    font{
                        family: "DengXian";
                        pixelSize: 45;
                    }
                    anchors.centerIn: parent
                }
            }

            Rectangle {
                z: 1
                x: 50 * 1.2
                y: 190 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: senderStatus
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }

            Rectangle {
                z: 1
                x: 136 * 1.2
                y: 190 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: regulatorStatus
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }

            Rectangle {
                z: 1
                x: 220 * 1.2
                y: 190 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: translatorStatus
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }
            Rectangle {
                z: 1
                x: 305 * 1.2
                y: 190 * 1.2
                width: 46 * 1.2
                height: 14 * 1.2
                color: "#00000000"
                Text{
                    id: supervisorStatus
                    text: qsTr("未绑定")
                    color: stringsPool.textGray1
                    anchors.centerIn: parent
                    font{family: "DengXian";pixelSize: 18;bold: true}
                }
            }
        }
    ]
}
