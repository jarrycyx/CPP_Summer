#include "articleslist.h"

ArticlesList::ArticlesList(int type) : QAbstractListModel(), typeOfThisList(type)
{
}

int ArticlesList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return articles.size();
}

QVariant ArticlesList::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    switch (role)
    {
    case 1:
        return articles.at(index.row())->titleOfArticle();
    case 2:
        return articles.at(index.row())->contentOfArticle();
    case 3:
        return articles.at(index.row())->statusCodeOfArticle();
    case 4:
        return typeOfThisList;
    }
}

QHash<int, QByteArray> ArticlesList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[1] = "titleOfArticle";
    roles[2] = "contentOfArticle";
    roles[3] = "statusCodeOfArticle";
    roles[4] = "typeOfList";
    return roles;
}

MyArticleObj *ArticlesList::getArticle(int idx)
{
    return articles[idx];
}

void ArticlesList::editAnArticle(int index, QString title, QString content)
{
    articles[index]->setTitleOfArticle(title);
    articles[index]->setContentOfArticle(content);
    QModelIndex idx = createIndex(index, index);
    emit dataChanged(idx, idx);
}

void ArticlesList::editAnArticle(int index)
{
    QModelIndex idx = createIndex(index, index);
    emit dataChanged(idx, idx);
}

void ArticlesList::addAnArticle(MyArticleObj *newArticle)
{
    beginInsertRows(QModelIndex(), 0, 0);
    articles.push_front(newArticle);
    endInsertRows();
}

void ArticlesList::deleteAnArticle(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    articles[index]->setModifyStatus(3);
    articles.remove(index);
    endRemoveRows();
}
