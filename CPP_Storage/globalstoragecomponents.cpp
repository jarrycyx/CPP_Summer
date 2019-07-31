#include "globalstoragecomponents.h"
#include "../CPP_Model/articleslist.h"
#include "../CPP_Data/MyArticleObj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QSqlQuery>
#include "../CPP_Data/myuserobj.h"
#include "../CPP_Data/myrequestobj.h"
#include <QtConcurrent/QtConcurrent>
#include <QSqlError>

GlobalStorageComponents::GlobalStorageComponents(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    f = QtConcurrent::run(this, &GlobalStorageComponents::refreshDataInNewThread);
    //f.waitForFinished();
}


/*************************************************************************
名称：     ~GlobalStorageComponents
功能：     析构函数，释放内存，等待线程执行结束
参数：     无
返回：     无
日期：     20190707 初步实现
          20190711 实现释放对象列表的内存
*************************************************************************/
GlobalStorageComponents::~GlobalStorageComponents()
{
    //标志位通知线程结束
    ifEndingApp = true;
    //阻塞等待线程结束
    f.waitForFinished();

    //逐个释放内存空间
    int lenOfArticles = getArticlesLength();
    for (int i=0;i<lenOfArticles;i++)
        delete allArticles[i];
    int lenOfRequests = getRequestsLength();
    for (int i=0;i<lenOfRequests;i++)
        delete allRequests[i];
    int lenOfUsers = getUsersLength();
    for (int i=0;i<lenOfUsers;i++)
        delete allUsers[i];
}


/*************************************************************************
名称：     refreshDataInNewThread
功能：     在后台线程中运行的函数，用于下载，刷新并上传数据
参数：     无
返回：     无
日期：     20190728
          20190729 增加自动重连功能
          20190730 增加连接失败提示功能
*************************************************************************/
void GlobalStorageComponents::refreshDataInNewThread(){
    while (!downloadAllData()){ //尝试连接直到连接成功
        QThread::msleep(200);
        emit sendErrorMessage("正在连接服务器");
    }
    emit sendSuccessMessage("连接服务器成功");
    QThread::msleep(500);
    emit sendSuccessMessage("");
    //每次查询是否结束程序
    while (!ifEndingApp){
        QThread::msleep(200);
        uploadAllData();
    }
    delete query;
}

