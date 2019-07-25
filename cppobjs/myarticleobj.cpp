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
************************************************************************************************************************/

//TO-DO: 将拆分文章方法封装在此类中

#include "myarticleobj.h"
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QDebug>

/* 备注：本类中article_id不能改变，只能为未设置状态或已设置状态 */

MyArticleObj::MyArticleObj() : StorageUnit(StorageUnit::Unchanged)
  //初始更改状态为0：未更改
{
}

/*************************************************************************
名称：     MyArticleObj
功能：     构造函数，传入必需的sender_id
参数：     parent可为空
日期：     20190715 实现构造函数
          20190717 删除在构造函数中传入文章数据
*************************************************************************/
MyArticleObj::MyArticleObj(const int &sender) : StorageUnit(StorageUnit::Unchanged), sender_id(sender)
  //初始更改状态为0：未更改
{
}

/*************************************************************************
名称：     setNewArticleInfo
功能：     新增文章，直接导入文章部分信息
参数：     文章内容
日期：     20190716
*************************************************************************/
int MyArticleObj::setArticleInfo(const int &newId, const QString &title, const QString &content)
{
    m_article_id = newId;
    m_titleOfArticle = title;
    m_contentOfArticle = content;
    return m_article_id;
}

/*************************************************************************
名称：     titleOfArticle等
功能：     设置和获取各项参数
日期：     20190714
*************************************************************************/
QString MyArticleObj::titleOfArticle() const
{
    return m_titleOfArticle;
}

void MyArticleObj::setTitleOfArticle(const QString &titleOfArticle)
{
    if (titleOfArticle != m_titleOfArticle)
    {
        m_titleOfArticle = titleOfArticle;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}
QString MyArticleObj::contentOfArticle() const
{
    return m_contentOfArticle;
}

void MyArticleObj::setContentOfArticle(const QString &contentOfArticle)
{
    if (contentOfArticle != m_contentOfArticle)
    {
        m_contentOfArticle = contentOfArticle;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}

int MyArticleObj::articleIdOfArticle() const
{
    return m_article_id;
}

void MyArticleObj::setArticleIdOfArticle(const int &id)
{
    m_article_id = id;
}

int MyArticleObj::statusCodeOfArticle() const
{
    return status_code;
}

void MyArticleObj::setStatusCodeOfArticle(int code)
{
    status_code = code;
    if (getModifyStatus() == StorageUnit::Unchanged)
        setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
}

int MyArticleObj::regulatorIdOfArticle() const
{
    return regulator_id;
}
void MyArticleObj::setRegulatorIdOfArticle(const int &regulatorId)
{
    if (regulator_id != regulatorId)
    {
        regulator_id = regulatorId;
        if (status_code < 110)
            status_code = 110; //如果还没有设置过负责人，则标记为110状态
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}

int MyArticleObj::senderIdOfArticle() const
{
    return sender_id;
}
void MyArticleObj::setSenderIdOfArticle(int id)
{
    sender_id = id;
    if (getModifyStatus() == StorageUnit::Unchanged)
        setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
}

int MyArticleObj::originArticleIdOfArticle() const
{
    return origin_article_id;
}
void MyArticleObj::setOriginArticleIdOfArticle(const int &articleId)
{
    origin_article_id = articleId;
    if (getModifyStatus() == StorageUnit::Unchanged)
        setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
}



int MyArticleObj::translatorIdOfArticle() const
{
    return translator_id;
}
void MyArticleObj::setTranslatorIdOfArticle(int id)
{
    translator_id = id;
    if (getModifyStatus() == StorageUnit::Unchanged)
        setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
}



QString MyArticleObj::translatedTitle() const
{
    return m_translatedTitle;
}
void MyArticleObj::setTranslatedTitle(QString newTitle)
{
    m_translatedTitle = newTitle;
    if (getModifyStatus() == StorageUnit::Unchanged)
        setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
}

QString MyArticleObj::translatedContent() const
{
    return m_translatedContent;
}
void MyArticleObj::setTranslatedContent(QString newContent)
{
    m_translatedContent = newContent;
    if (getModifyStatus() == StorageUnit::Unchanged)
        setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
}
