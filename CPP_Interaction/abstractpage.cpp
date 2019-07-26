#include "abstractpage.h"
#include "userinfopagehandler.h"
#include "../CPP_Storage/globalcomponents.h"
#include <QObject>

AbstractPage::AbstractPage(int newUserId,GlobalComponents *newGlobal, QObject *parent)
    :QObject(parent), globalStorageComponent(newGlobal), thisUserId(newUserId)
{

}

Q_INVOKABLE void AbstractPage::showUserInfo(){
    UserInfoPageHandler* newUserHandler = new UserInfoPageHandler(
                globalStorageComponent->searchUserById(thisUserId),
                globalStorageComponent);
    newUserHandler->startPage(thisEngine);
}
