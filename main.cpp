#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <processingmodel.h>
#include <qprocessclass.h>
#include <QQmlContext>
#include <cppobjs/myarticleobj.h>
#include <loginpagehandler.h>
#include <cppobjs/callremotemysqlthread.h>
#include <QTimer>
#include <senderpagehandler.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ProcessingModel>("qt.cpp.ProcessingModel", 1, 0, "ProcessingModel");
    qmlRegisterType<QProcessClass>("qt.cpp.QProcessClass", 1, 0, "QProcessClass");

    QQmlApplicationEngine engine;


    LoginPageHandler newLoginPage;
    newLoginPage.startPage(&engine);
    QObject::connect(&newLoginPage, SIGNAL(requireComplete(int)), &newLoginPage, SLOT(startMainPage(int)));




    return app.exec();
}

