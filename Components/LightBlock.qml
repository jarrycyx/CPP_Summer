import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.12

import QtGraphicalEffects 1.0
import "../Resources"

Rectangle {
    property int borderWidth: 2
    property color outsideColor: "#eeeeee"
    property color clickBorderColor: "#c0c0c0"
    property color hoverBorderColor: "#dddddd"
    property color selectedBorderColor: Universal.accent
    property color selectedHoverBorderColor: "#6891FF"
    property color lightChaserColorCenterHover: "#66ffffff"
    property color lightChaserColorCenterPressed: "#aaffffff"

    property alias childCont: innerComponent

    height: 200
    width: 200
    color: "white"
    border.width: borderWidth
    border.color: outsideColor
    z:2


    property int heightBefore: height-borderWidth*2
    property int widthBefore: width-borderWidth*2
    property int heightAfterShrinking: height-borderWidth*4
    property int widthAfterShrinking: width-borderWidth*4
    property bool selected: false

    //default property color shadowColor: shadow.color

    layer.enabled: true
    layer.effect: DropShadow {
        id: shadow
        objectName: "shadow"
        spread: 0
        color: "#00999999"
        radius: 4
        samples: 17
        transparentBorder: true
        horizontalOffset: 2
        verticalOffset: 2
    }

    Behavior on border.color {
        ColorAnimation { duration: 80 }
    }

    onSelectedChanged: {
        changeStatus(4);
    }

    function changeStatus(stat) {
        if (stat===1){
            if (selected) border.color=selectedBorderColor
            else border.color=clickBorderColor
            innerComponent.width=widthAfterShrinking;
            innerComponent.height=heightAfterShrinking;
            innerComponent.x=borderWidth*2;
            innerComponent.y=borderWidth*2;
            lightChaser.hoverLightChaserColorCenter=lightChaserColorCenterPressed
            //shadowColor="#44999999"
        }
        else if (stat===2){
            //border.color=outsideColor
            innerComponent.width=widthBefore;
            innerComponent.height=heightBefore;
            innerComponent.x=borderWidth;
            innerComponent.y=borderWidth;
            lightChaser.hoverLightChaserColorCenter=lightChaserColorCenterHover
            changeStatus(3);
        }else if (stat===3){
            if (selected) border.color=selectedHoverBorderColor
            else border.color=hoverBorderColor
            lightChaser.hoverLightChaserColorCenter=lightChaserColorCenterHover
        }else if (stat===4){
            if (selected) border.color=selectedBorderColor
            else border.color=outsideColor
            lightChaser.hoverLightChaserColorCenter="#00ffffff"
        }
    }

    function chaseLight(x, y){
        if (x!==-1) lightChaser.horizontalOffset=x-width/2;
        if (y!==-1) lightChaser.verticalOffset=y-height/2;
    }


    Rectangle  {
        id: innerComponent
        x: parent.borderWidth
        y: parent.borderWidth
        height: parent.height-parent.borderWidth*2
        width: parent.width-parent.borderWidth*2

        Behavior on x { NumberAnimation { duration: 100 } }
        Behavior on y { NumberAnimation { duration: 100 } }
        Behavior on height { NumberAnimation { duration: 100 } }
        Behavior on width { NumberAnimation { duration: 100 } }
    }

    RadialGradient {
        property color hoverLightChaserColorCenter: "#00ffffff"
        id: lightChaser
        //source: radialOrig;
        height: parent.height
        width: parent.width
        verticalOffset: -1000
        horizontalOffset: -1000
        horizontalRadius: parent.width*0.6;
        verticalRadius: parent.width*0.6;
        gradient: Gradient {
            GradientStop{
                position: 0.2;
                color: lightChaser.hoverLightChaserColorCenter;
                Behavior on color { ColorAnimation { duration: 100 } }
            }

            //GradientStop{ position: 0.3; color: "#84ffffff"}
            GradientStop{ position: 1.0; color: "#00ffffff"}
        }
    }
}
