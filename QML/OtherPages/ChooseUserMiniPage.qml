import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

import "../MyWidgets"
import "../../Resources"

ApplicationWindow {
    id: chooseRegulatorWindow
    objectName: "chooseRegulatorWindow"
    width: 450
    height: 500

    Strings{
        id: stringsPool
    }

    title: qsTr("选择负责人")
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
