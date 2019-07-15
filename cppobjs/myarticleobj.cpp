#include "myarticleobj.h"

MyArticleObj::MyArticleObj(QObject *parent)
    : QObject(parent)
{
}

MyArticleObj::MyArticleObj(const QString &title, const QString &content, QObject *parent)
    : QObject(parent), m_titleOfArticle(title), m_contentOfArticle(content)
{

}

QString MyArticleObj::titleOfArticle() const
{
    return m_titleOfArticle;
}

void MyArticleObj::setTitleOfArticle(const QString &titleOfArticle)
{
    if (titleOfArticle != m_titleOfArticle) {
        m_titleOfArticle = titleOfArticle;
        emit titleOfArticleChanged();
    }
}

QString MyArticleObj::contentOfArticle() const
{
    return m_contentOfArticle;
}

void MyArticleObj::setContentOfArticle(const QString &contentOfArticle)
{
    if (contentOfArticle != m_contentOfArticle) {
        m_contentOfArticle = contentOfArticle;
        emit contentOfArticleChanged();
    }
}
