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

#include "../CPP_Model/requestuserlist.h"
#include "../CPP_Storage/globalstoragecomponents.h"
#include "abstractpage.h"

/* 发送者页面后台处理程序
 * 也用于启动一些附属页面
 * 其功能与MyArticleObject紧密相关
 */

class RegulatorPageHandler : public AbstractPage
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志

public : 
    explicit RegulatorPageHandler(int regulatorId);
    ~RegulatorPageHandler();

    void startLoadingRegulatorArticleList(int userId);
    //自动拆分文章，新建多个子任务
    Q_INVOKABLE void splitRegulatorArticle(int index, QString title, QString content);
    //组合翻译后的子任务，同时检查是否全部翻译完成
    Q_INVOKABLE void mergeRegulatorArticle(int index);
    //报名做负责人
    Q_INVOKABLE void signForRegulatorArticle(int index);
    //开始招募翻译者
    Q_INVOKABLE void startRecruitingTranslatorForArticle(int index);
    //允许负责人对文章进行微调
    Q_INVOKABLE void editArticle(int index, QString title, QString content);
    //停止招募翻译者
    Q_INVOKABLE void stopRecruitingTranslatorForArticle(int index);
    //为子文章选取翻译者
    Q_INVOKABLE void chooseTranslator(int index);
    //查看报名的翻译者，但不能指定，需要先拆分
    Q_INVOKABLE void viewTranslator(int index);
    //从QML唤起的选择译者函数
    Q_INVOKABLE void translatorChosen(int idx);
    //向翻译者进行反馈
    Q_INVOKABLE void commentToTranslator(int idx, QString comment);
    //查看历史反馈
    Q_INVOKABLE QString getHistoryComment(int idx);
    //标记子文章审核通过
    Q_INVOKABLE void acceptSubarticle(int idx);
    //合并后的文章提交给发布者
    Q_INVOKABLE void submitToSender(int index);
    //向Storage层级查询人物列表
    void loadArticleTranslatorData(int originArticleId);

private:
    //分别为子任务列表，任务列表，其他正在招募负责人的任务列表
    ArticlesList regulatorSubarticleList, regulatorArticleList, allSeekingRegulatorArticle;
    //报名的翻译者列表，依赖于当前正在查看的文章
    RequestUserList translatorList;

    //当前正在浏览的文章身份标识，没有文章则为-1
    int articleSendingId = -1;
    //当前正在编辑/查看的文章
    int currentInViewIndex = 0;

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif
