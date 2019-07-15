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
    Q_OBJECT

    Q_PROPERTY(QVariant loginTextModel READ loginTextModel NOTIFY loginTextModelChanged)
public:
    explicit LoginPageHandler(QObject *parent = nullptr);

    QQmlApplicationEngine *thisEngine;
    Q_INVOKABLE void loginInit(QString name, QString pswd);
    Q_INVOKABLE void signUp(QString name, QString pswd);
    QVariant loginTextModel() const;// { return m_value1; }
    void setLoginTextModel(const QVariant& v);

private:
    QSqlQuery *query;
    QList<QObject*> myLoginTextModel;
    void signUpInNewThread(QString name, QString pswd);
    void loginInNewThread(QString name, QString pswd);
    QSqlDatabase *db;
signals:
    void loginTextModelChanged(const QVariant&);
    void sendErrorMessage(QString errStr);
    void requireComplete(int flag);//1:start new page

public slots:
    void startPage(QQmlApplicationEngine *engine);
    void startMainPage(int flag);//1:start new page
};

#endif // LOGINPAGEHANDLER_H
