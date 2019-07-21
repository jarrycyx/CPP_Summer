#include "myuserobj.h"

MyUserObj::MyUserObj(): StorageUnit (0)
{
}

MyUserObj::MyUserObj(const int &user_id ,const QString &name, const QString &password, int role)
    : StorageUnit (0), m_username(name), m_password(password), m_user_id(user_id), m_role(role)
{
}

QString MyUserObj::username() const
{
    return m_username;
}

void MyUserObj::setUsername(const QString &name)
{
    if (name != m_username) {
        m_username = name;
        if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
    }
}

QString MyUserObj::password() const
{
    return m_password;
}

void MyUserObj::setPassword(const QString &pswd)
{
    if (pswd != m_password) {
        m_password = pswd;
        if (getModifyStatus()==0) setModifyStatus(2);//标记为已修改
    }
}

int MyUserObj::userId() const
{
    return m_user_id;
}


int MyUserObj::role() const{
    return m_role;
}
