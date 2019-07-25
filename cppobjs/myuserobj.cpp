#include "myuserobj.h"

MyUserObj::MyUserObj() : StorageUnit(StorageUnit::Unchanged)
{
}

MyUserObj::MyUserObj(const int &user_id, const QString &name, const QString &password, int role)
    : StorageUnit(StorageUnit::Unchanged), m_username(name), m_password(password), m_user_id(user_id), m_role(role)
    //初始更改状态为0：未更改
{
}


/*************************************************************************
名称：     username等
功能：     设置和获取各项参数
日期：     20190712
*************************************************************************/
QString MyUserObj::username() const
{
    return m_username;
}

void MyUserObj::setUsername(const QString &name)
{
    if (name != m_username)
    {
        m_username = name;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}

QString MyUserObj::password() const
{
    return m_password;
}

void MyUserObj::setPassword(const QString &pswd)
{
    if (pswd != m_password)
    {
        m_password = pswd;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}

int MyUserObj::userId() const
{
    return m_user_id;
}

int MyUserObj::role() const
{
    return m_role;
}
