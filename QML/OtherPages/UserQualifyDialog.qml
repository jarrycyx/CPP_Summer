import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

import "../MyWidgets"
import "../../Resources"

FramlessWindow {
    id: userQualiWindow
    objectName: "chooseRegulatorWindow"
    width: 300 + 40
    height: 200 + 40
    title: "提示"
    x: 1200
    y: 400


    //用户点击窗口外则关闭窗口
    onActiveChanged: {
        if (!active) {
            userQualiWindow.close();
        }
    }

    Strings{
        id: stringsPool
    }

    visible: true

    childCont.children: [
        Rectangle{
            height: userQualiWindow.height - 40
            width: userQualiWindow.width - 40
            clip: true

            SpinBox {
                id: moneySpinbox
                from: 0
                to: items.length - 1
                value: 1 // "Medium"
                x: (userQualiWindow.width - 40 - width) / 2
                y: 24 + 28 + 24
                property var items: ["TOFEL 90分", "TOFEL 100分",
                                    "TOFEL 110分", "英语六级",
                                    "英语专业八级"]
                validator: RegExpValidator {
                    regExp: new RegExp(
                                "(TOFEL 90分|TOFEL 100分|TOFEL 110分|英语六级|英语专业八级)",
                                "i")
                }
                textFromValue: function(value) {
                    return items[value];
                }
                valueFromText: function(text) {
                    for (var i = 0; i < items.length; ++i) {
                        if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                            return i
                    }
                    return sb.value
                }
            }


            Text {
                id: element
                x: 24
                y: 28
                width: 282
                height: 24
                text: qsTr("用户资质证明（仅演示）")
                font.pixelSize: 24
                font.family: "DengXian"
            }

            Button {
                id: button
                x: 24
                y: parent.height - 64
                width: userQualiWindow.width - 40 - 48
                height: 40
                text: qsTr("确定")
                font.family: "DengXian"
                onClicked: {
                    loginPageHandler.setUserQualification(moneySpinbox.textFromValue(moneySpinbox.value));
                    userQualiWindow.close();
                }
            }
        }]
}
