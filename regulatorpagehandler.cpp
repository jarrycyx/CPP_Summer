#include "regulatorpagehandler.h"
#include <myarticleobj.h>
#include <myuserobj.h>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include "cpp-Components/globalcomponents.h"

/*************************************************************************
【函数名称】  RegulatorPageHandler
【函数功能】  构造函数，同时创建数据库连接
【参数】    parent，可以为空
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
RegulatorPageHandler::RegulatorPageHandler(int regulatorId, GlobalComponents *newGlobal, QObject *parent)
    : QObject(parent), regulatorSubarticleList(1),
      regulatorArticleList(1), allSeekingRegulatorArticle(2), thisUserId(regulatorId)
{

    globalStorageComponent = newGlobal;
    startLoadingRegulatorArticleList(regulatorId);
}

/*************************************************************************
【函数名称】  ~RegulatorPageHandler
【函数功能】  析构函数，删除动态分配的内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
RegulatorPageHandler::~RegulatorPageHandler()
{
}

void RegulatorPageHandler::startLoadingRegulatorArticleList(int userId)
{
    qDebug() << "Regulator" << userId;
    int len = globalStorageComponent->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        qDebug() << "Regulator article";
        if (globalStorageComponent->getArticleToEdit(i)
                ->regulatorIdOfArticle() == userId)
        {
            if (globalStorageComponent->getArticleToEdit(i)->statusCodeOfArticle() / 100 == 2)
                regulatorSubarticleList.addAnArticle(globalStorageComponent->getArticleToEdit(i));
            else
                regulatorArticleList.addAnArticle(globalStorageComponent->getArticleToEdit(i));
        }

        if (globalStorageComponent->getArticleToEdit(i)
                ->statusCodeOfArticle() == 100)
            allSeekingRegulatorArticle.addAnArticle(globalStorageComponent->getArticleToEdit(i));
    }
}

void RegulatorPageHandler::splitRegulatorArticle(int index, QString title, QString content)
{
    qDebug() << "split" << index;
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(140);
    regulatorArticleList.editAnArticle(index); //刷新文章状态
    QStringList subContents = content.split("\n");
    qDebug() << subContents << index;
    for (int i = 0; i < subContents.length(); i++)
        if (subContents[i].length() == 0)
            subContents.removeAt(i);
    int numOfSubarticles = subContents.length();
    for (int i = 0; i < numOfSubarticles; i++)
    {
        QString subTitle = title + " 子任务" + QString("%1").arg(i);
        MyArticleObj *newSubArticle = new MyArticleObj(thisUserId);
        newSubArticle->setArticleInfo(globalStorageComponent->getAnArticleId(), subTitle, subContents[i]);
        newSubArticle->setTranslatedTitle(subTitle);
        newSubArticle->setTranslatedContent(subContents[i]);
        newSubArticle->setStatusCodeOfArticle(200);
        newSubArticle->setRegulatorIdOfArticle(thisUserId);
        newSubArticle->setOriginArticleIdOfArticle(regulatorArticleList.getArticle(index)->articleIdOfArticle());
        newSubArticle->setModifyStatus(StorageUnit::New);

        globalStorageComponent->addAnArticle(newSubArticle);
        regulatorSubarticleList.addAnArticle(newSubArticle);
    }
    emit sendSuccessMessage("文章已拆分");
}



Q_INVOKABLE void RegulatorPageHandler::mergeRegulatorArticle(int index){
    int originArticleId = regulatorSubarticleList.getArticle(index)->originArticleIdOfArticle();

    //通过子文章翻译标题得到原文章翻译标题
    QString subTitle=regulatorSubarticleList.getArticle(index)->translatedTitle();


    //先用一个list暂存子文章后，再进行排序
    QList<QString> listOfSubContent;

    qDebug() << "merge to " << originArticleId;
    int len = globalStorageComponent->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        //寻找原文章的其他子文章
        if (globalStorageComponent->getArticleToEdit(i)
                ->originArticleIdOfArticle() == originArticleId)
        {
            //判断其他子文章是否已经审核通过
            if (globalStorageComponent->getArticleToEdit(i)->statusCodeOfArticle() == 230)
            {
                MyArticleObj* thisSubArticle = globalStorageComponent->getArticleToEdit(i);
                QString subTransContent=thisSubArticle->translatedContent();
                QString subTitle=thisSubArticle->titleOfArticle();
                thisSubArticle->setStatusCodeOfArticle(240);
                qDebug() << subTransContent << subTitle << subTitle[subTitle.length()-1];
                QChar indexChar=subTitle[subTitle.length()-1];//子文章标题最后的序号
                if (!indexChar.isNumber())
                {
                    emit sendErrorMessage("未知错误");
                    return;
                }
                listOfSubContent.insert(indexChar.unicode()-'0', subTransContent);
            }else
            {
                emit sendErrorMessage("其他子文章未审核通过");
                return;
            }
        }
    }
    QString transContent;
    int lenOfList = listOfSubContent.length();
    for (int i=0;i<lenOfList;i++)
        transContent+=listOfSubContent[i];
    //查找拆分前的文章
    MyArticleObj* originAriticle = globalStorageComponent->searchArticleById(originArticleId);
    //修改文章状态，增加译文
    originAriticle->setTranslatedTitle(subTitle);
    originAriticle->setTranslatedContent(transContent);
    originAriticle->setStatusCodeOfArticle(300);

    //刷新视图
    regulatorArticleList.refreshAll();
    regulatorSubarticleList.refreshAll();
}

Q_INVOKABLE void RegulatorPageHandler::editArticle(int index, QString title, QString content)
{
    qDebug() << "save" << index;
    regulatorArticleList.editAnArticle(index, title, content);

    emit sendSuccessMessage("已保存");
}


Q_INVOKABLE void RegulatorPageHandler::chooseTranslator(int index)
{
    loadArticleTranslatorData(regulatorSubarticleList.getArticle(index)->originArticleIdOfArticle());
    currentInViewIndex = index;
    const QUrl url(QStringLiteral("qrc:/ChooseUserMiniPage.qml"));
    thisEngine->load(url);
}

Q_INVOKABLE void RegulatorPageHandler::loadArticleTranslatorData(int originArticleId){
    qDebug() << "choose origin" << originArticleId;

    translatorList.removeAllRequestUsers();
    int numOfRequest = globalStorageComponent->getRequestsLength();
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = globalStorageComponent->getRequest(i);
        if (getRequest->getArticleId() == originArticleId
                && getRequest->getType() == 2)
        {
            MyUserObj *requestUser = globalStorageComponent->searchUserById(getRequest->getUserId());
            translatorList.addARequestUser(requestUser);
        }
    }
}


Q_INVOKABLE void RegulatorPageHandler::translatorChosen(int idx){
    MyArticleObj *articleToChoose = regulatorSubarticleList.getArticle(currentInViewIndex);
    articleToChoose->setTranslatorIdOfArticle(translatorList.getRequestUser(idx)->userId());

    qDebug() << translatorList.getRequestUser(idx)->userId() << "choosen";

    articleToChoose->setStatusCodeOfArticle(210);
    regulatorSubarticleList.editAnArticle(currentInViewIndex);

    emit sendSuccessMessage("已确定译者");
}


Q_INVOKABLE void RegulatorPageHandler::signForRegulatorArticle(int index)
{
    qDebug() << "sign up for" << index;
    MyRequestObj *sendNewRequest = new MyRequestObj(
        globalStorageComponent->getARequestId(),
        thisUserId,
        allSeekingRegulatorArticle.getArticle(index)->articleIdOfArticle(),
        1); //1表示成为负责人的请求
    sendNewRequest->setModifyStatus(StorageUnit::New);
    globalStorageComponent->addARequest(sendNewRequest);
    emit sendSuccessMessage("报名成功");
}

Q_INVOKABLE void RegulatorPageHandler::startRecruitingTranslatorForArticle(int index)
{
    qDebug() << "chooseTranslatorForArticle" << index;
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(120);
    regulatorArticleList.editAnArticle(index);
    emit sendSuccessMessage("开始招募");
}

Q_INVOKABLE void RegulatorPageHandler::stopRecruitingTranslatorForArticle(int index)
{
    qDebug() << "stopRecruitingTranslatorForArticle" << index;
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(130);
    regulatorArticleList.editAnArticle(index); //刷新文章状态
    emit sendSuccessMessage("停止招募");
}


Q_INVOKABLE void RegulatorPageHandler::commentToTranslator(int idx, QString comment){
    qDebug() << idx << " " << comment;
    MyRequestObj* newRequest = new MyRequestObj(globalStorageComponent->getARequestId(),
                                                thisUserId,
                                                regulatorSubarticleList.getArticle(idx)->articleIdOfArticle(),
                                                3);
    regulatorSubarticleList.getArticle(idx)->setStatusCodeOfArticle(220);
    regulatorSubarticleList.editAnArticle(idx);
    newRequest->setContent(comment);
    newRequest->setModifyStatus(StorageUnit::New);
    globalStorageComponent->addARequest(newRequest);

    emit sendSuccessMessage("反馈成功");
}


Q_INVOKABLE QString RegulatorPageHandler::getHistoryComment(int idx){
    qDebug() << "load comment" << idx;

    int numOfRequest = globalStorageComponent->getRequestsLength();
    QString commentStr = "";
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = globalStorageComponent->getRequest(i);
        if (getRequest->getArticleId() == regulatorSubarticleList.getArticle(idx)->articleIdOfArticle()
                && getRequest->getType() == 3)
        {
            commentStr += getRequest->getContent();
        }
    }
    return commentStr;
}


Q_INVOKABLE void RegulatorPageHandler::acceptSubarticle(int idx)
{
    regulatorSubarticleList.getArticle(idx)->setStatusCodeOfArticle(230);
    regulatorSubarticleList.editAnArticle(idx);
    emit sendSuccessMessage("已审核通过");
}

/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void RegulatorPageHandler::startPage(QQmlApplicationEngine *engine)
{
    thisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("regulatorPageHandler", this);
    thisContext->setContextProperty("regulatorArticleList", &regulatorArticleList);
    thisContext->setContextProperty("regulatorSubarticleList", &regulatorSubarticleList);
    thisContext->setContextProperty("allSeekingRegulatorArticle", &allSeekingRegulatorArticle);
    thisContext->setContextProperty("userListModel", &translatorList);
    const QUrl url1(QStringLiteral("qrc:/RegulatorPage.qml"));
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
