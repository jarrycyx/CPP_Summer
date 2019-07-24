//总体框架：             Storage - Data - Model - Interaction - View
//该类所处层级：     Data
/************************************************************************************************************************
类名：     MyArticleObj
功能：     本地存储请求内容，实现对请求的增添、状态更改等操作方法
备注：     type各状态意义如下
                    1	报名为负责人
                    2	报名为译者

日期：     20190720 初步实现该类的Regulator部分
                 20190722 实现Translator部分
************************************************************************************************************************/

//TO-DO: 增添Comment部分


#ifndef MYREQUESTOBJ_H
#define MYREQUESTOBJ_H

#include "storageunit.h"
#include <QString>

class MyRequestObj : public StorageUnit
{
public:
    MyRequestObj(const int &newRequestId, const int &newUserId,
                 const int &newArticleId, const int &newType);
    inline int getRequestId() { return requestId; }
    inline int getUserId() { return userId; }
    inline int getArticleId() { return articleId; }
    inline int getType() { return type; }

private:
    int requestId;
    int articleId;
    int userId;
    int type;
    QString content;
};

#endif // MYREQUESTOBJ_H
