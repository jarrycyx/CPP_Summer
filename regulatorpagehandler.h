#ifndef RegulatorPageHandler_H
#define RegulatorPageHandler_H

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

class RegulatorPageHandler : public QObject
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
    /* 以上三个model分别存储
     * 该用户的文章列表
     * 所有用户的文章列表
     * 负责人选取列表的数据 */

public:
    explicit RegulatorPageHandler(int regulatorId, GlobalComponents* newGlobal ,QObject *parent = nullptr);
    ~RegulatorPageHandler();

    void startLoadingRegulatorArticleList(int userId);

    Q_INVOKABLE void splitRegulatorArticle(int index, QString title, QString content);
    Q_INVOKABLE void signForRegulatorArticle(int index);
    Q_INVOKABLE void startRecruitingTranslatorForArticle(int index);
    Q_INVOKABLE void editArticle(int index, QString title, QString content);
    Q_INVOKABLE void stopRecruitingTranslatorForArticle(int index);

    void loadArticleRegulatorData(int articleId);

private:

    ArticlesList regulatorSubarticleList, regulatorArticleList, allSeekingRegulatorArticle;

    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *thisEngine;
    GlobalComponents* globalStorageComponent;

    //当前负责人身份标识，默认为-1（空）
    int thisUserId=-1;
    //当前正在浏览的文章身份标识，没有文章则为-1
    int articleSendingId=-1;
    //当前正在编辑/查看的文章
    int currentInViewIndex=0;

signals:
    //以下四项为各model向QML暴露的信号接口
    void thisModelChanged(const QVariant&);
    void otherModelChanged(const QVariant&);
    void regulatorListModelChanged(const QVariant&);
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
