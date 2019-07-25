//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     RegulatorPageHandler
功能：     负责人页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190718 初步实现负责人报名和查看文章功能
                 20190720 实现修改文章状态
                 20190722 实现拆分文章功能
                 20190723 实现向翻译者分配文章功能
************************************************************************************************************************/

//TO-DO: 负责人提交前再次修改

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

#include "cpp-Components/requestuserlist.h"
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

        public : explicit RegulatorPageHandler(int regulatorId, GlobalComponents *newGlobal, QObject *parent = nullptr);
    ~RegulatorPageHandler();

    void startLoadingRegulatorArticleList(int userId);

    Q_INVOKABLE void splitRegulatorArticle(int index, QString title, QString content);
    Q_INVOKABLE void mergeRegulatorArticle(int index);
    Q_INVOKABLE void signForRegulatorArticle(int index);
    Q_INVOKABLE void startRecruitingTranslatorForArticle(int index);
    Q_INVOKABLE void editArticle(int index, QString title, QString content);
    Q_INVOKABLE void stopRecruitingTranslatorForArticle(int index);
    Q_INVOKABLE void chooseTranslator(int index);
    //从QML唤起的选择译者函数
    Q_INVOKABLE void translatorChosen(int idx);
    Q_INVOKABLE void commentToTranslator(int idx, QString comment);
    Q_INVOKABLE QString getHistoryComment(int idx);
    Q_INVOKABLE void acceptSubarticle(int idx);

    void loadArticleTranslatorData(int originArticleId);

private:
    ArticlesList regulatorSubarticleList, regulatorArticleList, allSeekingRegulatorArticle;
    RequestUserList translatorList;

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
    void regulatorListModelChanged(const QVariant &);
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
