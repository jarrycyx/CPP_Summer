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
