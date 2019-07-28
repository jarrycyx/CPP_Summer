//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Storage-Data
/************************************************************************************************************************
类名：     GlobalStorageComponents，通用存储封装类
功能：     封装数据库存取操作，也是所有数据实体的存储地
备注：     根据需求对数据库进行整存整取，程序开始时和结束时分别调用构造和析构函数进行下载上传操作
日期：     20190718 初步实现Sender和Regulator
          20190721 进一步完善，增加Translator和Request
************************************************************************************************************************/

#ifndef GLOBALCOMPONENTS_H
#define GLOBALCOMPONENTS_H

#include <QObject>
#include "../CPP_Data/myarticleobj.h"
#include "../CPP_Model/articleslist.h"
#include "../CPP_Data/myuserobj.h"
#include "../CPP_Data/myrequestobj.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>

class GlobalStorageComponents : public QObject
{
    Q_OBJECT
public:
    explicit GlobalStorageComponents(QObject *parent = nullptr);
    virtual ~GlobalStorageComponents();

    //文章状态改变，通知给相关用户
    void sendMessageToRelatedUser(QString str, MyArticleObj* articleInChange);
    //返回状态值对应的含义
    QString decodeStatusCode(int code);

    //程序结束前，析构函数调用以上传所有更改数据
    void uploadAllData();
    //自动生成一个文章ID，比所有其他ID都大
    inline int getAnArticleId()
    {
        biggestArticleId++;
        return biggestArticleId;
    }
    //获取总文章数量，用于遍历
    inline int getArticlesLength() { return allArticles.length(); }
    //返回对应文章对象指针
    inline MyArticleObj* getArticleToEdit(int index) { return allArticles[index]; }
    //新增一篇文章，注意这里只是在后台存储中增加了一篇文章，不代表视图会显示
    inline void addAnArticle(MyArticleObj *newArticle) { allArticles.push_front(newArticle); }
    //自动生成一个用户ID
    inline int getAUserId()
    {
        biggestUserId++;
        return biggestUserId;
    }
    //获取用户总数，用于遍历
    inline int getUsersLength() { return allUsers.length(); }
    //返回用户对象指针
    inline MyUserObj *getUserToEdit(int index) { return allUsers[index]; }
    //增加一个用户到list中，注意这里只是在后台存储中增加，不代表视图会显示
    inline void addAUser(MyUserObj *newUser) { allUsers.push_front(newUser); }
    //通过id查找user，性能较低
    MyUserObj* searchUserById(int thisUserId);

    QList<MyUserObj*> searchSubUsers(QString name);
    //通过id查找article，性能较低
    MyArticleObj* searchArticleById(int thisUserId);

    //自动生成一个请求id
    inline int getARequestId()
    {
        biggestRequestId++;
        return biggestRequestId;
    }
    //获取请求总数
    inline int getRequestsLength() { return allRequests.length(); }
    //获取一个请求对象指针
    inline MyRequestObj *getRequest(int index) { return allRequests[index]; }
    //增加一个请求实体，注意这里只是在后台存储中增加，不代表视图会显示
    inline void addARequest(MyRequestObj *newRequest) { allRequests.push_back(newRequest); }
    void sendUserModifiedMessage(int userId ,QString content);

private:
    //实际存储所有数据
    QVector<MyArticleObj*> allArticles;
    QVector<MyUserObj*> allUsers;
    QVector<MyRequestObj*> allRequests;
    //数据库请求
    QSqlQuery *query;
    //最大的ID，用于生成新的ID
    int biggestArticleId = 0, biggestUserId = 0, biggestRequestId = 0;

signals:

public slots:
};

#endif // GLOBALCOMPONENTS_H
