#ifndef GLOBALCOMPONENTS_H
#define GLOBALCOMPONENTS_H

#include <QObject>
#include "../cppobjs/myarticleobj.h"
#include "articleslist.h"
#include "../cppobjs/myuserobj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>

class GlobalComponents : public QObject
{
    Q_OBJECT
public:
    explicit GlobalComponents( QQmlApplicationEngine *engine, QObject *parent = nullptr);
    virtual ~GlobalComponents();
    void uploadAllData();


    inline int getAnArticleId(){biggestArticleId++;return biggestArticleId;}

    inline int getArticlesLength(){return allArticles.length();}

    inline MyArticleObj* getArticleToEdit(int index){return allArticles[index];}

    inline void addAnArticle(MyArticleObj* newArticle){allArticles.push_front(newArticle);}



    inline int getAUserId(){biggestUserId++;return biggestUserId;}

    inline int getUsersLength(){return allUsers.length();}

    inline MyUserObj* getUserToEdit(int index){return allUsers[index];}

    inline void addAUser(MyUserObj* newUser){allUsers.push_front(newUser);}



    //int userLogin(QString name, QString pswd, int role=1);
    //int searchUser(QString name, int role=1);
    //int addUser(QString name, QString pswd, int role);


private:
    QVector<MyArticleObj*> allArticles;
    QVector<MyUserObj*> allUsers;

    QSqlQuery* query;

    int biggestArticleId=0, biggestUserId=0;

signals:

public slots:
};

#endif // GLOBALCOMPONENTS_H
