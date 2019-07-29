//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data
/************************************************************************************************************************
类名：     MyArticleObj
功能：     本地存储文章内容，实现对文章修改、状态更改等操作方法
          继承StorageUnit类，以实现对修改、增删的标记
备注：     statusCode各状态意义如下

            100	已上传，招募负责人开始
            110	已标记负责人，招募负责人结束
            120	开始招募译者
            130	招募译者结束，即将分配任务（若忽略则直接修改为200）
            140	已拆分
            ---	200	子文章已创建（或已忽略拆分的文章）
            ---	210	已标记翻译者
            ---	215	译者开始翻译
            ---	220	译者正在根据负责人意见修改
            ---	230	子文章译文评审通过
            ---	240	子文章生命周期完成
            300	子文章合并完成
            310	负责人提交
            320	发送者已收取
            330	发送者已付款，款项成功分配
            400	生命周期完成

日期：     20190710 初步实现该类的Sender部分
          20190715 实现Regulator部分
          20190716 将数据库操作封装在该类，并修改数据传入方式
          20190717 架构大调整，移除数据库部分操作，分离到Storage层级
          20190719 增加酬金部分
          20190723 修改变量名
************************************************************************************************************************/

/* 备注：本类中article_id不能改变，只能为未设置状态或已设置状态 */

#ifndef MYARTICLEOBJ_H
#define MYARTICLEOBJ_H

#include <QObject>
#include <QSqlQuery>
#include "../CPP_Storage/storageunit.h"

class MyArticleObj : public StorageUnit
{
public:
    MyArticleObj();
    MyArticleObj(const int &sender); //构造函数，传入必须要素

    //未知文章ID，设置新的文章内容
    int setArticleInfo(const int &newId, const QString &title, const QString &content);
    //以下为读取和设置文章内容各函数
    //发布者id
    int senderIdOfArticle() const;
    void setSenderIdOfArticle(int code);
    //标题
    QString titleOfArticle() const;
    void setTitleOfArticle(const QString &name);
    //文章id
    int articleIdOfArticle() const;
    void setArticleIdOfArticle(const int &id);
    //文章内容
    QString contentOfArticle() const;
    void setContentOfArticle(const QString &color);
    //状态号
    int statusCodeOfArticle() const;
    //此处传入GlobalStorage的目的是发送状态更改的消息
    void setStatusCodeOfArticle(int code);
    //负责人id
    int regulatorIdOfArticle() const;
    void setRegulatorIdOfArticle(const int &regulatorId);
    //对于子文章，标记拆分前的原文章id
    int originArticleIdOfArticle() const;
    void setOriginArticleIdOfArticle(const int &articleId);
    //翻译者用户id
    int translatorIdOfArticle() const;
    void setTranslatorIdOfArticle(int id);
    //翻译后标题
    QString translatedTitle() const;
    void setTranslatedTitle(QString newTitle);
    //翻译后内容
    QString translatedContent() const;
    void setTranslatedContent(QString newContent);
    //酬金
    int fee() const;
    void setFee(int newFee);

private:
    //文章内容
    QString mTitleOfArticle, mTranslatedTitle;
    QString mContentOfArticle, mTranslatedContent;

    int mOriginArticleId;
    //article身份标识
    int mArticleId;
    /* 备注：本类中article_id不能改变，只能为未设置状态或已设置状态 */

    //定义并设置默认发送者和负责人
    int mSenderId;
    int mRegulatorId;
    int mStatusCode;
    int mTranslatorId;
    int mFee;
};

#endif // MYARTICLEOBJ_H
