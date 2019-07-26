//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     View
/************************************************************************************************************************
文件名：    AcrylicBlock
功能：      实现Fluent Design效果的底层控件，模仿亚克力质感，有追光、悬停、选中等不同效果
日期：      20190705 实现亚克力质感风格
           20190708 实现悬停、选中的效果
           20190710 实现追光效果
           20190711 封装完成
************************************************************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.12

import QtGraphicalEffects 1.0
import "../../Resources"

Rectangle {
    property int borderWidth: 2
    property color outsideColor: "#eeeeee"                          //设置外围颜色，即为边框默认颜色
    property color clickBorderColor: "#c0c0c0"                      //鼠标按下状态时的边框颜色
    property color hoverBorderColor: "#dddddd"                      //鼠标悬浮状态时的边框颜色
    property color selectedBorderColor: Universal.accent            //选中高亮状态的边框颜色
    property color selectedHoverBorderColor: "#6891FF"              //选中状态是，鼠标悬停的边框颜色
    property color lightChaserColorCenterHover: "#66ffffff"         //追光效果的默认颜色
    property color lightChaserColorCenterPressed: "#aaffffff"       //按下时的追光颜色

    property alias childCont: innerComponent                        //外部调用时设置内部控件

    height: 200
    width: 200
    color: "white"
    border.width: borderWidth
    border.color: outsideColor
    z:2

    property int heightBefore: height-borderWidth*2                 //按下前内部大小
    property int widthBefore: width-borderWidth*2
    property int heightAfterShrinking: height-borderWidth*4         //按下时内部略微变小效果
    property int widthAfterShrinking: width-borderWidth*4
    property bool selected: false

    layer.enabled: true

    Behavior on border.color {
        ColorAnimation { duration: 80 }
    }

    onSelectedChanged: {
        changeStatus(4);
    }

    //类似于枚举，定义四种状态
    property int pressed: 1
    property int released: 2
    property int hovered: 3
    property int none: 4

    function changeStatus(stat) {
        if (stat===pressed){
            if (selected) border.color=selectedBorderColor
            else border.color=clickBorderColor
            innerComponent.width=widthAfterShrinking;
            innerComponent.height=heightAfterShrinking;
            innerComponent.x=borderWidth*2;
            innerComponent.y=borderWidth*2;
            lightChaser.hoverLightChaserColorCenter=lightChaserColorCenterPressed
        }
        else if (stat===released){
            //border.color=outsideColor
            innerComponent.width=widthBefore;
            innerComponent.height=heightBefore;
            innerComponent.x=borderWidth;
            innerComponent.y=borderWidth;
            lightChaser.hoverLightChaserColorCenter=lightChaserColorCenterHover
            changeStatus(hovered);
        }else if (stat===hovered){
            if (selected) border.color=selectedHoverBorderColor                 //选中则显示高亮颜色
            else border.color=hoverBorderColor
            lightChaser.hoverLightChaserColorCenter=lightChaserColorCenterHover
        }else if (stat===none){
            if (selected) border.color=selectedBorderColor
            else border.color=outsideColor
            lightChaser.hoverLightChaserColorCenter="#00ffffff"
        }
    }

    //外部检测鼠标悬停时调用此函数改变追光位置
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

        //颜色、大小改变的渐变效果
        Behavior on x { NumberAnimation { duration: 100 } }
        Behavior on y { NumberAnimation { duration: 100 } }
        Behavior on height { NumberAnimation { duration: 100 } }
        Behavior on width { NumberAnimation { duration: 100 } }
    }

    //追光效果
    RadialGradient {
        property color hoverLightChaserColorCenter: "#00ffffff"
        id: lightChaser
        height: parent.height
        width: parent.width
        //没有鼠标悬停时追光消失
        verticalOffset: -1000
        horizontalOffset: -1000
        horizontalRadius: parent.width*0.6;
        verticalRadius: parent.width*0.6;
        gradient: Gradient {
            //圆台效果比锥形效果在视觉上更友好
            GradientStop{
                position: 0.2;
                color: lightChaser.hoverLightChaserColorCenter;
                Behavior on color { ColorAnimation { duration: 100 } }
            }
            GradientStop{ position: 1.0; color: "#00ffffff"}
        }
    }
}
