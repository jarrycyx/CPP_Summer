#include "loginpagehandler.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include <QtConcurrent/QtConcurrent>
#include "senderpagehandler.h"
#include "regulatorpagehandler.h"

#include <QSqlDatabase>

#include <QDebug>

#include <QSqlQuery>
#include <QThread>

/*************************************************************************
【类名】        LoginPageHandler
【功能】        登陆/注册界面的后台数据处理类，与Login.qml联动
【接口说明】     startPage函数从外部调用，表示开始渲染界面
【开发者及日期】  jarrycyx 20190713
【更改记录】     20190714: 修改为异步查询数据
*************************************************************************/

/* 登陆界面的后台处理类
 * 使用异步线程请求登录数据
 */

/*************************************************************************
【函数名称】  LoginPageHandler
【函数功能】  构造函数，同时创建数据库连接
【参数】    parent，可以为空
【开发者及日期】    jarrycyx 20190709
*************************************************************************/
LoginPageHandler::LoginPageHandler(GlobalComponents *newGlobalStorageComponent, QObject *parent) : QObject(parent), globalStorageComponent(newGlobalStorageComponent)
{
}

/*************************************************************************
【函数名称】  ~LoginPageHandler
【函数功能】  析构函数，同时释放内存
【开发者及日期】    jarrycyx 20190718
*************************************************************************/
LoginPageHandler::~LoginPageHandler()
{
    qDebug() << "登陆页面析构";
    //delete newSenderPage;
    //newSenderPage=NULL;
}

/*************************************************************************
【函数名称】  startPage
【函数功能】  开始渲染主页面
【参数】    QQmlApplicationEngine *engine
【返回值】   无
【开发者及日期】    jarrycyx 20190712
*************************************************************************/
void LoginPageHandler::startPage(QQmlApplicationEngine *engine)
{
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("loginPageHandler", this);
    thisEngine = engine;
    engine->load(QUrl(QStringLiteral("qrc:/Login.qml")));
}

/*************************************************************************
【函数名称】  loginInit，loginInNewThread
【函数功能】  QML从界面中唤起该函数，执行登录操作（loginInNewThread）
【参数】    从界面中读取的用户名和密码
【返回值】   无
【开发者及日期】    jarrycyx 20190714
*************************************************************************/
Q_INVOKABLE void LoginPageHandler::loginInit(QString name, QString pswd, int role)
{
    int loginResult = userLogin(name, pswd, role);
    switch (loginResult)
    {
    case 1:
    {
        emit sendSuccessMessage("登陆成功");

        if (role == 1)
        {
            newSenderPage = new SenderPageHandler(searchUser(name, role), globalStorageComponent);
            newSenderPage->startPage(thisEngine);
        }
        else if (role == 2)
        {
            newRegulatorPage = new RegulatorPageHandler(searchUser(name, role), globalStorageComponent);
            newRegulatorPage->startPage(thisEngine);
        }else if (role == 3)
        {
            newTranslatorPage = new TranslatorPageHandler(searchUser(name, role), globalStorageComponent);
            newTranslatorPage->startPage(thisEngine);
        }
        break;
    }
    case 2:
    {
        emit sendErrorMessage("密码不匹配");
        break;
    }
    case 3:
    {
        emit sendErrorMessage("用户身份不匹配");
        break;
    }
    case 4:
    {
        emit sendErrorMessage("用户不存在");
        break;
    }
    default:
    {
        emit sendErrorMessage("登陆错误");
        break;
    }
    }
}

/*************************************************************************
【函数名称】  signUp，signUpInNewThread
【函数功能】  QML从界面中唤起该函数，创建新线程执行注册操作（signUpInNewThread）
【参数】    从界面中读取的用户名和密码
【返回值】   无
【开发者及日期】    jarrycyx 20190714
*************************************************************************/
Q_INVOKABLE void LoginPageHandler::signUp(QString name, QString pswd, int role)
{
    int signupResult = addUser(name, pswd, role);
    if (!signupResult)
        emit sendErrorMessage("用户已存在");
    else
        emit sendSuccessMessage("注册成功");
}

int LoginPageHandler::userLogin(QString name, QString pswd, int role)
{
    int len = globalStorageComponent->getUsersLength();
    int ifUnRegistered = true;
    int ifRoleNotExist = true;
    for (int i = 0; i < len; i++)
    {
        if (name == globalStorageComponent->getUserToEdit(i)->username())
        {
            ifUnRegistered = false;
            if (role == globalStorageComponent->getUserToEdit(i)->role())
            {
                ifRoleNotExist = false;
                if (pswd == globalStorageComponent->getUserToEdit(i)->password())
                    return 1;
                else
                    return 2; //密码不匹配
            }
        }
    }

    if (ifUnRegistered)
        return 4; //用户未注册
    if (ifRoleNotExist)
        return 3; //身份不匹配
    return 0;
}

int LoginPageHandler::searchUser(QString name, int role)
{
    int len = globalStorageComponent->getUsersLength();
    for (int i = 0; i < len; i++)
    {
        if (name == globalStorageComponent->getUserToEdit(i)->username())
            if (role == globalStorageComponent->getUserToEdit(i)->role())
                return globalStorageComponent->getUserToEdit(i)->userId();
    }
    return -1;
}

int LoginPageHandler::addUser(QString name, QString pswd, int role)
{
    int len = globalStorageComponent->getUsersLength();
    for (int i = 0; i < len; i++)
    {
        if (globalStorageComponent->getUserToEdit(i)->username() == name &&
                globalStorageComponent->getUserToEdit(i)->role() == role)
            return 0; //用户已存在
    }
    int newUserId = globalStorageComponent->getAUserId();
    qDebug() << "注册ID" << newUserId;
    MyUserObj *newUser = new MyUserObj(newUserId, name, pswd, role);
    newUser->setModifyStatus(1);
    globalStorageComponent->addAUser(newUser);
    return 1;
}
