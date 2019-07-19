#include "globalcomponents.h"
#include "articleslist.h"
#include "../cppobjs/MyArticleObj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QSqlQuery>
#include "../cppobjs/myuserobj.h"

GlobalComponents::GlobalComponents(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent)
{
    engine->rootContext()->setContextProperty("senderArticleList", &senderArticleList);
    engine->rootContext()->setContextProperty("allUserArticleList", &allUserArticleList);

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
            allUserArticleList.addAnArticle(articleFromDB);
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
}


void GlobalComponents::startLoadingSenderArticleList(int userId){
    qDebug() << "sender" << userId;
    int len=allArticles.length();
    for (int i=0;i<len;i++){
        qDebug() << "sender article";
        if (allArticles[i]->senderIdOfArticle()==userId)
            senderArticleList.addAnArticle(allArticles[i]);
    }
}



void GlobalComponents::loadArticleRegulatorData(int articleId){
    qDebug() << "article" << articleId;
    int len=allUsers.length();
    for (int i=0;i<len;i++){
        qDebug() << "sender article";
        if (allUsers[i]->role()==2){}
            //myRegulatorListModel.append(allUsers[i]);
    }
}




int GlobalComponents::userLogin(QString name, QString pswd, int role){
    int len=allUsers.length();
    for (int i=0; i<len; i++){
        if (name==allUsers[i]->username()){
            if (role==allUsers[i]->role()){
                if (pswd==allUsers[i]->password())
                    return 1;
                else return 2;//密码不匹配
            }else return 3;//用户身份不匹配
        }else return 4;//用户未注册
    }
    return 0;
}



int GlobalComponents::searchUser(QString name, int role){
    int len=allUsers.length();
    for (int i=0; i<len; i++){
        if (name==allUsers[i]->username())
            if (role==allUsers[i]->role())
                return allUsers[i]->userId();
    }
    return -1;
}

int GlobalComponents::addUser(QString name, QString pswd, int role){
    int len=allUsers.length();
    for (int i=0; i<len; i++){
        if (allUsers[i]->username()==name&&allUsers[i]->role()==role)
            return 0;//用户已存在
    }
    qDebug() << "注册ID" << biggestUserId + 2;
    MyUserObj* newUser = new MyUserObj(biggestUserId+2 ,name, pswd, role);
    newUser->setModifyStatus(1);
    biggestUserId+=2;
    allUsers.push_front(newUser);
    return 1;
}



void GlobalComponents::addSenderArticle(int sender, QString title, QString content){
    MyArticleObj* newSenderArticle = new MyArticleObj(sender);
    newSenderArticle->setArticleInfo(biggestArticleId+1, title, content);
    newSenderArticle->setStatusCodeOfArticle(100);
    newSenderArticle->setModifyStatus(1);
    biggestArticleId+=1;
    //由于需要显示两处，保存一处，故需要增加三处
    allArticles.push_front(newSenderArticle);
    senderArticleList.addAnArticle(newSenderArticle);
    allUserArticleList.addAnArticle(newSenderArticle);
}



void GlobalComponents::editSenderArticle(int index, QString title, QString content){
    //由于数据实体只保存一份，只需编辑一处即可
    senderArticleList.editAnArticle(index, title, content);
}

void GlobalComponents::deleteSenderArticle(int index){
    senderArticleList.deleteAnArticle(index);
}


