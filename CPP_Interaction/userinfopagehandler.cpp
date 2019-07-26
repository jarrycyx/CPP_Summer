#include "userinfopagehandler.h"
#include <QDebug>

UserInfoPageHandler::UserInfoPageHandler(MyUserObj* thisUser, GlobalComponents *newGlobal, QObject *parent) :
    QObject(parent), user(thisUser), globalStorageComponent(newGlobal)
{

}

void UserInfoPageHandler::startPage(QQmlApplicationEngine *engine)
{
    engine->rootContext()->setContextProperty("userPageHandler", this);
    engine->rootContext()->setContextProperty("messageList", &messageList);
    engine->load(QUrl(QStringLiteral("qrc:/QML/OtherPages/UserInfoPage.qml")));

    qDebug() << "load user messagebox";
    int len = globalStorageComponent->getRequestsLength();
    for (int i = 0; i < len; i++)
    {
        if (globalStorageComponent->getRequest(i)->getUserId() == user->userId()
                && globalStorageComponent->getRequest(i)->getType() == 4){
            messageList.addAMessage(globalStorageComponent->getRequest(i));
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
    int len = globalStorageComponent->getUsersLength();
    for (int i=0;i<len;i++){
        if (globalStorageComponent->getUserToEdit(i)->username() == user->username()){
            if (globalStorageComponent->getUserToEdit(i)->userId() != user->userId()){
                res[globalStorageComponent->getUserToEdit(i)->role()-1]='2';
            }
        }
    }
    return res;
}
