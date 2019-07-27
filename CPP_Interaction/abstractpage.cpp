#include "abstractpage.h"
#include "userinfopagehandler.h"
#include "../CPP_Storage/globalstoragecomponents.h"
#include "articleinfopagehandler.h"
#include <QObject>
#include <QDebug>

GlobalStorageComponents* AbstractPage::storage = new GlobalStorageComponents;

AbstractPage::AbstractPage(int newUserId, QObject *parent)
    :QObject(parent), thisUserId(newUserId)
{

}

AbstractPage::~AbstractPage(){
    if (storage!=nullptr) delete storage;
}

Q_INVOKABLE void AbstractPage::showUserInfo()
{
    UserInfoPageHandler* newUserHandler = new UserInfoPageHandler(
                storage->searchUserById(thisUserId));
    newUserHandler->startPage(thisEngine);
}


Q_INVOKABLE void AbstractPage::showArticleInfo(int articleId)
{
    qDebug() << "show article" << articleId;
    ArticleInfoPageHandler* newArticlePage = new ArticleInfoPageHandler(
                storage->searchArticleById(articleId));
    newArticlePage->startPage(thisEngine);
}
