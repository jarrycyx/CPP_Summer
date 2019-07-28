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
