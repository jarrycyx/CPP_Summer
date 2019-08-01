//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     SupervisorPageHandler
功能：     审核人页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190727 初步实现
************************************************************************************************************************/

#include "supervisorpagehandler.h"
#include "../CPP_Data/myarticleobj.h"
#include "../CPP_Data/myuserobj.h"
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include "../CPP_Storage/globalstoragecomponents.h"
#include "userinfopagehandler.h"

/*************************************************************************
名称：     SupervisorPageHandler
功能：     构造函数
参数：     用户Id
日期：     20190712
*************************************************************************/
SupervisorPageHandler::SupervisorPageHandler(int supervisorId)
    : AbstractPage(supervisorId), mSupervisorSubarticleList(1)
{
    startLoadingSupervisorArticleList(supervisorId);
}

/*************************************************************************
名称：     ~SupervisorPageHandler
功能：     析构函数，删除动态分配的内存
日期：     20190718
*************************************************************************/
SupervisorPageHandler::~SupervisorPageHandler()
{
}

/*************************************************************************
名称：     startPage
功能：     开始渲染主页面
参数：     QQmlApplicationEngine *engine
返回：     无
日期：     20190712 初步实现
          20190720 多次调整名称
*************************************************************************/
void SupervisorPageHandler::startPage(QQmlApplicationEngine *engine)
{
    mThisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("supervisorPageHandler", this);
    thisContext->setContextProperty("supervisorSubarticleList", &mSupervisorSubarticleList);
    const QUrl url1(QStringLiteral("qrc:/QML/MainPages/SupervisorPage.qml"));
    engine->load(url1);
}

/*************************************************************************
名称：     startLoadingSupervisorArticleList
功能：     加载审核人页面需要显示的文章列表
参数：     用户Id
返回：     无
日期：     20190727 初步实现
*************************************************************************/
void SupervisorPageHandler::startLoadingSupervisorArticleList(int userId)
{
    qDebug() << "Supervisor" << userId;
    //轮询查找
    int len = storage->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        MyArticleObj *selectedArticle = storage->getArticleToEdit(i);
        if (selectedArticle->statusCodeOfArticle() == 215 
            || selectedArticle->statusCodeOfArticle() == 220)
        {
            mSupervisorSubarticleList.addAnArticle(selectedArticle);
            qDebug() << "subarticle " << selectedArticle->articleIdOfArticle();
        }
    }
}

/*************************************************************************
名称：     acceptSubarticle
功能：     标记子任务为审核通过
参数：     在列表中的位置idx
返回：     无
日期：     20190721 初步实现
          20190727 添加积分功能
*************************************************************************/
Q_INVOKABLE void SupervisorPageHandler::acceptSubarticle(int idx)
{
    if (storage->searchUserById(mThisUserId)->credit() >= 65)
    {

        mSupervisorSubarticleList.getArticle(idx)->setStatusCodeOfArticle(230);
        storage->sendMessageToRelatedUser(
            QString("%1").arg(storage->decodeStatusCode(230)),
            mSupervisorSubarticleList.getArticle(idx));
        mSupervisorSubarticleList.editAnArticle(idx);
        int translatorId = mSupervisorSubarticleList.getArticle(idx)->translatorIdOfArticle();
        storage->searchUserById(translatorId)->addCredit(1);
        emit sendSuccessMessage("已审核通过，译者积分+1");
        storage->sendUserModifiedMessage(translatorId, QString("您的账户余额已改变，请注意"));
    }
    else
        emit sendErrorMessage("抱歉，积分65分以上才能履行监管者职能");
}

/*************************************************************************
名称：     commentToTranslator
功能：     向翻译者发送反馈信息
参数：     在列表中的位置idx，反馈内容comment
返回：     无
日期：     20190722
*************************************************************************/
Q_INVOKABLE void SupervisorPageHandler::commentToTranslator(int idx, QString comment)
{
    qDebug() << idx << " " << comment;
    MyRequestObj *newRequest = new MyRequestObj(storage->getARequestId(),
                                                mThisUserId,
                                                mSupervisorSubarticleList.getArticle(idx)->articleIdOfArticle(),
                                                3);
    mSupervisorSubarticleList.getArticle(idx)->setStatusCodeOfArticle(220);
    storage->sendMessageToRelatedUser( //发送状态更新消息
        QString("%1").arg(storage->decodeStatusCode(220)),
        mSupervisorSubarticleList.getArticle(idx));
    mSupervisorSubarticleList.editAnArticle(idx);
    newRequest->setContent(comment);
    newRequest->setModifyStatus(StorageUnit::New);
    storage->addARequest(newRequest);

    emit sendSuccessMessage("反馈成功");
}
