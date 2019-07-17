#include "senderpagehandler.h"
#include <myarticleobj.h>
#include <myuserobj.h>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>

/*************************************************************************
【函数名称】  SenderPageHandler
【函数功能】  构造函数，同时创建数据库连接
【参数】    parent，可以为空
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
SenderPageHandler::SenderPageHandler(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setHostName("39.106.107.241");
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else {
        qDebug() << "open";
        query=new QSqlQuery(db);
    }
}

/*************************************************************************
【函数名称】  ~SenderPageHandler
【函数功能】  析构函数，删除动态分配的内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
SenderPageHandler::~SenderPageHandler(){
    int lenThis=myThisModel.length();
    int lenOther=myOtherModel.length();
    int lenUser=myRegulatorListModel.length();
    for (int i=0;i<lenThis;i++)
        delete myThisModel[i];
    for (int i=0;i<lenOther;i++)
        delete myOtherModel[i];
    for (int i=0;i<lenUser;i++)
        delete myRegulatorListModel[i];
    delete query;
}


/*************************************************************************
【函数名称】  refreshPage
【函数功能】  刷新页面，内部调用huhuo 从QML调用
【参数】    无
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::refreshPage(){
    emit startRefreshQml();
    startLoadingData(2, thisUserId);
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
【函数名称】  thisModel，otherModel，regulatorListModel
【函数功能】  向QML传递ListView所需数据
【参数】   无
【返回值】   转换为QVariant类型的QList
【开发者及日期】    jarrycyx 2019-7-12
*************************************************************************/
QVariant SenderPageHandler::thisModel() const{
    return QVariant::fromValue(myThisModel);
}

QVariant SenderPageHandler::otherModel() const{
    return QVariant::fromValue(myOtherModel);
}

QVariant SenderPageHandler::regulatorListModel() const{
    return QVariant::fromValue(myRegulatorListModel);
}

/*************************************************************************
【函数名称】  itemMove
【函数功能】  删除文章，执行MyArticleObj的删除操作
【参数】    idx，该文章在model中的位置
【返回值】   无
【开发者及日期】    jarrycyx 20190716
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::itemMove(int idx){
    MyArticleObj *articleToRemove = qobject_cast<MyArticleObj*>(myThisModel.takeAt(idx));
    qDebug()<<myThisModel.length()<<"itemMoveInCpp"<<articleToRemove->articleIdOfArticle();
    query->exec(QString("delete from articles where article_id=%1")
                .arg(articleToRemove->articleIdOfArticle()));
    //startLoadingData(1, thisUserId);
    refreshPage();
}



/*************************************************************************
【函数名称】       startLoadingData
【函数功能】       向服务器请求数据并加载到model
【参数】          flag用于标识登录请求，user_id传递登录用户信息
【返回值】         无
【开发者及日期】    jarrycyx 20190713
*************************************************************************/
void SenderPageHandler::startLoadingData(int flag, int user_id){
    if (flag&&query){           //如果为登录请求，且服务器连接正常
        myThisModel.clear();    //清空list，避免重复加载数据
        thisUserId=user_id;     //保存userid到该对象中
        qDebug()<<"data";
        query->exec(QString("select article_id from articles where sender=%1 ORDER BY create_time DESC")
                    .arg(user_id));
                                //读取所需文章的身份标志
        qDebug()<<"data1";
        while(query->next()){
            qDebug()<<"data2";
            MyArticleObj *newArticleToPull = new MyArticleObj(query, thisUserId ,0);    //创建空文章
            newArticleToPull->setArticleIdOfArticle(query->value(0).toInt());           //设置空文章身份标志，便于之后读取数据
            myThisModel.append(newArticleToPull);                                       //加入list的model中
        }
        int len = myThisModel.length();
        for (int i=0; i<len;i++){
            MyArticleObj *pullAnArticle = qobject_cast<MyArticleObj*>(myThisModel[i]);
            pullAnArticle->pullArticleInfo(pullAnArticle->articleIdOfArticle());        //存储Article的对象开始向服务器请求文章具体内容数据
        }

        qDebug()<<"complete signal";

        //同上，但为加载其他用户的文章列表
        myOtherModel.clear();
        query->exec(QString("select article_id from articles ORDER BY create_time DESC"));
        while(query->next()){
            qDebug()<<"data2";
            MyArticleObj *pullOtherArticle = new MyArticleObj(query, thisUserId ,0);
            pullOtherArticle->setArticleIdOfArticle(query->value(0).toInt());
            myOtherModel.append(pullOtherArticle);
        }
        len = myOtherModel.length();
        for (int i=0; i<len;i++){
            MyArticleObj *pullAnArticle = qobject_cast<MyArticleObj*>(myOtherModel[i]);
            pullAnArticle->pullArticleInfo(pullAnArticle->articleIdOfArticle());
        }

        qDebug()<<"complete signal";
    }

    //通知QML进行刷新
    emit refreshQmlComplete();
}


/*************************************************************************
【函数名称】  addAnArticle
【函数功能】  增加文章，先创建文章的Object存取信息，再交给MyArticleObj去同步数据
【参数】    文章标题和内容
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::addAnArticle(QString title, QString content){
    emit startRefreshQml();
    MyArticleObj *newArticle = new MyArticleObj(query, thisUserId, 0);
    articleSendingId = newArticle->setNewArticleInfo(title, content);
    if (articleSendingId!=-1) myThisModel.push_front(newArticle);
    emit refreshQmlComplete();
    currentInViewIndex=0;

    myRegulatorListModel.clear();
    query->exec(QString("select user_id, user_name, password from users WHERE role=2 ORDER BY create_time DESC"));
    while(query->next()){
        qDebug()<<"data2";
        myRegulatorListModel.append(new MyUserObj(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString()));
    }

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
    currentInViewIndex=index;
    MyArticleObj *newArticle = qobject_cast<MyArticleObj*>(myThisModel[index]);
    qDebug() << index<< " Chosen" <<articleSendingId<<newArticle->articleIdOfArticle();
    newArticle->setTitleOfArticle(title);
    newArticle->setContentOfArticle(content);
    refreshPage();
}


/*************************************************************************
【函数名称】  chooseRegulator
【函数功能】  添加/修改文章负责人
【参数】    文章在列表中的位置
【返回值】   无
【开发者及日期】    jarrycyx 20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::chooseRegulator(int index){
    myRegulatorListModel.clear();
    query->exec(QString("select user_id, user_name, password from users WHERE role=2 ORDER BY create_time DESC"));
    while(query->next()){
        qDebug()<<"data2";
        myRegulatorListModel.append(new MyUserObj(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString()));
    }
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
    MyUserObj *regulatorToChoose = qobject_cast<MyUserObj*>(myRegulatorListModel[idx]);
    MyArticleObj *newArticle = qobject_cast<MyArticleObj*>(myThisModel[currentInViewIndex]);
    qDebug() << idx<< " Chosen" << regulatorToChoose->userId()<<articleSendingId<<newArticle->articleIdOfArticle();
    newArticle->setRegulatorIdOfArticle(regulatorToChoose->userId());
   // emit refreshQmlComplete();
    refreshPage();
}
