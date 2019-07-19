#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <cppobjs/myarticleobj.h>
#include <loginpagehandler.h>
#include <cppobjs/callremotemysqlthread.h>
#include <QTimer>
#include <senderpagehandler.h>
#include "cpp-Components/globalcomponents.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    GlobalComponents globalStorageComponent(&engine);

    LoginPageHandler newLoginPage(&globalStorageComponent); //创建登陆页面对象
    newLoginPage.startPage(&engine);//渲染登录页面的主界面

    //由于QML前台渲染部分程序与C++后台处理程序部分连接的需要，大部分页面的对象需要在主函数创建
    //SenderPageHandler newSenderPage;//创建“发送者”页面
    //newSenderPage.startPage(&engine);

    //C++后台程序中，跨界面的信号与槽的连接
    //QObject::connect(&newLoginPage, SIGNAL(requireComplete(int, int)),
    //                 &newSenderPage, SLOT(startLoadingData(int, int)));
                    //登陆成功信号与开始加载数据信号的连接，可以部分冲淡同步读取数据带来的阻塞问题


    return app.exec();
}

