#ifndef MYARTICLEOBJ_H
#define MYARTICLEOBJ_H

#include <QObject>
#include <QSqlQuery>
#include "storageunit.h"


class MyArticleObj: public StorageUnit
{
public:
    MyArticleObj();
    MyArticleObj(const int &sender);     //构造函数，传入必须要素

    int setArticleInfo(const int &newId, const QString &title, const QString &content);    //未知文章ID，设置新的文章内容

    //以下为读取和设置文章内容各函数
    int senderIdOfArticle() const;
    int setSenderIdOfArticle(int code);
    QString titleOfArticle() const;
    void setTitleOfArticle(const QString &name);
    int articleIdOfArticle() const;
    void setArticleIdOfArticle(const int &id);
    QString contentOfArticle() const;
    void setContentOfArticle(const QString &color);
    int statusCodeOfArticle() const;
    int setStatusCodeOfArticle(int code);
    int regulatorIdOfArticle() const;
    void setRegulatorIdOfArticle(const int &regulatorId);
    int originArticleIdOfArticle() const;
    void setOriginArticleIdOfArticle(const int &articleId);
    int addArticleToRemoteDBReturnId();
    void updateArticleInfoToRemote();

signals:
    //文章内容改变后发送的信号
    void titleOfArticleChanged();
    void contentOfArticleChanged();
    void statusCodeOfArticleChanged();


private:
    //文章内容
    QString m_titleOfArticle;
    QString m_contentOfArticle;

    int origin_article_id;
    //article身份标识
    int m_article_id;
    /* 备注：本类中article_id不能改变，只能为未设置状态或已设置状态 */

    //定义并设置默认发送者和负责人
    int sender_id;
    int regulator_id;
    int status_code;
};

#endif // MYARTICLEOBJ_H
