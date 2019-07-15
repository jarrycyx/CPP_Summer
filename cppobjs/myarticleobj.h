#ifndef MYARTICLEOBJ_H
#define MYARTICLEOBJ_H

#include <QObject>

class MyArticleObj : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString titleOfArticle READ titleOfArticle WRITE setTitleOfArticle NOTIFY titleOfArticleChanged)
    Q_PROPERTY(QString contentOfArticle READ contentOfArticle WRITE setContentOfArticle NOTIFY contentOfArticleChanged)
//![0]

public:
    MyArticleObj(QObject *parent=0);
    MyArticleObj(const int &article_id ,const QString &name, const QString &color, QObject *parent=0);

    QString titleOfArticle() const;
    void setTitleOfArticle(const QString &name);
    int articleIdOfArticle() const;
    QString contentOfArticle() const;
    void setContentOfArticle(const QString &color);

signals:
    void titleOfArticleChanged();
    void contentOfArticleChanged();

private:
    QString m_titleOfArticle;
    QString m_contentOfArticle;
    int m_article_id;
};

#endif // MYARTICLEOBJ_H
