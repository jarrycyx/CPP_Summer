//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data-Model
/************************************************************************************************************************
类名：     RequestUserList
功能：     用于报名的用户（负责人和翻译者）显示，继承QAbstractListModel，作为界面显示的Model
                 本类是沟通Model与Interaction的桥梁
备注：     该类用于多个界面，在外部调用构造函数时应指定Type，即是否允许进行更改操作
                 实际因尽量减少在此类中直接进行修改操作，应该通过返回的指针在MyArticleObj中操作
                 该类的主要目的是通过Qt各对象和QML视图直接交互
日期：     20190720 初步实现该类，与ListView联调
                 20190721 进一步完善，增加全部删除等方法
************************************************************************************************************************/

//TO-DO: 使用类模板进行优化
//TO-DO: 重载[]运算符

#ifndef REQUESTUSERLIST_H
#define REQUESTUSERLIST_H

#include <QObject>
#include <QAbstractListModel>
#include "../cppobjs/myuserobj.h"

class RequestUserList : public QAbstractListModel
{
public:
    explicit RequestUserList();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addARequestUser(MyUserObj *newRequestUser);

    inline MyUserObj *getRequestUser(int idx) { return requestUsers[idx]; }

    inline void removeAllRequestUsers() { requestUsers.clear(); }

    Q_INVOKABLE void removeOne();

protected:
    QVector<MyUserObj *> requestUsers;
    QHash<int, QByteArray> roleNames() const override;
};
#endif // REQUESTUSERLIST_H
