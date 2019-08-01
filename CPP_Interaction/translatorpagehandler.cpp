//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     TranslatorPageHandler
功能：     翻译者页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190722 实现收取文章和报名功能
          20190723 实现保存翻译功能
************************************************************************************************************************/

#include "translatorpagehandler.h"
#include "../CPP_Data/myarticleobj.h"
#include "../CPP_Data/myuserobj.h"
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include "../CPP_Storage/globalstoragecomponents.h"

/*************************************************************************
名称：     TranslatorPageHandler
功能：     构造函数
参数：     parent，可以为空
日期：     20190712
*************************************************************************/
TranslatorPageHandler::TranslatorPageHandler(int translatorId)
    : AbstractPage(translatorId), mTranslatorSubarticleList(1),
      mAllSeekingTranslatorArticleList(2)
{
    startLoadingTranslatorArticleList(translatorId);
}

/*************************************************************************
名称：     ~TranslatorPageHandler
功能：     析构函数，删除动态分配的内存
日期：     20190718
*************************************************************************/
TranslatorPageHandler::~TranslatorPageHandler()
{
}

/*************************************************************************
名称：     startLoadingTranslatorArticleList
功能：     加载所需显示的数据，根据需求将Data组合为Model
参数：     翻译者用户Id
返回：     无
日期：     20190717 初步实现
          20190718 增加查看其他人的文章功能
*************************************************************************/
void TranslatorPageHandler::startLoadingTranslatorArticleList(int userId)
{
    qDebug() << "Translator" << userId;
    int len = storage->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        qDebug() << "Translator article";

        MyArticleObj *selectedArticle = storage->getArticleToEdit(i);
        if (selectedArticle->translatorIdOfArticle() == userId && selectedArticle->statusCodeOfArticle() != 400)
        {
            if (selectedArticle->statusCodeOfArticle() / 100 == 2)
                if (selectedArticle->statusCodeOfArticle() != 240)
                    mTranslatorSubarticleList.addAnArticle(selectedArticle);
        }

        if (selectedArticle->statusCodeOfArticle() == 120)
            mAllSeekingTranslatorArticleList.addAnArticle(selectedArticle);
    }
}

/*************************************************************************
名称：     editAnArticle
功能：     编辑译文（译文初始与原文相同，由翻译者逐段替换）
参数：     文章标题和内容
返回：     无
日期：     20190712
*************************************************************************/
Q_INVOKABLE void TranslatorPageHandler::editTranslatedArticle(int index, QString tTitle, QString tContent)
{
    qDebug() << "save" << index;
    mTranslatorSubarticleList.getArticle(index)->setTranslatedTitle(tTitle);
    mTranslatorSubarticleList.getArticle(index)->setStatusCodeOfArticle(215);
    storage->sendMessageToRelatedUser(
        QString("%1").arg(storage->decodeStatusCode(215)),
        mTranslatorSubarticleList.getArticle(index));

    mTranslatorSubarticleList.getArticle(index)->setTranslatedContent(tContent);
    mTranslatorSubarticleList.editAnArticle(index);
    //发送信号，通知QML显示成功信息
    emit sendSuccessMessage("译文已保存");
}

/*************************************************************************
名称：     signForTranslatorArticle
功能：     报名做翻译者
参数：     在列表中的位置index
返回：     无
日期：     20190722
*************************************************************************/
Q_INVOKABLE void TranslatorPageHandler::signForTranslatorArticle(int index)
{
    int thisUserCredit = storage->searchUserById(mThisUserId)->credit();
    if (thisUserCredit >= 25)
    {                              //检查积分
        int alreadySigned = false; //检查是否已报名
        int len = storage->getRequestsLength();
        for (int i = 0; i < len; i++)
        {
            MyRequestObj *selected = storage->getRequest(i);
            if (selected->getUserId() == mThisUserId //是否存在该用户发送的，文章Id相同的，申请成为译者的请求
                && selected->getType() == 2
                    && selected->getArticleId() ==
                    mAllSeekingTranslatorArticleList.getArticle(index)->articleIdOfArticle())
            {
                alreadySigned = true; //标记暂存
            }
        }

        if (!alreadySigned)
        {
            qDebug() << "sign up for" << index;
            MyRequestObj *sendNewRequest = new MyRequestObj(
                storage->getARequestId(),
                mThisUserId,
                mAllSeekingTranslatorArticleList.getArticle(index)->articleIdOfArticle(),
                2); //2表示成为译者的请求
            sendNewRequest->setModifyStatus(StorageUnit::New);
            storage->addARequest(sendNewRequest);
            emit sendSuccessMessage("报名成功"); //发送信号，通知QML显示操作成功或失败通知
        }
        else
        {
            emit sendErrorMessage("已经报名"); //发送信号，通知QML显示操作成功或失败通知
        }
    }
    else
    {
        emit sendErrorMessage("抱歉，积分25以上才能报名成为翻译者"); //发送信号，通知QML显示操作成功或失败通知
    }
}

/*************************************************************************
名称：     getRegulatorComment
功能：     显示负责人发送的历史反馈信息
参数：     在列表中的位置idx
返回：     多条反馈信息组合为的QString
日期：     20190725
*************************************************************************/
Q_INVOKABLE QString TranslatorPageHandler::getRegulatorComment(int index)
{ //此处index指在正在翻译的文章列表中的索引
    qDebug() << "get comment " << index;
    int numOfRequest = storage->getRequestsLength();
    QString commentStr = "";
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = storage->getRequest(i);
        if (getRequest->getArticleId() ==
                mTranslatorSubarticleList.getArticle(index)->articleIdOfArticle()
                && getRequest->getType() == 3) //type为3表示反馈信息
        {
            commentStr += getRequest->getContent() + "\n\n"; //增加换行，调整显示格式
        }
    }
    return commentStr;
}

/*************************************************************************
名称：     startPage
功能：     开始渲染主页面
参数：     QQmlApplicationEngine *engine
返回：     无
日期：     20190717
*************************************************************************/
void TranslatorPageHandler::startPage(QQmlApplicationEngine *engine)
{
    mThisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("translatorPageHandler", this);
    thisContext->setContextProperty("translatorSubarticleList", &mTranslatorSubarticleList);
    thisContext->setContextProperty("allSeekingTranslatorArticleList", &mAllSeekingTranslatorArticleList);
    const QUrl url1(QStringLiteral("qrc:/QML/MainPages/TranslatorPage.qml"));
    engine->load(url1);
}
