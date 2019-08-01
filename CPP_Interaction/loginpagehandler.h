//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     LoginPageHandler
功能：     登录、注册页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190710 初步实现登录注册功能
          20190720 增加一些便于调试的功能
          20190725 调整用户的多种身份登录
************************************************************************************************************************/

#ifndef LOGINPAGEHANDLER_H
#define LOGINPAGEHANDLER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQmlContext>
#include <QObject>
#include <QQuickView>
#include <QSqlQuery>

#include "../CPP_Storage/globalstoragecomponents.h"
#include "senderpagehandler.h"
#include "regulatorpagehandler.h"
#include "translatorpagehandler.h"
#include "abstractpage.h"
#include "supervisorpagehandler.h"

class LoginPageHandler : public AbstractPage
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志

public : 
    explicit LoginPageHandler();
    ~LoginPageHandler();
    //登录
    Q_INVOKABLE void loginInit(QString name, QString pswd, int role = 1);
    //注册
    Q_INVOKABLE void signUp(QString name, QString pswd);
    //增加用户，调用Model
    Q_INVOKABLE MyUserObj *addUser(QString name, QString pswd);
    //调用Storage查找用户
    Q_INVOKABLE int searchUser(QString name);
    //用户登录
    Q_INVOKABLE int userLogin(QString name, QString pswd, int role);
    //唤起设置认证信息界面
    Q_INVOKABLE void setUserQualification(QString quali);

private:
    SenderPageHandler *mSenderPage;
    RegulatorPageHandler *mRegulatorPage;
    TranslatorPageHandler *mTranslatorPage;
    SupervisorPageHandler *mSupervisorPage;

    MyUserObj *mSignupUser;

public slots:
    //开始渲染主页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif // LOGINPAGEHANDLER_H
