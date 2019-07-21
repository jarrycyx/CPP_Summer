#include "globalcomponents.h"
#include "articleslist.h"
#include "../cppobjs/MyArticleObj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QSqlQuery>
#include "../cppobjs/myuserobj.h"
#include "../cppobjs/myrequestobj.h"

GlobalComponents::GlobalComponents(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("39.106.107.241");
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else {
        qDebug() << "open";
        query=new QSqlQuery(db);

        query->exec(QString("SELECT article_id,title,content,create_time,sender,regulator,translator,curr_status from articles"));
        while (query->next()){
            MyArticleObj* articleFromDB = new MyArticleObj();
            articleFromDB->setArticleInfo(query->value(0).toInt(),query->value(1).toString(),query->value(2).toString());
            articleFromDB->setSenderIdOfArticle(query->value(4).toInt());
            articleFromDB->setRegulatorIdOfArticle(query->value(5).toInt());
            //articleFromDB->setSenderIdOfArticle(query->value(4).toInt());
            articleFromDB->setStatusCodeOfArticle(query->value(7).toInt());

            articleFromDB->setModifyStatus(0);

            if (query->value(0).toInt()>biggestUserId) biggestArticleId=query->value(0).toInt();
            allArticles.append(articleFromDB);
        }


        query->exec(QString("SELECT user_id,user_name,create_time,password,role from users"));
        while (query->next()){
            MyUserObj* userFromDB = new MyUserObj(
                        query->value(0).toInt(),
                        query->value(1).toString(),
                        query->value(3).toString(),
                        query->value(4).toInt());
            allUsers.append(userFromDB);

            if (query->value(0).toInt()>biggestUserId) biggestUserId=query->value(0).toInt();
        }


        query->exec(QString("SELECT request_id,user_id,article_id,content,type FROM requests"));
        while (query->next()){
            MyRequestObj* requestFromDB = new MyRequestObj(
                        query->value(0).toInt(),
                        query->value(1).toInt(),
                        query->value(2).toInt(),
                        query->value(4).toInt());
            allRequests.append(requestFromDB);

            if (query->value(0).toInt()>biggestRequestId) biggestRequestId=query->value(0).toInt();
        }
    }

}

GlobalComponents::~GlobalComponents(){
    uploadAllData();
}



void GlobalComponents::uploadAllData(){
    qDebug() << "uploading";
    int artiLen=allArticles.length();
    for (int i=0;i<artiLen;i++){
        int modifyStat = allArticles[i]->getModifyStatus();
        if (modifyStat==1){
            qDebug() << "added article";
            query->exec(QString("insert into articles "
                                "(title,content,create_time,sender,regulator,translator,curr_status,article_id) values "
                                    "(\"%1\", \"%2\", NOW(), %3, %4, %5, %6, %7)")
                        .arg(allArticles[i]->titleOfArticle())
                        .arg(allArticles[i]->contentOfArticle())
                        .arg(allArticles[i]->senderIdOfArticle())
                        .arg(allArticles[i]->regulatorIdOfArticle())
                        .arg(-1)
                        .arg(allArticles[i]->statusCodeOfArticle())
                        .arg(allArticles[i]->articleIdOfArticle()));
        }
        else if (modifyStat==2){
            qDebug() << "modified article";
            query->exec(QString("UPDATE articles SET title=\"%1\", content=\"%2\" ,sender=%3, "
                                "regulator=%4, translator=%5, curr_status=%6 WHERE article_id=%7")
                        .arg(allArticles[i]->titleOfArticle())
                        .arg(allArticles[i]->contentOfArticle())
                        .arg(allArticles[i]->senderIdOfArticle())
                        .arg(allArticles[i]->regulatorIdOfArticle())
                        .arg(-1)
                        .arg(allArticles[i]->statusCodeOfArticle())
                        .arg(allArticles[i]->articleIdOfArticle()));
        }else if (modifyStat==3){
            qDebug() << "deleted article";
            query->exec(QString("DELETE FROM articles WHERE article_id=%1").arg(allArticles[i]->articleIdOfArticle()));
        }

    }

    int userLen=allUsers.length();
    for (int i=0;i<userLen;i++){
        int modifyStat = allUsers[i]->getModifyStatus();
        if (modifyStat==1){
            qDebug() << "added user";
            query->exec(QString("insert into users "
                                "(user_id,user_name,create_time,password,role) values "
                                    "(%1, \"%2\", NOW(), \"%3\", %4)")
                        .arg(allUsers[i]->userId())
                        .arg(allUsers[i]->username())
                        .arg(allUsers[i]->password())
                        .arg(allUsers[i]->role()));
        }
        else if (modifyStat==2){
            qDebug() << "modified user";
            query->exec(QString("UPDATE users SET user_name=\"%2\" ,password=\"%3\", role=%4 WHERE user_id=%1")
                        .arg(allUsers[i]->userId())
                        .arg(allUsers[i]->username())
                        .arg(allUsers[i]->password())
                        .arg(allUsers[i]->role()));
        }
    }


    int requestLen=allRequests.length();
    for (int i=0;i<requestLen;i++){
        int modifyStat = allRequests[i]->getModifyStatus();
        if (modifyStat == 1){
            qDebug() << "added request";
            query->exec(QString("insert into requests "
                                "(request_id, user_id, article_id, time, content, type) "
                                "values "
                                "(%1, %2, %3, NOW(), \"\", %4)")
                        .arg(allRequests[i]->getRequestId())
                        .arg(allRequests[i]->getUserId())
                        .arg(allRequests[i]->getArticleId())
                        .arg(allRequests[i]->getType()));
        }
    }
}


MyUserObj* GlobalComponents::searchUserById(int thisUserId){
    int len = allUsers.length();
    for (int i=0;i<len;i++)
        if (allUsers[i]->userId()==thisUserId)
            return allUsers[i];
    return nullptr;
}
