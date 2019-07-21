#ifndef MYUSEROBJ_H
#define MYUSEROBJ_H

#include "storageunit.h"
#include <QObject>

class MyUserObj: public StorageUnit
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

private:
    QString m_username;
    QString m_password;
    int m_user_id;
    int m_role;
};
#endif // MYUSEROBJ_H
