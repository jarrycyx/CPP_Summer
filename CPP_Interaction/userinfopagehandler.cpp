//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     UserInfoPageHandler
功能：     用户页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
备注：     credit在25分以上可以做翻译者，在45分以上可以做负责人
日期：     20190725 实现用户信息界面
          20190726 实现消息盒子
************************************************************************************************************************/


#include "userinfopagehandler.h"
#include <QDebug>
#include "regulatorpagehandler.h"

/*************************************************************************
名称：     UserInfoPageHandler
功能：     构造函数，同时创建数据库连接
参数：     parent，可以为空
日期：     20190712
*************************************************************************/
UserInfoPageHandler::UserInfoPageHandler(MyUserObj* thisUser, QObject *parent) :
    AbstractPage(-1), user(thisUser)
{

}

/*************************************************************************
名称：     startPage
功能：     构造函数，同时创建数据库连接
参数：     parent，可以为空
日期：     20190712
*************************************************************************/
void UserInfoPageHandler::startPage(QQmlApplicationEngine *engine)
{
    engine->rootContext()->setContextProperty("userPageHandler", this);
    engine->rootContext()->setContextProperty("messageList", &messageList);
    engine->load(QUrl(QStringLiteral("qrc:/QML/OtherPages/UserInfoPage.qml")));
    qDebug() << "load user messagebox";
    int len = storage->getRequestsLength();
    for (int i = 0; i < len; i++)
    {
        if (storage->getRequest(i)->getUserId() == user->userId()
                && storage->getRequest(i)->getType() == 4){
            messageList.addAMessage(storage->getRequest(i));
        }
    }
}


/*************************************************************************
名称：     getUsername等
功能：     获取各类信息
日期：     20190727
*************************************************************************/
Q_INVOKABLE QString UserInfoPageHandler::getUsername()
{
    return user->username();
}
Q_INVOKABLE int UserInfoPageHandler::getMoney()
{
    return user->money();
}
Q_INVOKABLE int UserInfoPageHandler::getCredit()
{
    return user->credit();
}
Q_INVOKABLE QString UserInfoPageHandler::getQualification()
{
    return user->qualification();
}

/*************************************************************************
名称：     getMultiuserStatus
功能：     向QML发送用户权限信息
参数：     无
返回：     权限信息的四位代码QString
日期：     20190727
*************************************************************************/
Q_INVOKABLE QString UserInfoPageHandler::getMultiuserStatus()
{
    //一共四位，分别表示发布者、负责人、翻译者、监管者
    //0为无权限，1为有权限
    if (user->credit()>=65) return QString("1111");
    if (user->credit()>=45) return QString("1110");
    if (user->credit()>=25) return QString("1010");
    return QString("1000");
}

/*************************************************************************
名称：     updateUser
功能：     更新用户信息
参数：     新的用户名和密码
返回：     无
日期：     20190726
*************************************************************************/
Q_INVOKABLE void UserInfoPageHandler::updateUser(QString name, QString pswd){
    user->setUsername(name);
    user->setPassword(pswd);
    //发送状态更新提醒
    storage->sendUserModifiedMessage(user->userId(), QString("您的账户名或密码已改变，请牢记"));

    qDebug() << "set" << name << " " <<pswd;
}

/*************************************************************************
名称：     addMoney
功能：     充值，点击一次充值100
参数：     充值的金额
返回：     无
日期：     20190726
*************************************************************************/
Q_INVOKABLE void UserInfoPageHandler::addMoney(int addedMoney){
    if (addedMoney<0 || addedMoney>1000) return;
    user->setMoney(user->money()+addedMoney);
    qDebug() << "add money " << addedMoney << " to " << user->money();
}
