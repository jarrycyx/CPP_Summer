#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <cppobjs/myarticleobj.h>
#include <loginpagehandler.h>
#include <QTimer>
#include <senderpagehandler.h>
#include "cpp-Components/globalcomponents.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    GlobalComponents globalStorageComponent;

    LoginPageHandler newLoginPage(&globalStorageComponent); //创建登陆页面对象
    newLoginPage.startPage(&engine);                        //渲染登录页面的主界面

    return app.exec();
}
