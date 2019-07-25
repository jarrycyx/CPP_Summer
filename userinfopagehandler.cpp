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
    engine->load(QUrl(QStringLiteral("qrc:/UserInfoPage.qml")));

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
