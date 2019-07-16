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
    Q_PROPERTY(QVariant otherModel READ otherModel NOTIFY otherModelChanged)
    Q_PROPERTY(QVariant regulatorListModel READ regulatorListModel NOTIFY regulatorListModelChanged)

public:
    explicit SenderPageHandler(QObject *parent = nullptr, int user_id=3);
    QVariant thisModel() const;// { return m_value1; }
    Q_INVOKABLE void itemMove(int idx);
    Q_INVOKABLE void regulatorChosen(int idx);
    Q_INVOKABLE void addAnArticle(QString title, QString content);
    QVariant otherModel() const;// { return m_value1; }
    QVariant regulatorListModel() const;// { return m_value1; }

private:
    QQmlApplicationEngine *thisEngine;
    QSqlQuery *query;
    QList<QObject*> myThisModel;
    QList<QObject*> myOtherModel;
    QList<QObject*> myRegulatorListModel;
    void requireArticleDataInNewThread();
    int thisUserId=-1;
    int articleSendingId=-1;

signals:
    void thisModelChanged(const QVariant&);
    void otherModelChanged(const QVariant&);
    void regulatorListModelChanged(const QVariant&);
    void loadArticlesComplete(int article_id, QString title, QString content);
    void refreshQml();

public slots:
    void startPage(QQmlApplicationEngine *engine);
    void onLoadArticlesComplete(int article_id, QString title, QString content);
    void startLoadingData(int flag, int user_id);
};


#endif
