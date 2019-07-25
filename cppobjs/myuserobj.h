//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data
/************************************************************************************************************************
类名：     MyArticleObj
功能：     本地存储用户信息，实现对用户的增添、状态更改等操作方法
备注：     role各状态意义如下
                    1	发送者
                    2	负责人
                    3  翻译者

日期：     20190709 初步实现该类的Sender，Regulator部分
                 20190717 架构大调整，移除数据库操作，调整到Storage层级
                 20190720 实现Translator部分
************************************************************************************************************************/

//TO-DO: 增添认证信息，积分，余额等信息

#ifndef MYUSEROBJ_H
#define MYUSEROBJ_H

#include "storageunit.h"
#include <QObject>

class MyUserObj : public StorageUnit
{

public:
    MyUserObj();
    MyUserObj(const int &user_id, const QString &name, const QString &password, int role);

    QString username() const;
    void setUsername(const QString &name);
    int userId() const;
    int role() const;
    QString password() const;
    void setPassword(const QString &color);

private:
    QString m_username;
    QString m_password;
    int m_user_id;
    int m_role;
};
#endif // MYUSEROBJ_H