/*************************************************************************
名称：     downloadAllData
功能：     从服务器下载数据
参数：     无
返回：     无
日期：     20190707 初步实现
          20190711 封装到该类
          2019071X 逐步增加Request等功能
          20190729 增加自动重连功能
*************************************************************************/
int GlobalStorageComponents::downloadAllData()
{
    QMutex mutex;                                                   //线程锁，确保一次刷新不会被破坏
    mutex.lock();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("39.106.107.241");                               //远程服务器地址
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1;MYSQL_OPT_READ_TIMEOUT=5;MYSQL_OPT_WRITE_TIMEOUT=8");
    if (!db.open()){
        qDebug() << "Failed to connect to root mysql admin";
        mutex.unlock();
        return 0;
    }
    else
    {
        db.setConnectOptions();
        qDebug() << "open";
        query = new QSqlQuery(db);
        query->exec(QString("SELECT article_id,title,content,create_time,sender,"
                            "regulator,translator,curr_status,origin,t_title,t_content,fee "
                            "from articles"));
        while (query->next())
        {
            MyArticleObj *articleFromDB = new MyArticleObj();       //新分配内存，析构函数中释放
            articleFromDB->setArticleInfo(query->value(0).toInt(),
                                          query->value(1).toString(),
                                          query->value(2).toString());
            articleFromDB->setSenderIdOfArticle(query->value(4).toInt());
            articleFromDB->setRegulatorIdOfArticle(query->value(5).toInt());
            articleFromDB->setTranslatorIdOfArticle(query->value(6).toInt());
            articleFromDB->setStatusCodeOfArticle(query->value(7).toInt());
            articleFromDB->setOriginArticleIdOfArticle(query->value(8).toInt());
            articleFromDB->setTranslatedTitle(query->value(9).toString());
            articleFromDB->setTranslatedContent(query->value(10).toString());
            articleFromDB->setFee(query->value(11).toInt());

            articleFromDB->setModifyStatus(StorageUnit::Unchanged); //从服务器下载的数据标记为未更改

            if (query->value(0).toInt() > biggestUserId)
                biggestArticleId = query->value(0).toInt();
            allArticles.append(articleFromDB);
        }

        query->exec(QString("SELECT user_id,user_name,create_time,password,money,credit,quali"
                            " from users"));
        while (query->next())
        {
            MyUserObj *userFromDB = new MyUserObj(
                        query->value(0).toInt(),
                        query->value(1).toString(),
                        query->value(3).toString());
            userFromDB->setMoney(query->value(4).toInt());
            userFromDB->setCredit(query->value(5).toInt());
            userFromDB->setQualification(query->value(6).toString());
            userFromDB->setModifyStatus(StorageUnit::Unchanged);
            allUsers.append(userFromDB);

            if (query->value(0).toInt() > biggestUserId)
                biggestUserId = query->value(0).toInt();
        }

        query->exec(QString("SELECT request_id,user_id,article_id,content,type,time FROM requests"));
        while (query->next())
        {
            MyRequestObj *requestFromDB = new MyRequestObj(
                        query->value(0).toInt(),
                        query->value(1).toInt(),
                        query->value(2).toInt(),
                        query->value(4).toInt());
            requestFromDB->setTime(query->value(5).toDateTime());
            requestFromDB->setContent(query->value(3).toString());
            allRequests.append(requestFromDB);

            if (query->value(0).toInt() > biggestRequestId)
                biggestRequestId = query->value(0).toInt();
        }
    }
    mutex.unlock();
    return 1;
}

