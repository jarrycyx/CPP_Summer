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

    int getAnArticleId();
    int getArticlesLength();

    void uploadAllData();

    void addAnArticle(MyArticleObj* newArticle);
    MyArticleObj* getArticleToEdit(int index);


    int userLogin(QString name, QString pswd, int role=1);
    int searchUser(QString name, int role=1);
    int addUser(QString name, QString pswd, int role);


private:
    QVector<MyArticleObj*> allArticles;
    QVector<MyUserObj*> allUsers;

    QSqlQuery* query;

    int biggestArticleId=0, biggestUserId=0;

signals:

public slots:
};

#endif // GLOBALCOMPONENTS_H
