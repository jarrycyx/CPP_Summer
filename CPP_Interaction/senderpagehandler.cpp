#include "senderpagehandler.h"
#include "../CPP_Data/myarticleobj.h"
#include "../CPP_Data/myuserobj.h"
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include "../CPP_Storage/globalstoragecomponents.h"

/*************************************************************************
名称：    SenderPageHandler
功能：    构造函数
参数：    parent，可以为空
日期：    20190712
*************************************************************************/
SenderPageHandler::SenderPageHandler(int senderId)
    : AbstractPage(senderId), mSenderArticleList(1), allUserArticleList(2)
{
    startLoadingSenderArticleList(senderId);
}

/*************************************************************************
名称：     ~SenderPageHandler
功能：     析构函数，删除动态分配的内存
日期：     jarrycyx 20190718
*************************************************************************/
SenderPageHandler::~SenderPageHandler()
{
}


/*************************************************************************
名称：     startLoadingSenderArticleList
功能：     加载所需显示的数据，根据需求将Data组合为Model
参数：     发布者用户Id
返回：     无
日期：     20190712 初步实现
          20190715 增加查看其他人的文章功能
*************************************************************************/
void SenderPageHandler::startLoadingSenderArticleList(int userId)
{
    mSenderArticleList.removeAllArticles();
    allUserArticleList.removeAllArticles();

    qDebug() << "sender" << userId;
    int len = storage->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        if (storage->getArticleToEdit(i)->statusCodeOfArticle() != 400){
            qDebug() << "sender article";
            if (storage->getArticleToEdit(i)->senderIdOfArticle() == userId
                    && storage->getArticleToEdit(i)->statusCodeOfArticle() / 100 != 2)
                mSenderArticleList.addAnArticle(storage->getArticleToEdit(i));
            if (storage->getArticleToEdit(i)->statusCodeOfArticle() / 100 != 2)
                allUserArticleList.addAnArticle(storage->getArticleToEdit(i));
        }
    }
}


/*************************************************************************
名称：     addSenderArticle
功能：     发布一篇文章
参数：     从QML读取的描述（标题）和内容，以及设定的酬金
返回：     无
日期：     20190709 初步实现发布文章
          20190718 增加选择酬金功能
*************************************************************************/
void SenderPageHandler::addSenderArticle(QString title, QString content, int money)
{
    MyArticleObj *newSenderArticle = new MyArticleObj(mThisUserId);
    newSenderArticle->setArticleInfo(storage->getAnArticleId(), title, content);
    newSenderArticle->setStatusCodeOfArticle(100);
    newSenderArticle->setFee(money);

    storage->sendMessageToRelatedUser(
                QString("%1").arg(storage->decodeStatusCode(100)),
                newSenderArticle);

    newSenderArticle->setModifyStatus(StorageUnit::New);
    //由于需要显示两处，保存一处，故需要增加三处
    //allArticles.push_front(newSenderArticle);
    storage->addAnArticle(newSenderArticle);
    mSenderArticleList.addAnArticle(newSenderArticle);
    allUserArticleList.addAnArticle(newSenderArticle);
    emit sendSuccessMessage(QString("文章已上传，酬金为%1元").arg(money));
}


/*************************************************************************
名称：     editAnArticle
功能：     修改文章
参数：     文章标题和内容
返回：     无
日期：     20190712
*************************************************************************/
void SenderPageHandler::editSenderArticle(int index, QString title, QString content)
{
    //由于数据实体只保存一份，只需编辑一处即可
    mSenderArticleList.editAnArticle(index, title, content);

    emit sendSuccessMessage("已保存");
}


/*************************************************************************
名称：     deleteSenderArticle
功能：     删除文章，执行MyArticleObj的删除操作
参数：     idx，该文章在model中的位置
返回：     无
日期：     20190710 实现拖动删除
          20190719 更改函数名
*************************************************************************/
void SenderPageHandler::deleteSenderArticle(int index)
{
    mSenderArticleList.deleteAnArticle(index);
    emit sendSuccessMessage("已删除");
}

/*************************************************************************
名称：     startPage
功能：     开始渲染主页面
参数：     QQmlApplicationEngine *engine
返回：     无
日期：     20190712
*************************************************************************/
void SenderPageHandler::startPage(QQmlApplicationEngine *engine)
{
    mThisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("senderPageHandler", this);
    thisContext->setContextProperty("senderArticleList", &mSenderArticleList);
    thisContext->setContextProperty("allUserArticleList", &allUserArticleList);
    thisContext->setContextProperty("userListModel", &requestUserList);
    const QUrl url1(QStringLiteral("qrc:/QML/MainPages/SenderPage.qml"));
    engine->load(url1);
}

/*************************************************************************
名称：     chooseRegulator
功能：     启动添加/修改文章负责人页面
参数：     文章在列表中的位置index
返回：     无
日期：     20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::chooseRegulator(int index)
{
    //加载报名的负责人列表
    loadArticleRegulatorData(mSenderArticleList.getArticle(index)->articleIdOfArticle());
    //暂存正在浏览的文章index
    currentInViewIndex = index;
    const QUrl url(QStringLiteral("qrc:/QML/OtherPages/ChooseUserMiniPage.qml"));
    mThisEngine->load(url);
}


/*************************************************************************
名称：     loadArticleRegulatorData
功能：     根据选中的文章，加载报名的负责人列表
参数：     选中的文章Id
返回：     无
日期：     20190717
*************************************************************************/
void SenderPageHandler::loadArticleRegulatorData(int articleId)
{
    qDebug() << "choose" << articleId;
    requestUserList.removeAllRequestUsers();
    int numOfRequest = storage->getRequestsLength();
    for (int i = 0; i < numOfRequest; i++)
    {
        //轮询查找
        MyRequestObj *getRequest = storage->getRequest(i);
        if (getRequest->getArticleId() == articleId
                && getRequest->getType() == 1)//Type为1代表是申请做负责人的请求
        {
            MyUserObj *requestUser = storage->searchUserById(getRequest->getUserId());
            requestUserList.addARequestUser(requestUser);
        }
    }
}

/*************************************************************************
名称：     regulatorChosen
功能：     选中负责人后，交给MyArticleObj存储并同步负责人信息
参数：     idx，选中的负责人在model中的位置
返回：     无
日期：     20190717
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::regulatorChosen(int idx)
{
    MyArticleObj *articleToChoose = mSenderArticleList.getArticle(currentInViewIndex);
    articleToChoose->setRegulatorIdOfArticle(requestUserList.getRequestUser(idx)->userId());
    articleToChoose->setStatusCodeOfArticle(110);

    storage->sendMessageToRelatedUser(
                QString("%1").arg(storage->decodeStatusCode(110)),
                articleToChoose);

    mSenderArticleList.editAnArticle(currentInViewIndex);
    //向QML发送信号，显示成功提示
    emit sendSuccessMessage("已确定负责人");
}


/*************************************************************************
名称：     confirmAcceptArticle
功能：     发送者拒绝接收译文，修改状态
参数：     文章在列表中的index
返回：     无
日期：     20190725
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::reTranslate(int index)
{
    MyArticleObj* thisArticle = mSenderArticleList.getArticle(index);
    thisArticle->setStatusCodeOfArticle(130);
    mSenderArticleList.editAnArticle(index);
    emit sendSuccessMessage("扣款成功，感谢！");
}

/*************************************************************************
名称：     confirmAcceptArticle
功能：     发送者确认翻译任务完成，修改状态，分配酬金
参数：     文章在列表中的index
返回：     无
日期：     20190725
*************************************************************************/
Q_INVOKABLE void SenderPageHandler::confirmAcceptArticle(int index)
{
    MyArticleObj* thisArticle = mSenderArticleList.getArticle(index);
    int moneyToRegulator = int(thisArticle->fee()*0.2);
    int regulatorId = thisArticle->regulatorIdOfArticle();
    int senderId = thisArticle->senderIdOfArticle();
    storage->searchUserById(regulatorId)->addMoney(moneyToRegulator);
                                                                        //向发送者、负责人发送金额改变通知
    storage->sendUserModifiedMessage(regulatorId, QString("您的账户余额已改变，请注意"));
    int totalMoney = thisArticle->fee();                                //应扣款总数
    int len = storage->getArticlesLength();                             //搜索对应的子文章
    for (int i=0;i<len;i++){
        MyArticleObj* selectedArticle = storage->getArticleToEdit(i);
        if (selectedArticle->originArticleIdOfArticle() == thisArticle->articleIdOfArticle())
        {
            int translatorId = selectedArticle->translatorIdOfArticle();
            storage->searchUserById(translatorId)
                    ->addMoney(selectedArticle->fee());
            selectedArticle->setStatusCodeOfArticle(400);
                                                                        //向翻译者发送金额改变通知
            storage->sendUserModifiedMessage(translatorId, QString("您的账户余额已改变，请注意"));
        }
    }
    storage->searchUserById(senderId)->addMoney(-totalMoney);
                                                                        //向发送者、负责人发送金额改变通知
    storage->sendUserModifiedMessage(senderId, QString("您的账户余额已改变，请注意"));
    mSenderArticleList.getArticle(index)->setStatusCodeOfArticle(400);
    emit sendSuccessMessage("扣款成功，感谢！");
}
