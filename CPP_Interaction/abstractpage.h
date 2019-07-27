#ifndef ABSTRACTPAGE_H
#define ABSTRACTPAGE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include "../CPP_Storage/globalstoragecomponents.h"

class AbstractPage : public QObject
{
Q_OBJECT
public:
    explicit AbstractPage(int newUserId, QObject *parent = nullptr);
    virtual ~AbstractPage();
    Q_INVOKABLE void showUserInfo();
    Q_INVOKABLE void showArticleInfo(int articleId);

signals:
    //错误信息信号，向QML发送，使其在界面上显示
    void sendErrorMessage(QString errStr);
    //成功信息信号，向QML发送，使其在界面上显示
    void sendSuccessMessage(QString successStr);

public slots:
    //开始渲染主页面
    virtual void startPage(QQmlApplicationEngine *engine)=0;

protected:
    //从主函数传来的engine指针，用于启动其他页面，也可传向其他页面
    QQmlApplicationEngine* thisEngine;
    static GlobalStorageComponents* storage;
    //当前身份标识，默认为-1（空）
    int thisUserId;
};

#endif // ABSTRACTPAGE_H
