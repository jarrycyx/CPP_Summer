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

/* 发送者页面后台处理程序
 * 也用于启动一些附属页面
 * 其功能与MyArticleObject紧密相关
 */

class SenderPageHandler : public QObject
{
    Q_OBJECT //需要注册到QML，添加Q_OBJECT标志
    Q_PROPERTY(QVariant thisModel READ thisModel NOTIFY thisModelChanged)
    Q_PROPERTY(QVariant otherModel READ otherModel NOTIFY otherModelChanged)
    Q_PROPERTY(QVariant regulatorListModel READ regulatorListModel NOTIFY regulatorListModelChanged)
    /* 以上三个model分别存储
     * 该用户的文章列表
     * 所有用户的文章列表
     * 负责人选取列表的数据 */

public:
    explicit SenderPageHandler(QObject *parent = nullptr);
    ~SenderPageHandler();

    //以下三个函数，用于将C++中的数据暴露给QML
    QVariant thisModel() const;
    QVariant otherModel() const;
    QVariant regulatorListModel() const;

    //从QML唤起的删除文章函数
    Q_INVOKABLE void itemMove(int idx);
    //从QML唤起的选择负责人函数
    Q_INVOKABLE void regulatorChosen(int idx);
    //从QML唤起的增加文章函数
    Q_INVOKABLE void addAnArticle(QString title, QString content);
    //从QML唤起的选择负责人函数
    Q_INVOKABLE void chooseRegulator(int index);
    //从QML唤起的增加文章函数
    Q_INVOKABLE void editAnArticle(int index, QString title, QString content);
    //刷新页面数据
    Q_INVOKABLE void refreshPage();

private:

    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine *thisEngine;
    //数据库请求对象指针
    QSqlQuery *query;

    //存储ListView所需model的数据结构
    QList<QObject*> myThisModel;
    QList<QObject*> myOtherModel;
    QList<QObject*> myRegulatorListModel;

    //当前发送者身份标识，默认为-1（空）
    int thisUserId=-1;
    //当前需要发送的文章身份标识，没有要发送的文章则为-1
    int articleSendingId=-1;
    //当前正在编辑/查看的文章
    int currentInViewIndex=0;

signals:
    //以下四项为各model向QML暴露的信号接口
    void thisModelChanged(const QVariant&);
    void otherModelChanged(const QVariant&);
    void regulatorListModelChanged(const QVariant&);
    void loadArticlesComplete(int article_id, QString title, QString content);

    //向QML发送刷新ListView界面信号
    void startRefreshQml();
    void refreshQmlComplete();

public slots:
    //开始渲染页面
    void startPage(QQmlApplicationEngine *engine);
    //开始加载数据（同步）
    void startLoadingData(int flag, int user_id);
};


#endif
