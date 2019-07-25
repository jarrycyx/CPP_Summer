//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     LoginPageHandler
功能：     登录、注册页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190710 初步实现登录注册功能
                 20190720 增加一些便于调试的功能
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

#include "cpp-Components/globalcomponents.h"
#include "senderpagehandler.h"
#include "regulatorpagehandler.h"
#include "translatorpagehandler.h"

class LoginPageHandler : public QObject
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
        public : explicit LoginPageHandler(GlobalComponents *newGlobalStorageComponent, QObject *parent = nullptr);
    ~LoginPageHandler();
    //登录
    Q_INVOKABLE void loginInit(QString name, QString pswd, int role = 1);
    //注册
    Q_INVOKABLE void signUp(QString name, QString pswd, int role);

    Q_INVOKABLE int addUser(QString name, QString pswd, int role);
    Q_INVOKABLE int searchUser(QString name, int role);
    Q_INVOKABLE int userLogin(QString name, QString pswd, int role);

private:
    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *thisEngine;
    GlobalComponents *globalStorageComponent;
    SenderPageHandler *newSenderPage;
    RegulatorPageHandler *newRegulatorPage;
    TranslatorPageHandler *newTranslatorPage;

signals:
    //错误信息信号，向QML发送，使其在界面上显示
    void sendErrorMessage(QString errStr);
    //成功信息信号，向QML发送，使其在界面上显示
    void sendSuccessMessage(QString successStr);
    //向服务器请求完成信号，其中flag为1表示登录请求
    void requireComplete(int flag, int user_id);

public slots:
    //开始渲染主页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif // LOGINPAGEHANDLER_H
