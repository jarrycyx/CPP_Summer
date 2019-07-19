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


MyArticleObj::MyArticleObj()
{
    modifyStatus=0;
}


/*************************************************************************
【函数名称】      MyArticleObj
【函数功能】      构造函数，传入必需的query和sender_id
【参数】         parent可为空
【开发者及日期】   jarrycyx 20190715
【更改记录】      20190717：删除在构造函数中传入文章数据
*************************************************************************/
MyArticleObj::MyArticleObj(const int &sender)
    :sender_id(sender)
{
    modifyStatus=0;
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
        if (modifyStatus==0) modifyStatus=2;//标记为已修改
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
        if (modifyStatus==0) modifyStatus=2;//标记为已修改
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
    if (modifyStatus==0) modifyStatus=2;//标记为已修改
}


int MyArticleObj::regulatorIdOfArticle() const{
    return regulator_id;
}
void MyArticleObj::setRegulatorIdOfArticle(const int &regulatorId){
    if (regulator_id!=regulatorId){
        regulator_id=regulatorId;
        status_code=110;
        if (modifyStatus==0) modifyStatus=2;//标记为已修改
    }
}


int MyArticleObj::senderIdOfArticle() const{
    return sender_id;
}
int MyArticleObj::setSenderIdOfArticle(int id){
    sender_id=id;
    if (modifyStatus==0) modifyStatus=2;//标记为已修改
}


/*************************************************************************
【函数名称】      updateArticleInfoToRemote
【函数功能】      本地进行修改后调用该函数上传到服务器
【参数】         无
【返回值】        无
【开发者及日期】   jarrycyx 20190718
*************************************************************************/
/*void MyArticleObj::updateArticleInfoToRemote(){
    qDebug() << QString("UPDATE articles SET title=\"%1\", content=\"%2\" ,sender=%3, regulator=%4, translator=%5, curr_status=%6 where article_id=%7")
               .arg(m_titleOfArticle).arg(m_contentOfArticle).arg(sender_id)
                   .arg(regulator_id).arg(-1).arg(status_code).arg(articleIdOfArticle());
    thisQuery->exec(QString("UPDATE articles SET title=\"%1\", content=\"%2\" ,sender=%3, regulator=%4, translator=%5, curr_status=%6 where article_id=%7")
                .arg(m_titleOfArticle).arg(m_contentOfArticle).arg(sender_id)
                    .arg(regulator_id).arg(-1).arg(status_code).arg(articleIdOfArticle()));
}*/

/*************************************************************************
【函数名称】      addArticleToRemoteDBReturnId
【函数功能】      未知文章ID，文章内容上传到服务器并获取ID
【参数】          无
【返回值】        无
【开发者及日期】   jarrycyx 20190717
*************************************************************************/
/*
int MyArticleObj::addArticleToRemoteDBReturnId(){
    thisQuery->exec(QString("insert into articles (title,content,create_time,sender,regulator,translator,curr_status) values "\
                            "(\"%1\", \"%2\", NOW(), %3, %4, %5, %6)")
                .arg(m_titleOfArticle).arg(m_contentOfArticle).arg(sender_id).arg(regulator_id).arg(-1).arg(status_code));
    thisQuery->exec(QString("select article_id FROM articles WHERE sender=%1 ORDER BY create_time DESC").arg(sender_id));
    qDebug() << QString("select article_id FROM articles WHERE sender=%1 ORDER BY create_time DESC").arg(sender_id);
    if (thisQuery->next()){
        qDebug() << "updateArticleToRemoteDBReturnId "<< thisQuery->value(0).toInt();
        return thisQuery->value(0).toInt();
    }
    return -1;
}
*/

int MyArticleObj::getModifyStatus(){
    return modifyStatus;
}

void MyArticleObj::setModifyStatus(int m){
    modifyStatus = m;
}

