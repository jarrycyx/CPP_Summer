#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QTimer>
#include "CPP_Storage/globalstoragecomponents.h"
#include "CPP_Interaction/loginpagehandler.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>


int main(int argc, char *argv[])
{
    QGuiApplication::addLibraryPath("./sqldrivers");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    LoginPageHandler newLoginPage;                          //创建登陆页面对象
    newLoginPage.startPage(&engine);                        //渲染登录页面的主界面

    return app.exec();
}
