//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     UserInfoPageHandler
功能：     用户页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190725 实现用户信息界面
          20190726 实现消息盒子
************************************************************************************************************************/


#ifndef USERINFOPAGEHANDLER_H
#define USERINFOPAGEHANDLER_H

#include <QObject>
#include "../CPP_Data/myuserobj.h"
#include "../CPP_Model/requestuserlist.h"
#include "../CPP_Storage/globalstoragecomponents.h"
#include "../CPP_Model/messagelist.h"
#include "abstractpage.h"

class UserInfoPageHandler : public AbstractPage
{
Q_OBJECT

public:
    explicit UserInfoPageHandler(MyUserObj* thisUser, QObject *parent = nullptr);
    Q_INVOKABLE QString getUsername();
    Q_INVOKABLE int getMoney();
    Q_INVOKABLE int getCredit();
    Q_INVOKABLE QString getQualification();
    Q_INVOKABLE QString getMultiuserStatus();
    Q_INVOKABLE void updateUser(QString name, QString pswd);
    Q_INVOKABLE void addMoney(int addedMoney);

signals:

public slots:
    void startPage(QQmlApplicationEngine *engine);

private:
    MyUserObj* user;
    MessageList messageList;
};

#endif // USERINFOPAGEHANDLER_H
