#include "articleslist.h"

ArticlesList::ArticlesList() :QAbstractListModel()
{

}

int ArticlesList::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return articles.size();
}

QVariant ArticlesList::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role);
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount() )
        return QVariant();

    switch (role){
    case 1:
        return articles.at(index.row())->titleOfArticle();
    case 2:
        return articles.at(index.row())->contentOfArticle();
    case 3:
        return articles.at(index.row())->statusCodeOfArticle();
    }
}

QHash<int, QByteArray> ArticlesList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[1] = "titleOfArticle";
    roles[2] = "contentOfArticle";
    roles[3] = "statusCodeOfArticle";
    return roles;
}



MyArticleObj* ArticlesList::getArticle(int idx){
    return articles[idx];
}

void ArticlesList::addAnArticle(MyArticleObj* newArticle){
    articles.push_front(newArticle);
}


