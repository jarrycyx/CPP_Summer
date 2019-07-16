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

    //qmlRegisterType<SenderPageHandler>("qt.cpp.QSenderPageHandler", 1, 0, "QSenderPageHandler");

    QQmlApplicationEngine engine;




    LoginPageHandler newLoginPage;
    newLoginPage.startPage(&engine);

    SenderPageHandler newSenderPage;
    newSenderPage.startPage(&engine);
    QObject::connect(&newLoginPage, SIGNAL(requireComplete(int, int)), &newSenderPage, SLOT(startLoadingData(int, int)));

    return app.exec();
}

