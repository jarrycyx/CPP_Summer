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
#include "userinfopagehandler.h"
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
LoginPageHandler::LoginPageHandler(QObject *parent)
    : AbstractPage(-1)
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
    engine->load(QUrl(QStringLiteral("qrc:/QML/OtherPages/Login.qml")));
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
            newSenderPage = new SenderPageHandler(searchUser(name), storage);
            newSenderPage->startPage(thisEngine);
        }
        else if (role == 2)
        {
            newRegulatorPage = new RegulatorPageHandler(searchUser(name), storage);
            newRegulatorPage->startPage(thisEngine);
        }else if (role == 3)
        {
            newTranslatorPage = new TranslatorPageHandler(searchUser(name), storage);
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
Q_INVOKABLE void LoginPageHandler::signUp(QString name, QString pswd)
{
    QList<MyUserObj*> searchSubUserRes = storage->searchSubUsers(name);
    MyUserObj* newUser = addUser(name, pswd);
    if (newUser == nullptr)
        emit sendErrorMessage("用户已存在");
    else {
        emit sendSuccessMessage("注册成功");
        const QUrl url1(QStringLiteral("qrc:/QML/OtherPages/UserQualifyDialog.qml"));
        thisEngine->load(url1);
        newSignupUser = newUser;
    }
}


Q_INVOKABLE void LoginPageHandler::setUserQualification(QString quali){
    newSignupUser->setQualification(quali);
    if (quali == "TOFEL 90分")
        newSignupUser->setCredit(10);
    else if (quali == "TOFEL 100分")
        newSignupUser->setCredit(20);
    else if (quali == "TOFEL 110分")
        newSignupUser->setCredit(30);
    else if (quali == "英语六级")
        newSignupUser->setCredit(8);
    else if (quali == "英语专业八级")
        newSignupUser->setCredit(40);
}

int LoginPageHandler::userLogin(QString name, QString pswd, int role)
{
    int len = storage->getUsersLength();
    int ifUnRegistered = true;
    int ifRoleNotExist = true;
    for (int i = 0; i < len; i++)
    {
        if (name == storage->getUserToEdit(i)->username())
        {
            ifUnRegistered = false;
            ifRoleNotExist = false;
            if (pswd == storage->getUserToEdit(i)->password())
                return 1;
            else
                return 2; //密码不匹配
        }
    }

    if (ifUnRegistered)
        return 4; //用户未注册
    if (ifRoleNotExist)
        return 3; //身份不匹配
    return 0;
}

int LoginPageHandler::searchUser(QString name)
{
    int len = storage->getUsersLength();
    for (int i = 0; i < len; i++)
    {
        if (name == storage->getUserToEdit(i)->username())
            return storage->getUserToEdit(i)->userId();
    }
    return -1;
}

MyUserObj* LoginPageHandler::addUser(QString name, QString pswd)
{
    int len = storage->getUsersLength();
    for (int i = 0; i < len; i++)
    {
        if (storage->getUserToEdit(i)->username() == name)
            return nullptr; //用户已存在
    }
    int newUserId = storage->getAUserId();
    qDebug() << "注册ID" << newUserId;
    MyUserObj *newUser = new MyUserObj(newUserId, name, pswd);
    newUser->setModifyStatus(StorageUnit::New);
    storage->addAUser(newUser);
    return newUser;
}
