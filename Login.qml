import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import QtQuick.Controls.Universal 2.12

import "./Components"

ApplicationWindow {
    id: loginWindow
    objectName: "loginWindow"
    visible: true
    width: 532
    height: 360
    title: qsTr("Stack")



    /* 存放两个输入框数据的结构
     * 需要说明的是，由于我自己实现了符合Fluent Design设计规范的许多控件
     * 其中输入框控件的“聚焦”操作依赖于ListView，也就需要相应的model
     */
    ListModel {
        id: loginModel
        objectName: "loginModel"
        ListElement {
            textInEdit: "sender1"
            placeHolderText: "请输入用户名"
        }
        ListElement {
            textInEdit: "sender1"
            placeHolderText: "请输入密码"
        }
    }


    Connections{
        target: loginPageHandler
        onSendErrorMessage: {
            console.log(errStr);
            errText.text=errStr;
            errText.color="#E51400";
        }
        onSendSuccessMessage: {
            console.log(successStr);
            errText.text=successStr;
            errText.color="#60A917";
            if (successStr==="登陆成功")
                loginWindow.close();
        }
        onRequireComplete: {
            busyIndicator.visible = false;
            errText.visible = false;
            if (flag==1) loginWindow.close();
        }
    }



    Rectangle{
        EditLightBox{
            id: loginDelegate
        }
        height: 114
        width: parent.width
        y:102
        ListView {
            id: loginList
            height: 114
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            model: loginModel
            delegate: loginDelegate
            anchors.fill: parent
            spacing: 18
            cacheBuffer: 50
        }

    }


    Button {
        id: button
        x: 44
        y: 234
        width: 213
        height: 48
        text: qsTr("登录")
        onClicked: {
            loginPageHandler.loginInit(loginModel.get(0).textInEdit, loginModel.get(1).textInEdit, 1);
        }
        font.family: "DengXian"
    }

    Button {
        id: button1
        x: 275
        y: 234
        width: 213
        height: 48
        text: qsTr("注册")
        font.family: "DengXian"
        onClicked: {
            loginPageHandler.signUp(loginModel.get(0).textInEdit,loginModel.get(1).textInEdit, 1);
        }
    }



    Image {
        id: borderImage
        x: 44
        y: 40
        width: 244
        height: 40
        sourceSize.width: 244
        sourceSize.height: 40
        source: "Resources/logintext.svg"
    }

    Text {
        id: element
        x: 44
        y: 304
        width: 154
        height: 28
        color: Universal.accent
        text: qsTr("Forget Password?")
        font{
            family: "DengXian";
            pixelSize: 14
        }
    }

    BusyIndicator {
        id: busyIndicator
        visible: false;
        x: 448
        y: 40
        width: 40
        height: 40
    }

    Text {
        id: errText
        x: 334
        y: 304
        width: 154
        height: 28
        color: "#E51400"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignRight
        font.family: "DengXian"
    }





}
