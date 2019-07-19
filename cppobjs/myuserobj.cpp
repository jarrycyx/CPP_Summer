#include "myuserobj.h"

MyUserObj::MyUserObj()
{
}

MyUserObj::MyUserObj(const int &user_id ,const QString &name, const QString &password, int role)
    : m_username(name), m_password(password), m_user_id(user_id), m_role(role)
{
    modifyStatus=0;
}

QString MyUserObj::username() const
{
    return m_username;
}

void MyUserObj::setUsername(const QString &name)
{
    if (name != m_username) {
        m_username = name;
        if (modifyStatus==0) modifyStatus=2;//标记为已修改
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
        if (modifyStatus==0) modifyStatus=2;//标记为已修改
    }
}

int MyUserObj::userId() const
{
    return m_user_id;
}


int MyUserObj::role() const{
    return m_role;
}


int MyUserObj::getModifyStatus(){
    return modifyStatus;
}

void MyUserObj::setModifyStatus(int m){
    modifyStatus = m;
}
