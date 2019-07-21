#include "myarticleobj.h"
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QDebug>


/*************************************************************************
【类名】        MyArticleObj
【功能】        代表文章的类，用于存储数据、修改数据、向服务器同步数据
【接口说明】     先调用构造函数传入query与发送者标识，若为新增文章则调用setNewArticleInfo获取上传后ID，
                若为从服务器获取文章则调用pullArticleInfo通过ID获取文章内容
【开发者及日期】  jarrycyx 20190715
【更改记录】     20190716 将数据库操作封装在该类，并修改数据传入方式
               20190717 设置本地修改为主动
*************************************************************************/

/* 备注：本类中article不能改变，只能为未设置状态或已设置状态 */


MyArticleObj::MyArticleObj(): StorageUnit (0)
{
}


/*************************************************************************
【函数名称】      MyArticleObj
【函数功能】      构造函数，传入必需的query和sender_id
【参数】         parent可为空
【开发者及日期】   jarrycyx 20190715
【更改记录】      20190717：删除在构造函数中传入文章数据
*************************************************************************/
MyArticleObj::MyArticleObj(const int &sender):
    StorageUnit (0), sender_id(sender)
{
}




/*************************************************************************
【函数名称】      setNewArticleInfo
【函数功能】      新增文章，需要显示并上传到服务器
【参数】         文章内容
【返回值】        上传到服务器后返回的文章ID
【开发者及日期】   jarrycyx 20190716
*************************************************************************/
int MyArticleObj::setArticleInfo(const int &newId, const QString &title, const QString &content){
    m_article_id=newId;
    m_titleOfArticle=title;
    m_contentOfArticle=content;
    //m_article_id = addArticleToRemoteDBReturnId();
    return m_article_id;
}



/*************************************************************************
【函数名称】      titleOfArticle等
【函数功能】      设置和获取各项参数
【开发者及日期】   jarrycyx 20190714
*************************************************************************/
QString MyArticleObj::titleOfArticle() const
{
    return m_titleOfArticle;
}

void MyArticleObj::setTitleOfArticle(const QString &titleOfArticle)
{
    if (titleOfArticle != m_titleOfArticle) {
        m_titleOfArticle = titleOfArticle;
        if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
    }
}
QString MyArticleObj::contentOfArticle() const
{
    return m_contentOfArticle;
}

void MyArticleObj::setContentOfArticle(const QString &contentOfArticle)
{
    if (contentOfArticle != m_contentOfArticle) {
        m_contentOfArticle = contentOfArticle;
        if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
    }
}

int MyArticleObj::articleIdOfArticle() const
{
    return m_article_id;
}


void MyArticleObj::setArticleIdOfArticle(const int &id){
    m_article_id=id;
}


int MyArticleObj::statusCodeOfArticle() const{
    return status_code;
}

int MyArticleObj::setStatusCodeOfArticle(int code){
    status_code=code;
    if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
}


int MyArticleObj::regulatorIdOfArticle() const{
    return regulator_id;
}
void MyArticleObj::setRegulatorIdOfArticle(const int &regulatorId){
    if (regulator_id!=regulatorId){
        regulator_id=regulatorId;
        status_code=110;
        if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
    }
}


int MyArticleObj::senderIdOfArticle() const{
    return sender_id;
}
int MyArticleObj::setSenderIdOfArticle(int id){
    sender_id=id;
    if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
}
