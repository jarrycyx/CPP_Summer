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


void ArticlesList::editAnArticle(int index, QString title, QString content){
    articles[index]->setTitleOfArticle(title);
    articles[index]->setContentOfArticle(content);
    QModelIndex idx = createIndex(index,index);
    emit dataChanged(idx, idx);
}

void ArticlesList::addAnArticle(MyArticleObj* newArticle){
    articles.push_front(newArticle);
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottom = createIndex(articles.length(),0);
    emit dataChanged(topLeft, bottom);
}

void ArticlesList::deleteAnArticle(int index){
    articles[index]->setModifyStatus(3);
    articles.remove(index);
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottom = createIndex(articles.length(),0);
    emit dataChanged(topLeft, bottom);
}

