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

    QStringList dataList;
    dataList.append("Item 1");
    dataList.append("Item 2");
    dataList.append("Item 3");
    dataList.append("Item 4");


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("myModel", QVariant::fromValue(dataList));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}

