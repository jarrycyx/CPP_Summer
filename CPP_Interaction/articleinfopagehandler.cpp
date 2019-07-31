//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Interaction
/************************************************************************************************************************
类名：     ArticleInfoPageHandler
功能：     文章信息弹出框后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190726 实现查看文章信息
************************************************************************************************************************/


#include "articleinfopagehandler.h"
#include <QDebug>


/*************************************************************************
名称：     ArticleInfoPageHandler
功能：     构造函数，初始化信息
参数：     需要查看的文章对象指针
返回：     无
日期：     20190727
*************************************************************************/
ArticleInfoPageHandler::ArticleInfoPageHandler(MyArticleObj* thisArticle) :
    AbstractPage(-1), mArticle(thisArticle)
{
    qDebug() << "load mArticle info";
    int mSenderId = mArticle->senderIdOfArticle();
    int mRegulatorId = mArticle->regulatorIdOfArticle();
    int mTranslatorId = mArticle->translatorIdOfArticle();

    //获得用户id后，需要向Storage查询
    mSender = storage->searchUserById(mSenderId);
    mRegulator = storage->searchUserById(mRegulatorId);
    mTranslator = storage->searchUserById(mTranslatorId);

}

/*************************************************************************
名称：     startPage
功能：     开始渲染QML界面，并绑定后台流程类
参数：     Qt加载引擎指针
返回：     无
日期：     20190726 实现
          20190727 修改为重载函数
*************************************************************************/
void ArticleInfoPageHandler::startPage(QQmlApplicationEngine *engine)
{
    engine->rootContext()->setContextProperty("articleInfoPageHandler", this);
    engine->load(QUrl(QStringLiteral("qrc:/QML/OtherPages/ArticleInfoPage.qml")));
}

/*************************************************************************
名称：     getSenderName等
功能：     获取文章各信息
日期：     20190726
*************************************************************************/
Q_INVOKABLE QString ArticleInfoPageHandler::getSenderName()
{
    if (mSender != nullptr)
        return mSender->username();
    else return QString("无信息");
}
Q_INVOKABLE QString ArticleInfoPageHandler::getRegulatorName()
{
    if (mRegulator != nullptr)
        return mRegulator->username();
    else return QString("无信息");
}
Q_INVOKABLE QString ArticleInfoPageHandler::getTranslatorName()
{
    if (mTranslator != nullptr)
        return mTranslator->username();
    else return QString("无信息");
}

Q_INVOKABLE int ArticleInfoPageHandler::getMoney()
{
    return mArticle->fee();
}
