import QtQuick 2.12
import QtQuick.Controls 2.5

import qt.cpp.ProcessingModel 1.0
import qt.cpp.QProcessClass 1.0

import QtQuick.Layouts 1.13


Page {


    width: 1280
    height: 720
    //background: "#eeeeee"
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

    Switch {
        id: element
        x: 360
        y: 176
        text: qsTr("Switch")
    }

    Tumbler {
        id: tumbler
        x: 106
        y: 91
        model: 10
    }


    Rectangle{

        width: 300
        height: 500
        x: 966
        y: 197
        color: "#eeeeee"
        MouseArea {
            id: button
            x: 50
            y: 150
            width: 200
            height: 200
            anchors.horizontalCenter: parent
            anchors.verticalCenter: parent
            hoverEnabled: true

            onMouseXChanged: myLightBlock.chaseLight(mouseX,-1)
            onMouseYChanged: myLightBlock.chaseLight(-1,mouseY)

            onPressed: {
                console.log("p");
                myLightBlock.changeStatus(1);
            }
            onReleased: {
                console.log("r");
                myLightBlock.changeStatus(2);
                myLightBlock.selected=true;
            }
            onEntered: {
                console.log("en");
                myLightBlock.changeStatus(3);
            }
            onExited: {
                console.log("ex");
                myLightBlock.changeStatus(4);
            }

            LightBlock{
                id: myLightBlock
                objectName: "myLightBlock"
                anchors.fill: parent
                color: "#f2f2f2"

                childCont.children :
                    Rectangle {
                        height: 50
                        width: 50
                        color: "#bbbbbb"
                    }
            }

        }
}


    function slotQml1(){
        console.log("test");
    }
}
