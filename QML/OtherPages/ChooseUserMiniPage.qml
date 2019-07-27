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
    title: qsTr("选择负责人")

    Strings{
        id: stringsPool
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

            //childCont.children:
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
