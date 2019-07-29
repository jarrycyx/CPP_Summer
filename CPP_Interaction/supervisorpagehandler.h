//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Model-Interaction
/************************************************************************************************************************
类名：     SupervisorPageHandler
功能：     审核人页面的后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190727 初步实现
************************************************************************************************************************/



#ifndef SUPERVISORPAGEHANDLER_H
#define SUPERVISORPAGEHANDLER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQmlContext>
#include <QObject>
#include <QQuickView>
#include <QSqlQuery>
#include <QAbstractListModel>

#include "../CPP_Model/requestuserlist.h"
#include "../CPP_Storage/globalstoragecomponents.h"
#include "abstractpage.h"

/* 发送者页面后台处理程序
 * 也用于启动一些附属页面
 * 其功能与MyArticleObject紧密相关
 */
class SupervisorPageHandler : public AbstractPage
{
Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
    /* 以上三个model分别存储
     * 该用户的文章列表
     * 所有用户的文章列表
     * 负责人选取列表的数据 */

public :
    explicit SupervisorPageHandler(int supervisorId, QObject *parent = nullptr);
    ~SupervisorPageHandler();

    void startLoadingSupervisorArticleList(int userId);

    //标记子文章审核通过
    Q_INVOKABLE void acceptSubarticle(int idx);
    //向翻译者进行反馈
    Q_INVOKABLE void commentToTranslator(int idx, QString comment);

private:
    //分别为子任务列表，任务列表，其他正在招募负责人的任务列表
    ArticlesList supervisorSubarticleList;

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
};


#endif // SUPERVISORPAGEHANDLER_H
