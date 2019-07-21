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
RegulatorPageHandler::RegulatorPageHandler(int regulatorId, GlobalComponents* newGlobal, QObject *parent) : QObject(parent), thisUserId(regulatorId)
{

    globalStorageComponent = newGlobal;
    startLoadingRegulatorArticleList(regulatorId);
}

/*************************************************************************
【函数名称】  ~RegulatorPageHandler
【函数功能】  析构函数，删除动态分配的内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
RegulatorPageHandler::~RegulatorPageHandler(){

}


void RegulatorPageHandler::startLoadingRegulatorArticleList(int userId){
    qDebug() << "Regulator" << userId;
    int len=globalStorageComponent->getArticlesLength();
    for (int i=0;i<len;i++){
        qDebug() << "Regulator article";
        if (globalStorageComponent->getArticleToEdit(i)->regulatorIdOfArticle()==userId)
            regulatorArticleList.addAnArticle(globalStorageComponent->getArticleToEdit(i));
        if (globalStorageComponent->getArticleToEdit(i)->statusCodeOfArticle()==100)
            allSeekingRegulatorArticle.addAnArticle(globalStorageComponent->getArticleToEdit(i));
    }
}


void RegulatorPageHandler::splitRegulatorArticle(int index, QString title, QString content){
    qDebug() << "split" << index;
}


Q_INVOKABLE void RegulatorPageHandler::editArticle(int index, QString title, QString content){
    qDebug() << "save" << index;
    regulatorArticleList.editAnArticle(index, title, content);
}


Q_INVOKABLE void RegulatorPageHandler::signForRegulatorArticle(int index){
    qDebug() << "sign up for" << index;
    MyRequestObj *sendNewRequest = new MyRequestObj(
                globalStorageComponent->getARequestId(),
                thisUserId,
                globalStorageComponent->getArticleToEdit(index)->articleIdOfArticle(),
                1);//1表示成为负责人的请求
    sendNewRequest->setModifyStatus(1);
    globalStorageComponent->addARequest(sendNewRequest);
}


Q_INVOKABLE void RegulatorPageHandler::startRecruitingTranslatorForArticle(int index){
    qDebug() << "chooseTranslatorForArticle" << index;
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(120);
    regulatorArticleList.editAnArticle(index);
}




/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void RegulatorPageHandler::startPage(QQmlApplicationEngine *engine){
    thisEngine = engine;
    QQmlContext *thisContext=engine->rootContext();
    thisContext->setContextProperty("regulatorPageHandler", this);
    thisContext->setContextProperty("regulatorArticleList", &regulatorArticleList);
    thisContext->setContextProperty("allSeekingRegulatorArticle", &allSeekingRegulatorArticle);
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
