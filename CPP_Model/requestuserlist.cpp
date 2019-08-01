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


#include "RequestUserList.h"

RequestUserList::RequestUserList() : QAbstractListModel()
{
}

int RequestUserList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return requestUsers.size();
}

QVariant RequestUserList::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    switch (role)
    {
    case 1:
        return requestUsers.at(index.row())->username();
    case 2:
        return requestUsers.at(index.row())->qualification();
    case 3:
        return requestUsers.at(index.row())->credit();
    }
}

QHash<int, QByteArray> RequestUserList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[1] = "nameOfUser";
    roles[2] = "qualificationOfUser";
    roles[3] = "credits";
    return roles;
}

void RequestUserList::addARequestUser(MyUserObj *newRequestArticle)
{
    beginInsertRows(QModelIndex(), 0, 0);
    requestUsers.push_front(newRequestArticle);
    endInsertRows();
}
