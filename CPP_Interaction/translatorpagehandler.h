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

#include "../CPP_Storage/globalstoragecomponents.h"
#include "abstractpage.h"

class TranslatorPageHandler : public AbstractPage
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志

        public : explicit TranslatorPageHandler(int translatorId);
    ~TranslatorPageHandler();
    //开始加载文章列表
    void startLoadingTranslatorArticleList(int userId);
    //报名做翻译者
    Q_INVOKABLE void signForTranslatorArticle(int index);
    //翻译文章，注意翻译内容最初与原文相同，需翻译者进行逐步替换
    Q_INVOKABLE void editTranslatedArticle(int index, QString title, QString content);
    //查看负责人的历史反馈信息
    Q_INVOKABLE QString getRegulatorComment(int index);

private:
    //分别为我负责的子任务和其他正在招募翻译者的任务
    ArticlesList mTranslatorSubarticleList, mAllSeekingTranslatorArticleList;

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
};

#endif
