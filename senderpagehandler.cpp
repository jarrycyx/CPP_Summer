#include "senderpagehandler.h"
#include <myarticleobj.h>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>


SenderPageHandler::SenderPageHandler(QObject *parent) : QObject(parent)
{
    /*myThisModel.append(new MyArticleObj("Item 9", "yellow"));*/

    QObject::connect(this, &SenderPageHandler::loadArticlesComplete, this, &SenderPageHandler::onLoadArticlesComplete);


    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setHostName("39.106.107.241");
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else {
        qDebug() << "open";
        query=new QSqlQuery(db);
        //QtConcurrent::run(this,&SenderPageHandler::requireArticleDataInNewThread);
        //emit loadArticlesComplete("","");
        qDebug()<<"data";
        query->exec("select article_id,title,content,create_time from articles");
        qDebug()<<"data1";
        while(query->next()){
            qDebug()<<"data2";
            emit SenderPageHandler::loadArticlesComplete(query->value(1).toString(), query->value(2).toString());
        }
        qDebug()<<"complete signal";
    }
}


void SenderPageHandler::requireArticleDataInNewThread(){
    QThread::msleep(500);
    qDebug()<<"data";
    query->exec("select article_id,title,content,create_time from articles");
    qDebug()<<"data1";
    while(query->next()){
        qDebug()<<"data2";
        emit SenderPageHandler::loadArticlesComplete(query->value(1).toString(), query->value(2).toString());
    }
    qDebug()<<"complete signal";
}

void SenderPageHandler::onLoadArticlesComplete(QString title, QString content){
    qDebug()<<"complete slot";
    myThisModel.append(new MyArticleObj(title,content));
    emit refreshQml();
}

void SenderPageHandler::startPage(QQmlApplicationEngine *engine){
    QQmlContext *thisContext=engine->rootContext();
    thisContext->setContextProperty("senderPageHandler", this);
    const QUrl url1(QStringLiteral("qrc:/SenderPage.qml"));
    engine->load(url1);
}


QVariant SenderPageHandler::thisModel() const{
    return QVariant::fromValue(myThisModel);
}


Q_INVOKABLE void SenderPageHandler::itemMove(int idx){
    myThisModel.removeAt(idx);
    qDebug()<<myThisModel.length();
}
