import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.12

import QtGraphicalEffects 1.0

Rectangle{
    property int borderWidth: 3
    property color outsideColor: "#eeeeee"
    property color clickBorderColor: "#c0c0c0"
    property color hoverBorderColor: "#dddddd"


    property alias childCont: innerComponent

    height: 200
    width: 200
    color: "white"
    border.width: borderWidth
    border.color: outsideColor


    property int heightBefore: height
    property int widthBefore: width
    property int heightAfterShrinking: height-borderWidth*2
    property int widthAfterShrinking: width-borderWidth*2



    Behavior on border.color {
        ColorAnimation { duration: 100 }
    }
    function changeStatus(stat) {
        if (stat===1){
            border.color=clickBorderColor
            innerComponent.width=widthAfterShrinking;
            innerComponent.height=heightAfterShrinking;
            innerComponent.x=borderWidth*2;
            innerComponent.y=borderWidth*2;
        }
        else if (stat===2){
            //border.color=outsideColor
            innerComponent.width=widthBefore;
            innerComponent.height=heightBefore;
            innerComponent.x=borderWidth;
            innerComponent.y=borderWidth;
            changeStatus(3);
        }else if (stat===3){
            border.color=hoverBorderColor
        }else if (stat===4){
            border.color=outsideColor
        }
    }


    Row  {
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





}
