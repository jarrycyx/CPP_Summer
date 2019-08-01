//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Interaction
/************************************************************************************************************************
类名：     ArticleInfoPageHandler
功能：     文章信息弹出框后台处理类，通过信号槽和直接唤起函数与QML紧密联动
日期：     20190726 实现查看文章信息
************************************************************************************************************************/

#ifndef ARTICLEINFOPAGEHANDLER_H
#define ARTICLEINFOPAGEHANDLER_H

#include <QObject>
#include "abstractpage.h"

class ArticleInfoPageHandler : public AbstractPage
{
    Q_OBJECT
public:
    explicit ArticleInfoPageHandler(MyArticleObj *thisArticle);
    //QML界面获取信息
    Q_INVOKABLE QString getSenderName();
    Q_INVOKABLE QString getRegulatorName();
    Q_INVOKABLE QString getTranslatorName();
    Q_INVOKABLE int getMoney();

signals:

public slots:
    void startPage(QQmlApplicationEngine *engine);

private:
    MyArticleObj *mArticle;
    MyUserObj *mSender;
    MyUserObj *mRegulator;
    MyUserObj *mTranslator;
};

#endif // ARTICLEINFOPAGEHANDLER_H
