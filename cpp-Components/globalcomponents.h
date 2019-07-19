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
    void startLoadingSenderArticleList(int userId);

    void addSenderArticle(int sender, QString title, QString content);
    void editSenderArticle(int index, QString title, QString content);
    void deleteSenderArticle(int index);
    void loadArticleRegulatorData(int articleId);


    int userLogin(QString name, QString pswd, int role=1);
    int searchUser(QString name, int role=1);
    int addUser(QString name, QString pswd, int role);


private:
    ArticlesList senderArticleList, allUserArticleList;
    QVector<MyArticleObj*> allArticles;
    QVector<MyUserObj*> allUsers;

    QSqlQuery* query;

    int biggestArticleId=0, biggestUserId=0;

signals:

public slots:
};

#endif // GLOBALCOMPONENTS_H
