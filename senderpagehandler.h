#ifndef SENDERPAGEHANDLER_H
#define SENDERPAGEHANDLER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQmlContext>
#include <QObject>
#include <QQuickView>
#include <QSqlQuery>
#include <QAbstractListModel>

class SenderPageHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant thisModel READ thisModel NOTIFY thisModelChanged)

public:
    explicit SenderPageHandler(QObject *parent = nullptr, int user_id=3);
    QVariant thisModel() const;// { return m_value1; }
    void setThisModel(const QVariant& v);
    Q_INVOKABLE void itemMove(int idx);
    Q_INVOKABLE void addAnArticle(QString title, QString content);

private:
    //QQmlApplicationEngine engine;
    QSqlQuery *query;
    QList<QObject*> myThisModel;
    void requireArticleDataInNewThread();
    int thisUserId=-1;

signals:
    void thisModelChanged(const QVariant&);
    void loadArticlesComplete(int article_id, QString title, QString content);
    void refreshQml();

public slots:
    void startPage(QQmlApplicationEngine *engine);
    void onLoadArticlesComplete(int article_id, QString title, QString content);
    void startLoadingData(int flag, int user_id);
};


#endif
