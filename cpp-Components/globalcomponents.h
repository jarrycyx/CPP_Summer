//总体框架：             Storage - Data - Model - Interaction - View
//该类所处层级：     Storage
/************************************************************************************************************************
类名：     GlobalComponents，通用存储封装类
功能：     封装数据库存取操作，也是所有数据实体的存储地
备注：     根据需求对数据库进行整存整取，程序开始时和结束时分别调用构造和析构函数进行下载上传操作
日期：     20190718 初步实现Sender和Regulator
                 20190721 进一步完善，增加Translator和Request
************************************************************************************************************************/

#ifndef GLOBALCOMPONENTS_H
#define GLOBALCOMPONENTS_H

#include <QObject>
#include "../cppobjs/myarticleobj.h"
#include "articleslist.h"
#include "../cppobjs/myuserobj.h"
#include "../cppobjs/myrequestobj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>

class GlobalComponents : public QObject
{
    Q_OBJECT
public:
    explicit GlobalComponents(QObject *parent = nullptr);
    virtual ~GlobalComponents();
    void uploadAllData();

    inline int getAnArticleId()
    {
        biggestArticleId++;
        return biggestArticleId;
    }
    inline int getArticlesLength() { return allArticles.length(); }
    inline MyArticleObj *getArticleToEdit(int index) { return allArticles[index]; }
    inline void addAnArticle(MyArticleObj *newArticle) { allArticles.push_front(newArticle); }

    inline int getAUserId()
    {
        biggestUserId++;
        return biggestUserId;
    }
    inline int getUsersLength() { return allUsers.length(); }
    inline MyUserObj *getUserToEdit(int index) { return allUsers[index]; }
    inline void addAUser(MyUserObj *newUser) { allUsers.push_front(newUser); }
    MyUserObj *searchUserById(int thisUserId);

    inline int getARequestId()
    {
        biggestRequestId++;
        return biggestRequestId;
    }
    inline int getRequestsLength() { return allRequests.length(); }
    inline MyRequestObj *getRequest(int index) { return allRequests[index]; }
    inline void addARequest(MyRequestObj *newRequest) { allRequests.push_front(newRequest); }

private:
    QVector<MyArticleObj *> allArticles;
    QVector<MyUserObj *> allUsers;
    QVector<MyRequestObj *> allRequests;

    QSqlQuery *query;

    int biggestArticleId = 0, biggestUserId = 0, biggestRequestId = 0;

signals:

public slots:
};

#endif // GLOBALCOMPONENTS_H