/*************************************************************************
名称：     uploadAllData
功能：     检测更改并上传数据
参数：     无
返回：     无
日期：     20190707 初步实现
          20190711 封装到该类
          2019071X 逐步增加Request等功能
          20190728 增加多线程后台刷新
          20190730 增加错误检查机制
          20190730 增加自动重连功能
*************************************************************************/
void GlobalStorageComponents::uploadAllData()
{
    QMutex mutex;
    mutex.lock();
    //qDebug() << "uploading" << db.isOpen() << query->isActive();

    int artiLen = allArticles.length();
    for (int i = 0; i < artiLen; i++)
    {
        int modifyStat = allArticles[i]->getModifyStatus();
        if (modifyStat == StorageUnit::New)                             //新增的数据对象
        {
            qDebug() << "added article";
            query->exec(QString("insert into articles "
                                "(title,content,create_time,sender,regulator,translator,"
                                "curr_status,article_id,origin,t_title,t_content,fee) "
                                "values "
                                "(\"%1\", \"%2\", NOW(), %3, %4, %5, %6, %7, %8, \"%9\", \"%10\", %11)")
                        .arg(allArticles[i]->titleOfArticle())
                        .arg(allArticles[i]->contentOfArticle())
                        .arg(allArticles[i]->senderIdOfArticle())
                        .arg(allArticles[i]->regulatorIdOfArticle())
                        .arg(allArticles[i]->translatorIdOfArticle())
                        .arg(allArticles[i]->statusCodeOfArticle())
                        .arg(allArticles[i]->articleIdOfArticle())
                        .arg(allArticles[i]->originArticleIdOfArticle())
                        .arg(allArticles[i]->translatedTitle())
                        .arg(allArticles[i]->translatedContent())
                        .arg(allArticles[i]->fee()));
        }
        else if (modifyStat == StorageUnit::Changed)                    //修改的数据对象
        {
            qDebug() << "modified article";
            query->exec(QString("UPDATE articles SET title=\"%1\", content=\"%2\" ,sender=%3, "
                                "regulator=%4, translator=%5, curr_status=%6, origin=%7, t_title=\"%8\", "
                                "t_content=\"%9\", fee=%10 "
                                "WHERE article_id=%11")
                        .arg(allArticles[i]->titleOfArticle())
                        .arg(allArticles[i]->contentOfArticle())
                        .arg(allArticles[i]->senderIdOfArticle())
                        .arg(allArticles[i]->regulatorIdOfArticle())
                        .arg(allArticles[i]->translatorIdOfArticle())
                        .arg(allArticles[i]->statusCodeOfArticle())
                        .arg(allArticles[i]->originArticleIdOfArticle())
                        .arg(allArticles[i]->translatedTitle())
                        .arg(allArticles[i]->translatedContent())
                        .arg(allArticles[i]->fee())
                        .arg(allArticles[i]->articleIdOfArticle()));
        }
        else if (modifyStat == StorageUnit::Deleted)                    //删除的数据对象
        {
            qDebug() << "deleted article";
            query->exec(QString("DELETE FROM articles WHERE article_id=%1").arg(allArticles[i]->articleIdOfArticle()));
        }
        //检查是否出错
        QSqlError error=query->lastError();
        if (!error.isValid())                                           //若上传成功则清除“更改”或“新增”标记
            allArticles[i]->setModifyStatus(StorageUnit::Unchanged);
        else {
            qDebug() << error.text();
            qDebug() << "网络断开，正在重新连接";
            emit sendErrorMessage("服务器无响应");
        }
    }

    int userLen = allUsers.length();
    for (int i = 0; i < userLen; i++)
    {
        int modifyStat = allUsers[i]->getModifyStatus();
        if (modifyStat == StorageUnit::New)
        {
            qDebug() << "added user";
            query->exec(QString("insert into users "
                                "(user_id,user_name,create_time,password,money,credit,quali) values "
                                "(%1, \"%2\", NOW(), \"%3\", %4, %5, \"%6\")")
                        .arg(allUsers[i]->userId())
                        .arg(allUsers[i]->username())
                        .arg(allUsers[i]->password())
                        .arg(allUsers[i]->money())
                        .arg(allUsers[i]->credit())
                        .arg(allUsers[i]->qualification()));
        }
        else if (modifyStat == StorageUnit::Changed)
        {
            qDebug() << "modified user";
            query->exec(QString("UPDATE users SET user_name=\"%2\" ,password=\"%3\", "
                                "money=%4, credit=%5, quali=\"%6\" WHERE user_id=%1")
                        .arg(allUsers[i]->userId())
                        .arg(allUsers[i]->username())
                        .arg(allUsers[i]->password())
                        .arg(allUsers[i]->money())
                        .arg(allUsers[i]->credit())
                        .arg(allUsers[i]->qualification()));
        }
        QSqlError error=query->lastError();
        if (!error.isValid())
            allUsers[i]->setModifyStatus(StorageUnit::Unchanged);
        else {
            qDebug() << error.text();
            qDebug() << "网络断开，正在重新连接";
            emit sendErrorMessage("服务器无响应");
        }
    }

    int requestLen = allRequests.length();
    for (int i = 0; i < requestLen; i++)
    {
        int modifyStat = allRequests[i]->getModifyStatus();
        if (modifyStat == StorageUnit::New)
        {
            qDebug() << "added request";
            query->exec(QString("insert into requests "
                                "(request_id, user_id, article_id, time, content, type) "
                                "values "
                                "(%1, %2, %3, NOW(), \"%4\", %5)")
                        .arg(allRequests[i]->getRequestId())
                        .arg(allRequests[i]->getUserId())
                        .arg(allRequests[i]->getArticleId())
                        .arg(allRequests[i]->getContent())
                        .arg(allRequests[i]->getType()));
        }
        QSqlError error=query->lastError();
        if (!error.isValid())
            allRequests[i]->setModifyStatus(StorageUnit::Unchanged);
        else {
            qDebug() << error.text();
            qDebug() << "网络断开，正在重新连接";
            emit sendErrorMessage("服务器无响应");
        }
    }
    mutex.unlock();
}


