#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <processingmodel.h>
#include <qprocessclass.h>
#include <QQmlContext>
#include <myarticleobj.h>

int main(int argc, char *argv[])
{
    qDebug() << "currentPath:";


    QGuiApplication app(argc, argv);

    qmlRegisterType<ProcessingModel>("qt.cpp.ProcessingModel", 1, 0, "ProcessingModel");
    qmlRegisterType<QProcessClass>("qt.cpp.QProcessClass", 1, 0, "QProcessClass");

    QQmlApplicationEngine engine;

    const QUrl url1(QStringLiteral("qrc:/SenderPage.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url1](QObject *obj, const QUrl &objUrl) {
        if (!obj && url1 == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url1);

    const QUrl url2(QStringLiteral("qrc:/Login.qml"));
    engine.load(url2);




    return app.exec();
}

