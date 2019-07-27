import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import "../MyWidgets"
import "../../Resources"

FramlessWindow {
    id: dialogWindow
    objectName: "chooseRegulatorWindow"
    width: 300 + 40
    height: 200 + 40
    x: 550
    y: 800

    property int currentArticleIndex: -1

    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            dialogWindow.close();
        }
    }

    onCurrentArticleIndexChanged: {
        contentEdit.text = Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss");
        contentEdit.text += ": \n\t"
        console.log(Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss.zzz ddd"));
    }


    Strings{
        id: stringsPool
    }

    visible: true

    childCont.children: [
        Rectangle{
            height: dialogWindow.height - 40
            width: dialogWindow.width - 40
            clip: true

            SpinBox {
                id: moneySpinBox
                editable: true
                x: (dialogWindow.width - 40 - width) / 2
                y: 24 + 28 + 24
                value: 500
                from: 100
                to: 5000
                stepSize: 100

            }


            Text {
                id: element
                x: 24
                y: 28
                width: 282
                height: 24
                text: qsTr("设置翻译任务酬金")
                font.pixelSize: 24
                font.family: "DengXian"
            }

            Button {
                id: button
                x: 24
                y: parent.height - 64
                width: dialogWindow.width - 40 - 48
                height: 40
                text: qsTr("确定")
                font.family: "DengXian"
                onClicked: {
                    senderPageHandler.addSenderArticle(titleEdit.text, contentEdit.text, moneySpinBox.value);
                    dialogWindow.close();
                }
            }
        }]

    //childCont.children:
}
