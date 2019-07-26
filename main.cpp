#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QTimer>
#include "CPP_Storage/globalcomponents.h"
#include "CPP_Interaction/loginpagehandler.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    GlobalComponents globalStorageComponent;

    LoginPageHandler newLoginPage(&globalStorageComponent); //创建登陆页面对象
    newLoginPage.startPage(&engine);                        //渲染登录页面的主界面

    return app.exec();
}
