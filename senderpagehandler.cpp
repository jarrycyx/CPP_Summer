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
SenderPageHandler::SenderPageHandler(int senderId, GlobalComponents* newGlobal, QObject *parent) : QObject(parent), thisUserId(senderId)
{
    globalStorageComponent = newGlobal;
    globalStorageComponent -> startLoadingSenderArticleList(senderId);
}

/*************************************************************************
【函数名称】  ~SenderPageHandler
【函数功能】  析构函数，删除动态分配的内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
SenderPageHandler::~SenderPageHandler(){

}


/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void SenderPageHandler::startPage(QQmlApplicationEngine *engine){
    thisEngine = engine;
    QQmlContext *thisContext=engine->rootContext();
    thisContext->setContextProperty("senderPageHandler", this);
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


Q_INVOKABLE void SenderPageHandler::itemMove(int idx){
    globalStorageComponent->deleteSenderArticle(idx);
}

/*************************************************************************
【函数名称】  addAnArticle
【函数功能】  增加文章，先创建文章的Object存取信息，再交给MyArticleObj去同步数据
【参数】    文章标题和内容
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/



Q_INVOKABLE void SenderPageHandler::addAnArticle(QString title, QString content){

    globalStorageComponent->addSenderArticle(thisUserId, title, content);

    //添加文章后还需要选取负责人
    const QUrl url(QStringLiteral("qrc:/ChooseRegulatorMiniPage.qml"));
    thisEngine->load(url);
}



/*************************************************************************
【函数名称】  editAnArticle
【函数功能】  修改文章
【参数】    文章标题和内容
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/

Q_INVOKABLE void SenderPageHandler::editAnArticle(int index, QString title, QString content){
    globalStorageComponent->editSenderArticle(index, title, content);
}


/*************************************************************************
【函数名称】  chooseRegulator
【函数功能】  添加/修改文章负责人
【参数】    文章在列表中的位置
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::chooseRegulator(int index){
    /*myRegulatorListModel.clear();
    query->exec(QString("select user_id, user_name, password from users WHERE role=2 ORDER BY create_time DESC"));
    while(query->next()){
        qDebug()<<"data2";
        myRegulatorListModel.append(new MyUserObj(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString()));
    }*/
    globalStorageComponent->loadArticleRegulatorData(0);
    currentInViewIndex=index;
    const QUrl url(QStringLiteral("qrc:/ChooseRegulatorMiniPage.qml"));
    thisEngine->load(url);
}

/*************************************************************************
【函数名称】  regulatorChosen
【函数功能】  交给MyArticleObj存储并同步负责人信息
【参数】    idx，选中的负责人在model中的位置
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/

Q_INVOKABLE void SenderPageHandler::regulatorChosen(int idx){
    /*MyUserObj *regulatorToChoose = qobject_cast<MyUserObj*>(myRegulatorListModel[idx]);
    MyArticleObj *newArticle = qobject_cast<MyArticleObj*>(myThisModel[currentInViewIndex]);
    qDebug() << idx<< " Chosen" << regulatorToChoose->userId()<<articleSendingId<<newArticle->articleIdOfArticle();
    newArticle->setRegulatorIdOfArticle(regulatorToChoose->userId());
   // emit refreshQmlComplete();
    refreshPage();*/
}

