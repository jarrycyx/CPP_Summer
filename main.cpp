#pragma execution_character_set("utf-8")

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "CPP_Interaction/loginpagehandler.h"
#include <QtDebug>

//主函数，创建应用程序对象，以及QML渲染引擎对象
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    LoginPageHandler newLoginPage;   //创建登陆页面对象
    newLoginPage.startPage(&engine); //渲染登录页面的主界面

    return app.exec();
}
