import QtQuick 2.12
import QtQuick.Controls 2.5

import qt.cpp.ProcessingModel 1.0
import qt.cpp.QProcessClass 1.0


Page {


    width: 600
    height: 400
    id: homePage
    objectName: "homePage"
    title: qsTr("Home")

    QProcessClass{
        id: myCppProcess
        objectName: "myCppProcess"
    }



    Label {
        id: newText1
        objectName: "newText1"
        text: qsTr("测试的字符串")
        anchors.verticalCenterOffset: 50
        anchors.horizontalCenterOffset: 5
        anchors.centerIn: parent
        signal signal1(string str)
        onSignal1: newText1.text=str+"label receive"
    }

    Button {
        id: button1
        objectName: "button1"
        x: 275
        y: 176
        height: 30
        width: 60
        text: qsTr("Button")
        onClicked: {
            text=myCppProcess.qmlToCppTest(2);
        }

    }

    Connections {
        target: myCppProcess
        onSignal1:
            console.log(qstr)
    }

    CheckBox {
        id: checkBox
        x: 251
        y: 275
        text: qsTr("Check Box")
    }

    CheckBox {
        id: checkBox1
        x: 251
        y: 313
        text: qsTr("Check Box")
    }

    BusyIndicator {
        id: busyIndicator
        x: 285
        y: 113
        width: 40
        height: 40
    }

    RangeSlider {
        id: rangeSlider
        x: 200
        y: 71
        second.value: 0.75
        first.value: 0.25
    }



    function slotQml1(){
        console.log("test");
    }
}
