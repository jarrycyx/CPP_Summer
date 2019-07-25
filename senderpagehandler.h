//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     SenderPageHandler
功能：     发送者页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190710 初步实现发送文章功能
                 20190712 实现修改文章状态
                 20190718 实现选取负责人
                 20190720 主要功能完成
************************************************************************************************************************/


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

#include "cpp-Components/globalcomponents.h"
#include "cpp-Components/requestuserlist.h"

/* 发送者页面后台处理程序
 * 也用于启动一些附属页面
 * 其功能与MyArticleObject紧密相关
 */

class SenderPageHandler : public QObject
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
        /* 以上三个model分别存储
     * 该用户的文章列表
     * 所有用户的文章列表
     * 负责人选取列表的数据 */

public :
    explicit SenderPageHandler(int senderId, GlobalComponents *newGlobal, QObject *parent = nullptr);
    ~SenderPageHandler();

    //从QML唤起的删除文章函数
    //Q_INVOKABLE void itemMove(int idx);
    //从QML唤起的选择负责人函数
    Q_INVOKABLE void regulatorChosen(int idx);
    //从QML唤起的增加文章函数
    //Q_INVOKABLE void addAnArticle(QString title, QString content);
    //从QML唤起的选择负责人函数
    Q_INVOKABLE void chooseRegulator(int index);
    //从QML唤起的增加文章函数
    //Q_INVOKABLE void editAnArticle(int index, QString title, QString content);

    void startLoadingSenderArticleList(int userId);
    Q_INVOKABLE void addSenderArticle(QString title, QString content);
    Q_INVOKABLE void editSenderArticle(int index, QString title, QString content);
    Q_INVOKABLE void deleteSenderArticle(int index);
    void loadArticleRegulatorData(int articleId);

private:
    ArticlesList senderArticleList, allUserArticleList;
    RequestUserList requestUserList;

    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *thisEngine;
    GlobalComponents *globalStorageComponent;

    //当前发送者身份标识，默认为-1（空）
    int thisUserId = -1;
    //当前需要发送的文章身份标识，没有要发送的文章则为-1
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
