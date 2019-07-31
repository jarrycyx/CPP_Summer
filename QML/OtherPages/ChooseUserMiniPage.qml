import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

import "../MyWidgets"
import "../../Resources"

FramlessWindow {
    id: chooseRegulatorWindow
    objectName: "chooseRegulatorWindow"
    width: 450 + 40
    height: 500 + 40
    x: 700
    y: 500
    title: qsTr("选择用户")

    Strings{
        id: stringsPool
    }

    Text {
        id: hintTextNobody
        visible: true
        x: (chooseRegulatorWindow.width - width )/2
        y: (chooseRegulatorWindow.height - height )/2
        color: stringsPool.textGray1
        text: qsTr("还没有人报名")
        font{
            family: "DengXian";
            pixelSize: 14
        }
    }


    onActiveChanged: {
        if (!active) {
            chooseRegulatorWindow.close();
        }
    }

    childCont.children: [
        Rectangle {
            width: 450
            height: 500
            color: "#f2f2f2"
            visible: true

            ChooseUserDelegate {
                id: chooseUserDelegate
            }

            ListView {
                id: chooseRegulatorList
                anchors.fill: parent
                delegate: chooseUserDelegate
                model: userListModel
                cacheBuffer: 50
            }
        }
    ]

}
