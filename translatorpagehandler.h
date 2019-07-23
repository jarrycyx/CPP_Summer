#ifndef TranslatorPageHandler_H
#define TranslatorPageHandler_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQmlContext>
#include <QObject>
#include <QQuickView>
#include <QSqlQuery>
#include <QAbstractListModel>

#include "cpp-Components/globalcomponents.h"

/* 发送者页面后台处理程序
 * 也用于启动一些附属页面
 * 其功能与MyArticleObject紧密相关
 */

class TranslatorPageHandler : public QObject
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志

public :
    explicit TranslatorPageHandler(int translatorId, GlobalComponents *newGlobal, QObject *parent = nullptr);
    ~TranslatorPageHandler();

    void startLoadingTranslatorArticleList(int userId);

    Q_INVOKABLE void signForTranslatorArticle(int index);
    Q_INVOKABLE void editTranslatedArticle(int index, QString title, QString content);

    void loadArticleTranslatorData(int articleId);

private:
    ArticlesList translatorSubarticleList, allSeekingTranslatorArticle;

    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *thisEngine;
    GlobalComponents *globalStorageComponent;

    //当前负责人身份标识，默认为-1（空）
    int thisUserId = -1;
    //当前正在浏览的文章身份标识，没有文章则为-1
    int articleSendingId = -1;
    //当前正在编辑/查看的文章
    int currentInViewIndex = 0;

signals:
    //以下四项为各model向QML暴露的信号接口
    void thisModelChanged(const QVariant &);
    void otherModelChanged(const QVariant &);
    void translatorListModelChanged(const QVariant &);
    void loadArticlesComplete(int article_id, QString title, QString content);
    //错误信息信号，向QML发送，使其在界面上显示
    void sendErrorMessage(QString errStr);
    //成功信息信号，向QML发送，使其在界面上显示
    void sendSuccessMessage(QString successStr);

    //向QML发送刷新ListView界面信号
    void startRefreshQml();
    void refreshQmlComplete();

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif
