#include "myuserobj.h"

MyUserObj::MyUserObj(QObject *parent)
    : QObject(parent)
{
}

MyUserObj::MyUserObj(const int &user_id ,const QString &title, const QString &content, QObject *parent)
    : QObject(parent), m_username(title), m_password(content), m_user_id(user_id)
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
        emit usernameChanged();
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
        emit passwordChanged();
    }
}

int MyUserObj::userId() const
{
    return m_user_id;
}
