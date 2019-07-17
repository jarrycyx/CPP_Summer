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

class LoginPageHandler : public QObject
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
public:
    explicit LoginPageHandler(QObject *parent = nullptr);
    ~LoginPageHandler();
    //登录
    Q_INVOKABLE void loginInit(QString name, QString pswd);
    //注册
    Q_INVOKABLE void signUp(QString name, QString pswd);

private:
    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *thisEngine;
    //数据库请求对象指针
    QSqlQuery *query;
    QList<QObject*> myLoginTextModel;
    //新线程中注册
    void signUpInNewThread(QString name, QString pswd);
    //新线程中登录
    void loginInNewThread(QString name, QString pswd);
signals:
    //错误信息信号，向QML发送，使其在界面上显示
    void sendErrorMessage(QString errStr);
    //向服务器请求完成信号，其中flag为1表示登录请求
    void requireComplete(int flag, int user_id);

public slots:
    //开始渲染主页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif // LOGINPAGEHANDLER_H