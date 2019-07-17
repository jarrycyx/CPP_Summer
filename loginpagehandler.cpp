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

/*************************************************************************
【类名】        LoginPageHandler
【功能】        登陆/注册界面的后台数据处理类，与Login.qml联动
【接口说明】     startPage函数从外部调用，表示开始渲染界面
【开发者及日期】  jarrycyx 20190713
【更改记录】     20190714: 修改为异步查询数据
*************************************************************************/


/* 登陆界面的后台处理类
 * 使用异步线程请求登录数据
 */


/*************************************************************************
【函数名称】  LoginPageHandler
【函数功能】  构造函数，同时创建数据库连接
【参数】    parent，可以为空
【开发者及日期】    jarrycyx 20190709
*************************************************************************/
LoginPageHandler::LoginPageHandler(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("39.106.107.241");
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else qDebug() << "open";

    query=new QSqlQuery(db);
}

/*************************************************************************
【函数名称】  ~LoginPageHandler
【函数功能】  析构函数，同时释放内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
LoginPageHandler::~LoginPageHandler(){
    delete query;
}

/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void LoginPageHandler::startPage(QQmlApplicationEngine *engine){
    QQmlContext *thisContext=engine->rootContext();
    thisContext->setContextProperty("loginPageHandler", this);
    thisEngine=engine;
    engine->load(QUrl(QStringLiteral("qrc:/Login.qml")));
}


/*************************************************************************
【函数名称】  loginInit，loginInNewThread
【函数功能】  QML从界面中唤起该函数，创建新线程执行登录操作（loginInNewThread）
【参数】    从界面中读取的用户名和密码
【返回值】   无
【开发者及日期】    jarrycyx 20190714
*************************************************************************/
Q_INVOKABLE void LoginPageHandler::loginInit(QString name, QString pswd){
    //创建新线程
    QtConcurrent::run(this,&LoginPageHandler::loginInNewThread,name,pswd);
}

//在新建线程中请求服务器数据
void LoginPageHandler::loginInNewThread(QString name, QString pswd){
    //QThread::msleep(500); //让好看的进度指示圈多转一会
    qDebug()<<name.toUtf8()<<pswd.toUtf8();
    query->exec(QString("select user_id,password from users where user_name=\"%1\";").arg(name));
    if (!query->first()) emit sendErrorMessage(QString::fromUtf8("用户不存在"));
    else{
        int userId = query->value(0).toInt();
        QString userPassword = query->value(1).toString();
        qDebug() << userId << userPassword << userPassword.compare(pswd);
        if (userPassword.compare(pswd)==0) {
            emit LoginPageHandler::requireComplete(1, userId);
                //发送请求完成信号，flag为1代表其为登录完成信号
            //QThread::msleep(200); //让好看的进度指示圈多转一会
        }
        else emit sendErrorMessage(QString::fromUtf8("用户名与密码不匹配"));
    }
}

/*************************************************************************
【函数名称】  signUp，signUpInNewThread
【函数功能】  QML从界面中唤起该函数，创建新线程执行注册操作（signUpInNewThread）
【参数】    从界面中读取的用户名和密码
【返回值】   无
【开发者及日期】    jarrycyx 20190714
*************************************************************************/
Q_INVOKABLE void LoginPageHandler::signUp(QString name, QString pswd){
    QtConcurrent::run(this,&LoginPageHandler::signUpInNewThread,name,pswd);
}

void LoginPageHandler::signUpInNewThread(QString name, QString pswd){
    qDebug()<<name.toUtf8()<<pswd.toUtf8();
    if (name.length()>=5&&pswd.length()>=8){
        QThread::sleep(1);
        query->exec(QString("select user_id from users where user_name=\"%1\"").arg(name));
        if (!query->first()){
            QString cmdQStr=QString("insert into users (user_type, user_name, create_time,password) values (0, \"%1\", now(),\"%2\");")
                    .arg(name).arg(pswd);
            qDebug()<<cmdQStr;
            query->exec(cmdQStr);
            emit LoginPageHandler::requireComplete(0, -1);
        }else emit sendErrorMessage(QString::fromUtf8("用户名已存在"));
    }else emit sendErrorMessage(QString::fromUtf8("长度不足"));
    //无需向服务器请求即可对密码合法性进行判断
}
