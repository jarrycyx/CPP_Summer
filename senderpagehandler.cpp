#include "senderpagehandler.h"
#include <myarticleobj.h>
#include <myuserobj.h>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include "cpp-Components/globalcomponents.h"

/*************************************************************************
【函数名称】  SenderPageHandler
【函数功能】  构造函数，同时创建数据库连接
【参数】    parent，可以为空
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
SenderPageHandler::SenderPageHandler(int senderId, GlobalComponents *newGlobal, QObject *parent)
    : QObject(parent), senderArticleList(1), allUserArticleList(2), thisUserId(senderId)
{

    globalStorageComponent = newGlobal;
    startLoadingSenderArticleList(senderId);
}

/*************************************************************************
【函数名称】  ~SenderPageHandler
【函数功能】  析构函数，删除动态分配的内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
SenderPageHandler::~SenderPageHandler()
{
}

void SenderPageHandler::startLoadingSenderArticleList(int userId)
{
    senderArticleList.removeAllArticles();
    allUserArticleList.removeAllArticles();

    qDebug() << "sender" << userId;
    int len = globalStorageComponent->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        qDebug() << "sender article";
        if (globalStorageComponent->getArticleToEdit(i)->senderIdOfArticle() == userId)
            senderArticleList.addAnArticle(globalStorageComponent->getArticleToEdit(i));
        if (globalStorageComponent->getArticleToEdit(i)->statusCodeOfArticle() / 100 != 2)
            allUserArticleList.addAnArticle(globalStorageComponent->getArticleToEdit(i));
    }
}

void SenderPageHandler::addSenderArticle(QString title, QString content)
{
    MyArticleObj *newSenderArticle = new MyArticleObj(thisUserId);
    newSenderArticle->setArticleInfo(globalStorageComponent->getAnArticleId(), title, content);
    newSenderArticle->setStatusCodeOfArticle(100);
    newSenderArticle->setModifyStatus(1);
    //由于需要显示两处，保存一处，故需要增加三处
    //allArticles.push_front(newSenderArticle);
    globalStorageComponent->addAnArticle(newSenderArticle);
    senderArticleList.addAnArticle(newSenderArticle);
    allUserArticleList.addAnArticle(newSenderArticle);
    emit sendSuccessMessage("文章已上传");
}

void SenderPageHandler::editSenderArticle(int index, QString title, QString content)
{
    //由于数据实体只保存一份，只需编辑一处即可
    senderArticleList.editAnArticle(index, title, content);

    emit sendSuccessMessage("已保存");
}

void SenderPageHandler::deleteSenderArticle(int index)
{
    senderArticleList.deleteAnArticle(index);
    emit sendSuccessMessage("已删除");
}

/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void SenderPageHandler::startPage(QQmlApplicationEngine *engine)
{
    thisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("senderPageHandler", this);
    thisContext->setContextProperty("senderArticleList", &senderArticleList);
    thisContext->setContextProperty("allUserArticleList", &allUserArticleList);
    thisContext->setContextProperty("userListModel", &requestUserList);
    const QUrl url1(QStringLiteral("qrc:/SenderPage.qml"));
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

/*************************************************************************
【函数名称】  chooseRegulator
【函数功能】  添加/修改文章负责人
【参数】    文章在列表中的位置index
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::chooseRegulator(int index)
{
    loadArticleRegulatorData(senderArticleList.getArticle(index)->articleIdOfArticle());
    currentInViewIndex = index;
    const QUrl url(QStringLiteral("qrc:/ChooseUserMiniPage.qml"));
    thisEngine->load(url);
}

void SenderPageHandler::loadArticleRegulatorData(int articleId)
{
    qDebug() << "choose" << articleId;

    requestUserList.removeAllRequestUsers();
    int numOfRequest = globalStorageComponent->getRequestsLength();
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = globalStorageComponent->getRequest(i);
        if (getRequest->getArticleId() == articleId && getRequest->getType() == 1)
        {
            MyUserObj *requestUser = globalStorageComponent->searchUserById(getRequest->getUserId());
            requestUserList.addARequestUser(requestUser);
        }
    }
}

/*************************************************************************
【函数名称】  regulatorChosen
【函数功能】  交给MyArticleObj存储并同步负责人信息
【参数】    idx，选中的负责人在model中的位置
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::regulatorChosen(int idx)
{
    MyArticleObj *articleToChoose = senderArticleList.getArticle(currentInViewIndex);
    articleToChoose->setRegulatorIdOfArticle(requestUserList.getRequestUser(idx)->userId());
    articleToChoose->setStatusCodeOfArticle(110);
    senderArticleList.editAnArticle(currentInViewIndex);

    emit sendSuccessMessage("已确定负责人");
}
