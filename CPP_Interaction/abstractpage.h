//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Interaction
/************************************************************************************************************************
类名：     AbstractPage
功能：     各用户后台流程类的基类，用于实现一些页面共有的方法（信号），以及规定派生流程类需要实现的接口
日期：     20190722 增加该类
          20190724 修改为抽象类
************************************************************************************************************************/

#ifndef ABSTRACTPAGE_H
#define ABSTRACTPAGE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include "../CPP_Storage/globalstoragecomponents.h"

class AbstractPage : public QObject
{
    Q_OBJECT
public:
    //需要指定当前使用页面的用户Id，若为不需要指定则为-1
    explicit AbstractPage(int newUserId, QObject *parent = nullptr);
    virtual ~AbstractPage();
    //所有页面共有，点击“显示用户信息”按钮所调用的方法
    Q_INVOKABLE void showUserInfo();
    //所有页面共有，点击“显示文章信息”按钮所调用的方法
    Q_INVOKABLE void showArticleInfo(int articleId);

    static GlobalStorageComponents *storage;

signals:
    //错误信息信号，向QML发送，使其在界面上显示
    void sendErrorMessage(QString errStr);
    //成功信息信号，向QML发送，使其在界面上显示
    void sendSuccessMessage(QString successStr);

public slots:
    //开始渲染主页面
    virtual void startPage(QQmlApplicationEngine *engine) = 0;

protected:
    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *mThisEngine;
    //当前身份标识，默认为-1（空）
    int mThisUserId;
};

#endif // ABSTRACTPAGE_H
