#include "abstractpage.h"
#include "userinfopagehandler.h"
#include "../CPP_Storage/globalstoragecomponents.h"
#include "articleinfopagehandler.h"
#include <QObject>
#include <QDebug>


//设置全局静态变量，用于流程类中查询存储单元
GlobalStorageComponents* AbstractPage::storage = new GlobalStorageComponents;



/*************************************************************************
名称：     AbstractPage
功能：     构造函数
参数：     用户ID，parent（可以为空）
日期：     20190726
*************************************************************************/
AbstractPage::AbstractPage(int newUserId, QObject *parent)
    :QObject(parent), mThisUserId(newUserId)
{
    QObject::connect(storage, SIGNAL(sendErrorMessage(QString)), this, SIGNAL(sendErrorMessage(QString)));
    QObject::connect(storage, SIGNAL(sendSuccessMessage(QString)), this, SIGNAL(sendSuccessMessage(QString)));
}


/*************************************************************************
名称：     ~AbstractPage
功能：     析构函数，删除动态分配的内存
日期：     20190727
*************************************************************************/
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
                storage->searchUserById(mThisUserId));
    //开始渲染
    newUserHandler->startPage(mThisEngine);
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
    newArticlePage->startPage(mThisEngine);
}
