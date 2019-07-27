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
【函数名称】  TranslatorPageHandler
【函数功能】  构造函数，同时创建数据库连接
【参数】    parent，可以为空
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
TranslatorPageHandler::TranslatorPageHandler(int translatorId, QObject *parent)
    : AbstractPage(translatorId), translatorSubarticleList(1),
      allSeekingTranslatorArticle(2)
{
    startLoadingTranslatorArticleList(translatorId);
}

/*************************************************************************
【函数名称】  ~TranslatorPageHandler
【函数功能】  析构函数，删除动态分配的内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
TranslatorPageHandler::~TranslatorPageHandler()
{
}

void TranslatorPageHandler::startLoadingTranslatorArticleList(int userId)
{
    qDebug() << "Translator" << userId;
    int len = storage->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        qDebug() << "Translator article";

        MyArticleObj* selectedArticle = storage->getArticleToEdit(i);
        if (selectedArticle->translatorIdOfArticle() == userId
                && selectedArticle->statusCodeOfArticle() != 400)
        {
            if (selectedArticle->statusCodeOfArticle() / 100 == 2)
                if (selectedArticle->statusCodeOfArticle() != 240)
                    translatorSubarticleList.addAnArticle(selectedArticle);
        }

        if (selectedArticle->statusCodeOfArticle() == 120)
            allSeekingTranslatorArticle.addAnArticle(selectedArticle);
    }
}

Q_INVOKABLE void TranslatorPageHandler::editTranslatedArticle(int index, QString tTitle, QString tContent)
{
    qDebug() << "save" << index;
    translatorSubarticleList.getArticle(index)->setTranslatedTitle(tTitle);
    translatorSubarticleList.getArticle(index)->setStatusCodeOfArticle(215);
    storage->sendMessageToRelatedUser(
                QString("%1").arg(storage->decodeStatusCode(215)),
                translatorSubarticleList.getArticle(index));

    translatorSubarticleList.getArticle(index)->setTranslatedContent(tContent);
    translatorSubarticleList.editAnArticle(index);

    emit sendSuccessMessage("译文已保存");
}

Q_INVOKABLE void TranslatorPageHandler::signForTranslatorArticle(int index)
{
    qDebug() << "sign up for" << index;
    MyRequestObj *sendNewRequest = new MyRequestObj(
                storage->getARequestId(),
                thisUserId,
                allSeekingTranslatorArticle.getArticle(index)->articleIdOfArticle(),
                2); //2表示成为译者的请求
    sendNewRequest->setModifyStatus(StorageUnit::New);
    storage->addARequest(sendNewRequest);
    emit sendSuccessMessage("报名成功");
}


Q_INVOKABLE QString TranslatorPageHandler::getRegulatorComment(int index)//此处index指在正在翻译的文章列表中的索引
{
    qDebug() << "get comment " << index;
    int numOfRequest = storage->getRequestsLength();
    QString commentStr = "";
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = storage->getRequest(i);
        if (getRequest->getArticleId() == translatorSubarticleList.getArticle(index)->articleIdOfArticle()
                && getRequest->getType() == 3)
        {
            commentStr += getRequest->getContent() + "\n\n";
        }
    }
    return commentStr;
}

/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void TranslatorPageHandler::startPage(QQmlApplicationEngine *engine)
{
    thisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("translatorPageHandler", this);
    thisContext->setContextProperty("translatorSubarticleList", &translatorSubarticleList);
    thisContext->setContextProperty("allSeekingTranslatorArticle", &allSeekingTranslatorArticle);
    const QUrl url1(QStringLiteral("qrc:/QML/MainPages/TranslatorPage.qml"));
    engine->load(url1);
}

/*************************************************************************
【函数名称】  itemMove
【函数功能】  删除文章，执行MyArticleObj的删除操作
【参数】    idx，该文章在model中的位置
【返回值】   无
【开发者及日期】    jarrycyx 20190716
*************************************************************************/

/*************************************************************************
【函数名称】  addAnArticle
【函数功能】  增加文章，先创建文章的Object存取信息，再交给MyArticleObj去同步数据
【参数】    文章标题和内容
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/

/*************************************************************************
【函数名称】  editAnArticle
【函数功能】  修改文章
【参数】    文章标题和内容
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
