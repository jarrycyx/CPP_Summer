//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data
/************************************************************************************************************************
类名：     MyArticleObj
功能：     本地存储请求内容，实现对请求的增添、状态更改等操作方法
备注：     mType各状态意义如下
            1	报名为负责人
            2	报名为译者
            3   负责人对译者的反馈
            4   提醒信息

日期：     20190720 初步实现该类的Regulator部分
          20190722 实现Translator部分
          20190727 增加时间属性
          20190728 增加消息盒子功能
************************************************************************************************************************/

#ifndef MYREQUESTOBJ_H
#define MYREQUESTOBJ_H

#include "../CPP_Storage/storageunit.h"
#include <QString>
#include <QDateTime>

class MyRequestObj : public StorageUnit
{
public:
    //构造函数传入请求信息
    MyRequestObj(const int &newRequestId, const int &newUserId,
                 const int &newArticleId, const int &newType);
    //请求id
    inline int getRequestId() const { return mRequestId; }
    //发出请求的用户id
    inline int getUserId() const { return mUserId; }
    //对应文章id
    inline int getArticleId() const { return mArticleId; }
    //请求类型
    inline int getType() const { return mType; }
    //请求内容，尽在mType为3时有效
    inline QString getContent() const { return mContent; }
    void setContent(QString str);
    //请求上传时间
    inline QDateTime getTime() const { return mTime; }
    void setTime(QDateTime newTime);

    MyRequestObj &operator=(const MyRequestObj &newObj)
    {
        mRequestId = newObj.getRequestId();
        mArticleId = newObj.getArticleId();
        mUserId = newObj.getUserId();
        mType = newObj.getType();
        mContent = newObj.getContent();
        mTime = newObj.getTime();
        return *this;
    }

private:
    int mRequestId;
    int mArticleId;
    int mUserId;
    int mType;
    QString mContent;
    QDateTime mTime;
};

#endif // MYREQUESTOBJ_H
