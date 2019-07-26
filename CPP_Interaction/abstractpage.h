#ifndef ABSTRACTPAGE_H
#define ABSTRACTPAGE_H

#include <QObject>
#include <QQmlApplicationEngine>

class AbstractPage : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPage(QObject *parent = nullptr);

signals:
    //错误信息信号，向QML发送，使其在界面上显示
    void sendErrorMessage(QString errStr);
    //成功信息信号，向QML发送，使其在界面上显示
    void sendSuccessMessage(QString successStr);

public slots:
    //开始渲染主页面
    virtual void startPage(QQmlApplicationEngine *engine)=0;
};

#endif // ABSTRACTPAGE_H
