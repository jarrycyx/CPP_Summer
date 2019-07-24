//总体框架：             Storage - Data - Model - Interaction - View
//该类所处层级：     Data
/************************************************************************************************************************
类名：     MyArticleObj
功能：     本地存储文章内容，实现对文章修改、状态更改等操作方法
备注：     statusCode各状态意义如下

                    100	已上传，招募负责人开始
                    110	已标记负责人，招募负责人结束
                    120	开始招募译者
                    130	招募译者结束，即将分配任务（若忽略则直接修改为200）
                    140	已拆分
                    ---	200	子文章已创建（或已忽略拆分的文章）
                    ---	210	已标记翻译者，译者开始翻译
                    ---	220	译者正在根据负责人意见修改
                    ---	230	子文章译文评审通过
                    ---	400	子文章生命周期完成
                    300	子文章合并完成
                    310	负责人提交
                    320	发送者已收取
                    330	发送者已付款，款项成功分配
                    400	生命周期完成

日期：     20190710 初步实现该类的Sender部分
                 20190715 实现Regulatorr部分
                 20190717 架构大调整，移除数据库部分操作，分离到Storage层级
************************************************************************************************************************/

//TO-DO: 将拆分文章方法封装在此类中


#ifndef MYARTICLEOBJ_H
#define MYARTICLEOBJ_H

#include <QObject>
#include <QSqlQuery>
#include "storageunit.h"

class MyArticleObj : public StorageUnit
{
public:
    MyArticleObj();
    MyArticleObj(const int &sender); //构造函数，传入必须要素

    int setArticleInfo(const int &newId, const QString &title, const QString &content); //未知文章ID，设置新的文章内容

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

    int translatorIdOfArticle() const;
    void setTranslatorIdOfArticle(int id);

    QString translatedTitle() const;
    void setTranslatedTitle(QString newTitle);

    QString translatedContent() const;
    void setTranslatedContent(QString newContent);


signals:
    //文章内容改变后发送的信号
    void titleOfArticleChanged();
    void contentOfArticleChanged();
    void statusCodeOfArticleChanged();

private:
    //文章内容
    QString m_titleOfArticle, m_translatedTitle;
    QString m_contentOfArticle, m_translatedContent;

    int origin_article_id;
    //article身份标识
    int m_article_id;
    /* 备注：本类中article_id不能改变，只能为未设置状态或已设置状态 */

    //定义并设置默认发送者和负责人
    int sender_id;
    int regulator_id;
    int status_code;
    int translator_id;
};

#endif // MYARTICLEOBJ_H