/*************************************************************************
名称：     searchUserById
功能：     通过用户Id查找文章指针，性能较低
参数：     用户id
返回：     用户对象指针
日期：     20190722
*************************************************************************/
MyUserObj *GlobalStorageComponents::searchUserById(int thisUserId)
{
    int len = allUsers.length();
    for (int i = 0; i < len; i++)
        if (allUsers[i]->userId() == thisUserId)
            return allUsers[i];
    return nullptr;
}


/*************************************************************************
名称：     searchArticleById
功能：     通过文章Id查找文章指针，性能较低
参数：     文章id
返回：     文章对象指针
日期：     20190722
*************************************************************************/
MyArticleObj *GlobalStorageComponents::searchArticleById(int thisArticleId)
{
    int len = allArticles.length();
    for (int i = 0; i < len; i++)
        if (allArticles[i]->articleIdOfArticle() == thisArticleId)
            return allArticles[i];
    return nullptr;
}



/*************************************************************************
名称：     sendMessageToRelatedUser
功能：     向一篇文章相关的用户发送消息
参数：     消息字符串，对应文章
返回：     无
日期：     20190727
*************************************************************************/
void GlobalStorageComponents::sendMessageToRelatedUser(QString str, MyArticleObj* articleInChange)
{
    QString titleStr=articleInChange->titleOfArticle();
    if (titleStr.length()>12) titleStr=titleStr.mid(0,12)+"...";
    qDebug() << "Send message " << str;
    if (articleInChange->senderIdOfArticle()>0){
        MyRequestObj* newRequest = new MyRequestObj(getARequestId(),
                                                    articleInChange->senderIdOfArticle(),
                                                    articleInChange->articleIdOfArticle(),
                                                    4);
        newRequest->setContent(QString("您发布的文章 %1 状态：").arg(titleStr)+str);
        newRequest->setModifyStatus(StorageUnit::New);
        addARequest(newRequest);
    }
    if (articleInChange->regulatorIdOfArticle()>0){
        MyRequestObj* newRequest = new MyRequestObj(getARequestId(),
                                                    articleInChange->regulatorIdOfArticle(),
                                                    articleInChange->articleIdOfArticle(),
                                                    4);
        newRequest->setContent(QString("您负责的文章 %1 状态：").arg(titleStr)+str);
        newRequest->setModifyStatus(StorageUnit::New);
        addARequest(newRequest);
    }
    if (articleInChange->translatorIdOfArticle()>0){
        MyRequestObj* newRequest = new MyRequestObj(getARequestId(),
                                                    articleInChange->translatorIdOfArticle(),
                                                    articleInChange->articleIdOfArticle(),
                                                    4);
        newRequest->setContent(QString("您翻译的文章 %1 状态：").arg(titleStr)+str);
        newRequest->setModifyStatus(StorageUnit::New);
        addARequest(newRequest);
    }
}


void GlobalStorageComponents::sendUserModifiedMessage(int userId ,QString content){
    MyRequestObj* newRequest = new MyRequestObj(
                getARequestId(),
                userId,
                -1,
                4);
    newRequest->setContent(content);
    newRequest->setModifyStatus(StorageUnit::New);
    addARequest(newRequest);
}


/*************************************************************************
名称：     decodeStatusCode
功能：     工具函数，根据状态号，解析对应的含义
参数：     状态号
返回：     中文含义字符串
日期：     20190728
*************************************************************************/
QString GlobalStorageComponents::decodeStatusCode(int code){
    switch (code) {
    case (100): return "已上传，招募负责人开始";
    case (110): return "已标记负责人，招募负责人结束";
    case (120): return "开始招募译者";
    case (130): return "招募译者结束，即将分配任务";
    case (140): return "已拆分";
    case (200): return "子文章已创建";
    case (210): return "已标记翻译者";
    case (215): return "译者开始翻译";
    case (220): return "译者正在根据负责人意见修改";
    case (230): return "子文章译文评审通过";
    case (240): return "子文章生命周期完成";
    case (300): return "子文章合并完成";
    case (310): return "负责人提交";
    case (320): return "发送者已收取";
    case (330): return "发送者已付款，款项成功分配";
    case (400): return "生命周期完成";
    }
    return "";
}
