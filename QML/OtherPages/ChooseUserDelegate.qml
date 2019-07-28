import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

Component{

    Rectangle {
        height: 80
        width: 450
        id: mainRectInChoose
        color: "#f2f2f2"
        property int indexOfThisDelegate: index

        ToolButton{
            anchors.fill:parent
            onClicked: {
                if (mainRectInChoose.indexOfThisDelegate != -1){
                    if (typeof senderPageHandler!=='undefined')
                        senderPageHandler.regulatorChosen(mainRectInChoose.indexOfThisDelegate);
                    if (typeof regulatorPageHandler!=='undefined')
                        regulatorPageHandler.translatorChosen(mainRectInChoose.indexOfThisDelegate);
                    chooseRegulatorWindow.close();
                }
            }
        }

        Image {
            id: borderImage
            x: 18
            y: 18
            width: 45
            height: 45
            sourceSize.width: 45
            sourceSize.height: 45
            source: "../../Resources/user-default.svg"
        }

        Text {
            id: translatorUsername
            x: 86
            y: 17
            width: 200
            height: 20
            text: nameOfUser
            font{
                family: "DengXian";
                pixelSize: 20
            }
        }

        Text {
            id: translatorQulification
            x: 86
            y: 47
            width: 257
            height: 12
            wrapMode: Text.WrapAnywhere
            text: "资质："+qualificationOfUser+" 积分："+credits
            font{
                family: "DengXian";
                pixelSize: 14
            }
        }
    }
}
