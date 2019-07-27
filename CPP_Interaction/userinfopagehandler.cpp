#include "userinfopagehandler.h"
#include <QDebug>
#include "regulatorpagehandler.h"

UserInfoPageHandler::UserInfoPageHandler(MyUserObj* thisUser, QObject *parent) :
    AbstractPage(-1), user(thisUser)
{

}

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
Q_INVOKABLE QString UserInfoPageHandler::getMultiuserStatus()
{
    QString res="1110";
    //一共四位，分别表示发布者、负责人、翻译者、监管者
    //0为无权限，1为未注册，2为已注册，3为当前用户
    res[user->role()-1]='3';
    int len = storage->getUsersLength();
    for (int i=0;i<len;i++){
        if (storage->getUserToEdit(i)->username() == user->username()){
            if (storage->getUserToEdit(i)->userId() != user->userId()){
                res[storage->getUserToEdit(i)->role()-1]='2';
            }
        }
    }
    return res;
}


Q_INVOKABLE void UserInfoPageHandler::updateUser(QString name, QString pswd){
    user->setUsername(name);
    user->setPassword(pswd);

    qDebug() << "set" << name << " " <<pswd;
}


Q_INVOKABLE void UserInfoPageHandler::addMoney(int addedMoney){
    if (addedMoney<0 || addedMoney>1000) return;
    user->setMoney(user->money()+addedMoney);

    qDebug() << "add money " << addedMoney << " to " << user->money();
}
