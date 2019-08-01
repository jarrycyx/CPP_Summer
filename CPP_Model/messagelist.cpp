//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data-Model
/************************************************************************************************************************
类名：     MessageList
功能：     用于文章列表显示，继承QAbstractListModel，作为界面显示的Model
          本类是沟通Model与Interaction的桥梁
备注：     该类用于多个界面，在外部调用构造函数时应指定Type，即是否允许进行更改操作
          实际因尽量减少在此类中直接进行修改操作，应该通过返回的指针在MyArticleObj中操作
          该类的主要目的是通过Qt各对象和QML视图直接交互
日期：     20190726 初步实现该类
          20190721 进一步完善，调整界面
************************************************************************************************************************/


#include "MessageList.h"

MessageList::MessageList() : QAbstractListModel()
{
}

int MessageList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return messages.size();
}

QVariant MessageList::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    switch (role)
    {
    case 1:
        return messages.at(index.row())->getContent();
    case 2:
        return messages.at(index.row())->getTime().toString("yyyy-MM-dd hh:mm");
    }
}

QHash<int, QByteArray> MessageList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[1] = "messageContent";
    roles[2] = "messageTime";
    return roles;
}

void MessageList::addAMessage(MyRequestObj *newMessage)
{
    beginInsertRows(QModelIndex(), 0, 0);
    messages.push_front(newMessage);
    endInsertRows();
}
