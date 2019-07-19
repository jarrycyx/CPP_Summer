#ifndef MYUSEROBJ_H
#define MYUSEROBJ_H

#include <QObject>

class MyUserObj
{

public:
    MyUserObj();
    MyUserObj(const int &user_id ,const QString &name, const QString &password, int role);

    QString username() const;
    void setUsername(const QString &name);
    int userId() const;
    int role() const;
    QString password() const;
    void setPassword(const QString &color);

    int getModifyStatus();
    void setModifyStatus(int m);

private:
    int modifyStatus=0; //0:未更改，1:新建，2:修改
    QString m_username;
    QString m_password;
    int m_user_id;
    int m_role;
};
#endif // MYUSEROBJ_H
