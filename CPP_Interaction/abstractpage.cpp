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
    //释放空间
    if (storage!=nullptr) delete storage;
}

/*************************************************************************
名称：     showUserInfo
功能：     启动显示用户信息的界面
参数：     无
返回：     无
日期：     20190726
*************************************************************************/
Q_INVOKABLE void AbstractPage::showUserInfo()
{
    //创建新页面指针，传入参数
    UserInfoPageHandler* newUserHandler = new UserInfoPageHandler(
                storage->searchUserById(thisUserId));
    //开始渲染
    newUserHandler->startPage(thisEngine);
}

/*************************************************************************
名称：     showArticleInfo
功能：     启动显示文章信息的界面
参数：     无
返回：     无
日期：     20190726
*************************************************************************/
Q_INVOKABLE void AbstractPage::showArticleInfo(int articleId)
{
    qDebug() << "show article" << articleId;
    ArticleInfoPageHandler* newArticlePage = new ArticleInfoPageHandler(
                storage->searchArticleById(articleId));
    newArticlePage->startPage(thisEngine);
}
