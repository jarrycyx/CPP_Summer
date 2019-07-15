#include "loginpagehandler.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QtConcurrent/QtConcurrent>
#include <senderpagehandler.h>
#include <QSqlDatabase>

#include <QDebug>

#include <QSqlQuery>
#include <QThread>


LoginPageHandler::LoginPageHandler(QObject *parent) : QObject(parent)
{
    QSqlDatabase newDb = QSqlDatabase::addDatabase("QMYSQL");
    db=&newDb;
    db->setHostName("39.106.107.241");
    db->setDatabaseName("cyxcpp");
    db->setUserName("root");
    db->setPassword("cyxserver-2019");
    if (!db->open())
        qDebug() << "Failed to connect to root mysql admin";
    else qDebug() << "open";

    query=new QSqlQuery(*db);
    //engine.load(url2);
}


void LoginPageHandler::startPage(QQmlApplicationEngine *engine){
    thisEngine=engine;
    QQmlContext *thisContext=engine->rootContext();
    thisContext->setContextProperty("loginPageHandler", this);
    engine->load(QUrl(QStringLiteral("qrc:/Login.qml")));
}

QVariant LoginPageHandler::loginTextModel() const{
    return QVariant::fromValue(myLoginTextModel);
}

Q_INVOKABLE void LoginPageHandler::loginInit(QString name, QString pswd){
    QtConcurrent::run(this,&LoginPageHandler::loginInNewThread,name,pswd);
}


void LoginPageHandler::loginInNewThread(QString name, QString pswd){
    QThread::msleep(500);
    qDebug()<<name.toUtf8()<<pswd.toUtf8();
    query->exec(QString("select user_id,password from users where user_name=\"%1\";").arg(name));
    if (!query->first()) emit sendErrorMessage(QString::fromUtf8("用户不存在"));
    else{
        int userId = query->value(0).toInt();
        QString userPassword = query->value(1).toString();
        qDebug() << userId << userPassword << userPassword.compare(pswd);
        if (userPassword.compare(pswd)==0) {
            emit LoginPageHandler::requireComplete(1);
            QThread::msleep(200);
        }
        else emit sendErrorMessage(QString::fromUtf8("用户名与密码不匹配"));
    }
}

void LoginPageHandler::startMainPage(int flag){
    if (flag){
        //ConnectionPool::release();
        //db->close();
        //const QUrl url1(QStringLiteral("qrc:/SenderPage.qml"));
        //engine.load(url1);
        SenderPageHandler newSenderPage;
        //qDebug()<< connect(&newSenderPage, SIGNAL(loadArticlesComplete()), &newSenderPage, SLOT(onLoadArticlesComplete()));
        newSenderPage.startPage(thisEngine);
    }
}

Q_INVOKABLE void LoginPageHandler::signUp(QString name, QString pswd){
    QtConcurrent::run(this,&LoginPageHandler::signUpInNewThread,name,pswd);
}

void LoginPageHandler::signUpInNewThread(QString name, QString pswd){
    qDebug()<<name.toUtf8()<<pswd.toUtf8();
    if (name.length()>=5&&pswd.length()>=8){
        QThread::sleep(1);
        QString cmdQStr=QString("insert into users (user_type, user_name, create_time,password) values (0, \"%1\", now(),\"%2\");")
                .arg(name).arg(pswd);
        qDebug()<<cmdQStr;
        query->exec(cmdQStr);
        emit LoginPageHandler::requireComplete(0);
    }else emit sendErrorMessage(QString::fromUtf8("长度不足"));
}
