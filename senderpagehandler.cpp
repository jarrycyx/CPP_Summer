#include "senderpagehandler.h"
#include <myarticleobj.h>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>


SenderPageHandler::SenderPageHandler(QObject *parent, int user_id) : QObject(parent)
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
    }


}


void SenderPageHandler::requireArticleDataInNewThread(){
    QThread::msleep(500);
    qDebug()<<"data";
    query->exec("select article_id,title,content,create_time from articles");
    qDebug()<<"data1";
    while(query->next()){
        qDebug()<<"data2";
        emit SenderPageHandler::loadArticlesComplete(query->value(0).toInt() ,query->value(1).toString(), query->value(2).toString());
    }
    qDebug()<<"complete signal";
}

void SenderPageHandler::onLoadArticlesComplete(int article_id, QString title, QString content){
    qDebug()<<"complete slot";
    myThisModel.append(new MyArticleObj(article_id,title,content));
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
    MyArticleObj *articleToRemove = qobject_cast<MyArticleObj*>(myThisModel.takeAt(idx));
    //query->exec(QString("delete from articles where article_id=%1").arg(articleToRemove.articleIdOfArticle()));
    //myThisModel.removeAt(idx);
    qDebug()<<myThisModel.length()<<"itemMoveInCpp"<<articleToRemove->articleIdOfArticle();
    query->exec(QString("delete from articles where article_id=%1")
                .arg(articleToRemove->articleIdOfArticle()));
    startLoadingData(1, thisUserId);
}

void SenderPageHandler::startLoadingData(int flag, int user_id){
    //query->exec("select article_id,title,content,create_time from articles");
    if (flag) if (query){
        myThisModel.clear();
        thisUserId=user_id;
        qDebug()<<"data";
        query->exec(QString("select article_id,title,content,create_time from articles where sender=%1 ORDER BY create_time DESC")
                    .arg(user_id));
        qDebug()<<"data1";
        while(query->next()){
            qDebug()<<"data2";
            emit SenderPageHandler::loadArticlesComplete(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString());
        }
        qDebug()<<"complete signal";
    }
    emit refreshQml();
}

Q_INVOKABLE void SenderPageHandler::addAnArticle(QString title, QString content){
    //query->exec(QString("select article_id from articles where sender=%1 and create_time=now()").arg(thisUserId));
    query->exec(QString("insert into articles (title, content, create_time, sender) values (\"%1\", \"%2\", NOW(), %3)")
                .arg(title).arg(content).arg(thisUserId));
    startLoadingData(1, thisUserId);
    //myThisModel.push_front(new MyArticleObj(-1, title,content));
    //emit refreshQml();
}
