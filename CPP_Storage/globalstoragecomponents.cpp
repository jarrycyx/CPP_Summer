#include "globalstoragecomponents.h"
#include "../CPP_Model/articleslist.h"
#include "../CPP_Data/MyArticleObj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QSqlQuery>
#include "../CPP_Data/myuserobj.h"
#include "../CPP_Data/myrequestobj.h"

GlobalStorageComponents::GlobalStorageComponents(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("39.106.107.241");
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else
    {
        qDebug() << "open";
        query = new QSqlQuery(db);

        query->exec(QString("SELECT article_id,title,content,create_time,sender,"
                            "regulator,translator,curr_status,origin,t_title,t_content,fee "
                            "from articles"));
        while (query->next())
        {
            MyArticleObj *articleFromDB = new MyArticleObj();
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

            articleFromDB->setModifyStatus(StorageUnit::Unchanged);

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
}

GlobalStorageComponents::~GlobalStorageComponents()
{
    uploadAllData();
}

void GlobalStorageComponents::uploadAllData()
{
    qDebug() << "uploading";
    int artiLen = allArticles.length();
    for (int i = 0; i < artiLen; i++)
    {
        int modifyStat = allArticles[i]->getModifyStatus();
        if (modifyStat == StorageUnit::New)
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
        else if (modifyStat == StorageUnit::Changed)
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
        else if (modifyStat == StorageUnit::Deleted)
        {
            qDebug() << "deleted article";
            query->exec(QString("DELETE FROM articles WHERE article_id=%1").arg(allArticles[i]->articleIdOfArticle()));
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
    }
}

MyUserObj *GlobalStorageComponents::searchUserById(int thisUserId)
{
    int len = allUsers.length();
    for (int i = 0; i < len; i++)
        if (allUsers[i]->userId() == thisUserId)
            return allUsers[i];
    return nullptr;
}

MyArticleObj *GlobalStorageComponents::searchArticleById(int thisArticleId)
{
    int len = allArticles.length();
    for (int i = 0; i < len; i++)
        if (allArticles[i]->articleIdOfArticle() == thisArticleId)
            return allArticles[i];
    return nullptr;
}



QList<MyUserObj*> GlobalStorageComponents::searchSubUsers(QString name){
    QList<MyUserObj*> res;
    int len = allUsers.length();
    for (int i=0;i<len;i++){
        if (allUsers[i]->username() == name)
            res.push_back(allUsers[i]);
    }
    return res;
}


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
