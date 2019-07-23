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
