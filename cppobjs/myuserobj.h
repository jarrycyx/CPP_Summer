#ifndef MYUSEROBJ_H
#define MYUSEROBJ_H

#include <QObject>

class MyUserObj : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
//![0]

public:
    MyUserObj(QObject *parent=0);
    MyUserObj(const int &article_id ,const QString &name, const QString &color, QObject *parent=0);

    QString username() const;
    void setUsername(const QString &name);
    int userId() const;
    QString password() const;
    void setPassword(const QString &color);

signals:
    void usernameChanged();
    void passwordChanged();

private:
    QString m_username;
    QString m_password;
    int m_user_id;
};
#endif // MYUSEROBJ_H
