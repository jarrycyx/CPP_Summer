//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     SenderPageHandler
功能：     发送者页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190710 初步实现发送文章功能
          20190712 实现修改文章状态
          20190718 实现选取负责人
          20190720 主要功能完成
          20190727 增加拒绝接受功能
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

#include "../CPP_Storage/globalstoragecomponents.h"
#include "../CPP_Model/requestuserlist.h"
#include "abstractpage.h"

/* 发送者页面后台处理程序
 * 也用于启动一些附属页面
 * 其功能与MyArticleObject紧密相关
 */

class SenderPageHandler : public AbstractPage
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
        /* 以上三个model分别存储
     * 该用户的文章列表
     * 所有用户的文章列表
     * 负责人选取列表的数据 */

public : 
    explicit SenderPageHandler(int senderId);
    ~SenderPageHandler();

    //从QML唤起的选择负责人函数
    Q_INVOKABLE void regulatorChosen(int idx);
    //从QML唤起的选择负责人函数
    Q_INVOKABLE void chooseRegulator(int index);

    //从Storage层级查询发布者任务列表
    void startLoadingSenderArticleList(int userId);
    //新发布文章
    Q_INVOKABLE void addSenderArticle(QString title, QString content, int money);
    //编辑并提交更新文章
    Q_INVOKABLE void editSenderArticle(int index, QString title, QString content);
    //拖动删除文章
    Q_INVOKABLE void deleteSenderArticle(int index);
    //确定任务完成并分配酬金
    Q_INVOKABLE void confirmAcceptArticle(int index);
    //拒绝接收，交给负责人重新翻译
    Q_INVOKABLE void reTranslate(int index);
    //根据当前正在查看的文章，加载报名的负责人列表
    void loadArticleRegulatorData(int articleId);

private:
    //分别为我发布的文章列表，其他用户的文章列表
    ArticlesList mSenderArticleList, allUserArticleList;
    RequestUserList requestUserList;

    //当前需要发送的文章身份标识，没有要发送的文章则为-1
    int articleSendingId = -1;
    //当前正在编辑/查看的文章
    int currentInViewIndex = 0;

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif
