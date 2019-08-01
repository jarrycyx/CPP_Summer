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
#include "supervisorpagehandler.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QThread>

/*************************************************************************
名称：     LoginPageHandler
功能：     构造函数
参数：     无
日期：     20190707
*************************************************************************/
LoginPageHandler::LoginPageHandler()
    : AbstractPage(-1)
{
}

/*************************************************************************
名称：     ~LoginPageHandler
功能：     析构函数，删除动态分配的内存
日期：     20190718
*************************************************************************/
LoginPageHandler::~LoginPageHandler()
{
    qDebug() << "登陆页面析构";
    //delete mSenderPage;
    //mSenderPage=NULL;
}

/*************************************************************************
名称：     startPage
功能：     开始渲染主页面
参数：     QQmlApplicationEngine *engine
返回：     无
日期：     jarrycyx 20190712
*************************************************************************/
void LoginPageHandler::startPage(QQmlApplicationEngine *engine)
{
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("loginPageHandler", this);
    mThisEngine = engine;
    engine->load(QUrl(QStringLiteral("qrc:/QML/OtherPages/Login.qml")));
}

/*************************************************************************
名称：     loginInit，loginInNewThread
功能：     QML从界面中唤起该函数，执行登录操作（loginInNewThread）
参数：     从界面中读取的用户名和密码
返回：     无
日期：     20190714 增加翻译者
          20190726 修改用户多种身份登陆方式
*************************************************************************/
Q_INVOKABLE void LoginPageHandler::loginInit(QString name, QString pswd, int role)
{
    int loginResult = userLogin(name, pswd, role);
    switch (loginResult)
    {
    case 1: //登陆成功
    {
        emit sendSuccessMessage("登陆成功");
        if (role == 1)
        {
            //翻译者，则加载翻译者界面
            mSenderPage = new SenderPageHandler(searchUser(name));
            mSenderPage->startPage(mThisEngine);
        }
        else if (role == 2)
        {
            mRegulatorPage = new RegulatorPageHandler(searchUser(name));
            mRegulatorPage->startPage(mThisEngine);
        }
        else if (role == 3)
        {
            mTranslatorPage = new TranslatorPageHandler(searchUser(name));
            mTranslatorPage->startPage(mThisEngine);
        }
        else if (role == 4)
        {
            mSupervisorPage = new SupervisorPageHandler(searchUser(name));
            mSupervisorPage->startPage(mThisEngine);
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
名称：     signUp，signUpInNewThread
功能：     QML从界面中唤起该函数，执行注册操作
参数：     从界面中读取的用户名和密码
返回：     无
日期：     20190714 实现该函数
          20190715 实现多线程增加流畅度
          20190716 取消多线程以匹配大作业要求
*************************************************************************/
Q_INVOKABLE void LoginPageHandler::signUp(QString name, QString pswd)
{
    MyUserObj *newUser = addUser(name, pswd);
    if (newUser == nullptr)
        emit sendErrorMessage("用户已存在");
    else
    {
        emit sendSuccessMessage("注册成功");
        const QUrl url1(QStringLiteral("qrc:/QML/OtherPages/UserQualifyDialog.qml"));
        mThisEngine->load(url1);
        mSignupUser = newUser;
    }
}

Q_INVOKABLE void LoginPageHandler::setUserQualification(QString quali)
{
    mSignupUser->setQualification(quali);
    if (quali == "TOFEL 90分")
        mSignupUser->setCredit(10);
    else if (quali == "TOFEL 100分")
        mSignupUser->setCredit(20);
    else if (quali == "TOFEL 110分")
        mSignupUser->setCredit(30);
    else if (quali == "英语六级")
        mSignupUser->setCredit(8);
    else if (quali == "英语专业八级")
        mSignupUser->setCredit(40);
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

/*************************************************************************
名称：     searchUser
功能：     通过用户名查找用户，用于检查用户名是否可以注册或查找用户的ID
参数：     用户名
返回：     返回用户id，如果没找到则返回-1
日期：     *
*************************************************************************/
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

/*************************************************************************
名称：     addUser
功能：     注册函数，判断是否可以注册
参数：     用户名与密码
返回：     用户对象指针，未找到则是空指针
日期：     20190710
*************************************************************************/
MyUserObj *LoginPageHandler::addUser(QString name, QString pswd)
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
    //标记用户对象存储状态为新增，结束时上传
    newUser->setModifyStatus(StorageUnit::New);
    storage->addAUser(newUser);
    return newUser;
}
