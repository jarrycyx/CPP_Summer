#ifndef USERINFOPAGEHANDLER_H
#define USERINFOPAGEHANDLER_H

#include <QObject>
#include "./cppobjs/myuserobj.h"
#include "./cpp-Components/requestuserlist.h"
#include "./cpp-Components/globalcomponents.h"
#include "./cpp-Components/messagelist.h"

class UserInfoPageHandler : public QObject
{
    Q_OBJECT
public:
    explicit UserInfoPageHandler(MyUserObj* thisUser, GlobalComponents *newGlobal, QObject *parent = nullptr);
    void startPage(QQmlApplicationEngine *engine);
    Q_INVOKABLE QString getUsername();

signals:

public slots:

private:
    MyUserObj* user;
    MessageList messageList;
    GlobalComponents *globalStorageComponent;
};

#endif // USERINFOPAGEHANDLER_H
