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

class SenderPageHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant thisModel READ thisModel NOTIFY thisModelChanged)

public:
    explicit SenderPageHandler(QObject *parent = nullptr);
    QVariant thisModel() const;// { return m_value1; }
    void setThisModel(const QVariant& v);
    Q_INVOKABLE void itemMove(int idx);

private:
    //QQmlApplicationEngine engine;
    QSqlQuery *query;
    QList<QObject*> myThisModel;
    void requireArticleDataInNewThread();

signals:
    void thisModelChanged(const QVariant&);
    void loadArticlesComplete(QString title, QString content);
    void refreshQml();

public slots:
    void startPage(QQmlApplicationEngine *engine);
    void onLoadArticlesComplete(QString title, QString content);
};

#endif // SENDERPAGEHANDLER_H
