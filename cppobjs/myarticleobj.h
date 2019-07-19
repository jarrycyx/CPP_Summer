#ifndef MYARTICLEOBJ_H
#define MYARTICLEOBJ_H

#include <QObject>
#include <QSqlQuery>



/**********************************************
 * 文章状态代号含义
100	已上传，招募开始，生命周期开始
110	已标记负责人
115	招募结束
120	负责人已进行拆分（若忽略则直接修改为200）
---	200	子文章已创建（或已忽略拆分的文章）
---	210	已标记翻译者
---	215	译者正在翻译
---	220	译者翻译完成，提交给负责人
---	225	负责人进行评审，译者修改
---	230	子文章译文评审通过
---	400	子文章生命周期完成
300	子文章合并完成
310	负责人提交
320	发送者已收取
330	发送者已付款，款项成功分配
400	生命周期完成
************************************************/

class MyArticleObj
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
    int addArticleToRemoteDBReturnId();
    void updateArticleInfoToRemote();

    int getModifyStatus();
    void setModifyStatus(int m);

signals:
    //文章内容改变后发送的信号
    void titleOfArticleChanged();
    void contentOfArticleChanged();
    void statusCodeOfArticleChanged();


private:
    int modifyStatus=0; //0:未更改，1:新建，2:修改，3:删除
    //文章内容
    QString m_titleOfArticle;
    QString m_contentOfArticle;

    //article身份标识
    int m_article_id;
    /* 备注：本类中article_id不能改变，只能为未设置状态或已设置状态 */

    //定义并设置默认发送者和负责人
    int sender_id=-1;
    int regulator_id=-1;
    int status_code=100;
};

#endif // MYARTICLEOBJ_H
