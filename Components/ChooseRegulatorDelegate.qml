import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12

Component{

    Rectangle {
        height: 80
        width: 450
        id: mainRectInChoose

        property int indexOfThisDelegate: index

        ToolButton{
            anchors.fill:parent
            onClicked: {
                senderPageHandler.regulatorChosen(mainRectInChoose.indexOfThisDelegate);
                chooseRegulatorWindow.close();
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
            source: "../Resources/user-default.svg"
        }

        Text {
            id: translatorUsername
            x: 86
            y: 17
            width: 200
            height: 20
            text: model.modelData.username
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
            text: model.modelData.password
            font{
                family: "DengXian";
                pixelSize: 12
            }
        }

        Text {
            id: translatorInProgressNumber
            x: 383
            y: 13
            width: 28
            height: 28
            text: qsTr("3")
            color: "#27ae60"
            font{
                family: "DengXian";
                pixelSize: 28
            }
        }

        Text {
            x: 362
            y: 47
            width: 70
            text: qsTr("正在进行中")
            font{
                family: "DengXian";
                pixelSize: 12
            }
        }
    }
}
