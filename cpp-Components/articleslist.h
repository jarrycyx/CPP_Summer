#ifndef ARTICLESLIST_H
#define ARTICLESLIST_H

#include <QObject>
#include <QAbstractListModel>
#include "../cppobjs/myarticleobj.h"

class ArticlesList: public QAbstractListModel
{
public:
    explicit ArticlesList();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addAnArticle(MyArticleObj* newArticle);
    MyArticleObj* getArticle(int idx);

    Q_INVOKABLE void removeOne();

private:
    QVector<MyArticleObj*> articles;
    QHash<int, QByteArray> roleNames() const;
};

#endif // ARTICLESLIST_H
