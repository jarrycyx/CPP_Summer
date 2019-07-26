//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     TranslatorPageHandler
功能：     翻译者页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190722 实现收取文章和报名功能
          20190723 实现保存翻译功能
************************************************************************************************************************/


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

#include "../CPP_Storage/globalcomponents.h"
#include "abstractpage.h"

class TranslatorPageHandler : public AbstractPage
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志

public :
    explicit TranslatorPageHandler(int translatorId, GlobalComponents *newGlobal, QObject *parent = nullptr);
    ~TranslatorPageHandler();

    void startLoadingTranslatorArticleList(int userId);

    Q_INVOKABLE void signForTranslatorArticle(int index);
    Q_INVOKABLE void editTranslatedArticle(int index, QString title, QString content);
    Q_INVOKABLE QString getRegulatorComment(int index);

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

    //向QML发送刷新ListView界面信号
    void startRefreshQml();
    void refreshQmlComplete();

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif
