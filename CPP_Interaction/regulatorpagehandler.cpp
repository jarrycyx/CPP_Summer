#include "regulatorpagehandler.h"
#include "../CPP_Data/myarticleobj.h"
#include "../CPP_Data/myuserobj.h"
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include "../CPP_Storage/globalstoragecomponents.h"
#include "userinfopagehandler.h"

/*************************************************************************
名称：     RegulatorPageHandler
功能：     构造函数
参数：     用户id
日期：     20190712
*************************************************************************/
RegulatorPageHandler::RegulatorPageHandler(int regulatorId)
    : AbstractPage(regulatorId), regulatorSubarticleList(1),
      regulatorArticleList(1), allSeekingRegulatorArticle(2)
{
    startLoadingRegulatorArticleList(regulatorId);
}

/*************************************************************************
名称：     ~RegulatorPageHandler
功能：     析构函数，删除动态分配的内存
日期：     20190718
*************************************************************************/
RegulatorPageHandler::~RegulatorPageHandler()
{
}

/*************************************************************************
名称：     startPage
功能：     开始渲染主页面
参数：     QQmlApplicationEngine *engine
返回：     无
日期：     20190712 初步实现
          20190720 多次调整名称
*************************************************************************/
void RegulatorPageHandler::startPage(QQmlApplicationEngine *engine)
{
    mThisEngine = engine;
    QQmlContext *thisContext = engine->rootContext();
    thisContext->setContextProperty("regulatorPageHandler", this);
    thisContext->setContextProperty("regulatorArticleList", &regulatorArticleList);
    thisContext->setContextProperty("regulatorSubarticleList", &regulatorSubarticleList);
    thisContext->setContextProperty("allSeekingRegulatorArticle", &allSeekingRegulatorArticle);
    thisContext->setContextProperty("userListModel", &translatorList);
    const QUrl url1(QStringLiteral("qrc:/QML/MainPages/RegulatorPage.qml"));
    engine->load(url1);
}

/*************************************************************************
名称：     startLoadingRegulatorArticleList
功能：     加载负责人页面需要显示的文章列表，根据需求将Data组合为Model
参数：     用户Id
返回：     无
日期：     20190716 初步实现
          20190718 增加子任务模块
*************************************************************************/
void RegulatorPageHandler::startLoadingRegulatorArticleList(int userId)
{
    qDebug() << "Regulator" << userId;
    //轮询查找
    int len = storage->getArticlesLength();
    for (int i = 0; i < len; i++)
    {
        MyArticleObj *selectedArticle = storage->getArticleToEdit(i);
        if (selectedArticle->regulatorIdOfArticle() == userId
                && selectedArticle->statusCodeOfArticle() != 400)
        //不显示完成的任务
        { //显示子任务的列表
            if (selectedArticle->statusCodeOfArticle() / 100 == 2)
            {
                //不显示已被审核通过的子任务
                if (selectedArticle->statusCodeOfArticle() != 240)
                    regulatorSubarticleList.addAnArticle(selectedArticle);
            }
            else
                regulatorArticleList.addAnArticle(selectedArticle);
        }
        if (selectedArticle->statusCodeOfArticle() == 100) //正在招募负责人的任务
            allSeekingRegulatorArticle.addAnArticle(selectedArticle);
    }
}

/*************************************************************************
名称：     splitRegulatorArticle
功能：     拆分任务，并新建子任务
备注：     空行处自动拆分
参数：     任务在列表中的index，简介和内容
返回：     无
日期：     20190720 初步实现
          20190724 增加分配酬金功能
*************************************************************************/
void RegulatorPageHandler::splitRegulatorArticle(int index, QString title, QString content)
{
    editArticle(index, title, content); //先保存更改
    qDebug() << "split" << index;
    MyArticleObj *articleToSplit = regulatorArticleList.getArticle(index);

    articleToSplit->setStatusCodeOfArticle(140);
    storage->sendMessageToRelatedUser( //发送通知给相关用户
        QString("%1").arg(storage->decodeStatusCode(140)),
        articleToSplit);

    articleToSplit->setFee(articleToSplit->fee()); //20%金额分配给负责人

    regulatorArticleList.editAnArticle(index);       //刷新文章状态
    QStringList subContents = content.split("\n\n"); //在空行处拆分文章
    qDebug() << subContents << index;
    for (int i = 0; i < subContents.length(); i++)
        if (subContents[i].length() == 0)
            subContents.removeAt(i); //文章内容为空，说明是空的一行
    int numOfSubarticles = subContents.length();
    for (int i = 0; i < numOfSubarticles; i++)
    {
        QString subTitle = title + " 子任务" + QString("%1").arg(i);
        MyArticleObj *newSubArticle = new MyArticleObj(-1);
        newSubArticle->setArticleInfo(storage->getAnArticleId(), subTitle, subContents[i]);
        newSubArticle->setTranslatedTitle(subTitle); //译文内容先设置为与原文相同，由翻译者进行替换
        newSubArticle->setTranslatedContent(subContents[i]);
        newSubArticle->setStatusCodeOfArticle(200);
        storage->sendMessageToRelatedUser( //发送通知给相关用户
            QString("%1").arg(storage->decodeStatusCode(200)),
            newSubArticle);
        newSubArticle->setRegulatorIdOfArticle(mThisUserId);
        newSubArticle->setOriginArticleIdOfArticle(articleToSplit->articleIdOfArticle());
        float remainFee = float((articleToSplit->fee()) * 0.8);
        //80%款项平均分给翻译者
        newSubArticle->setFee(int(remainFee / numOfSubarticles));
        newSubArticle->setModifyStatus(StorageUnit::New);
        //标记为新增的存储对象
        storage->addAnArticle(newSubArticle);
        regulatorSubarticleList.addAnArticle(newSubArticle);
    }
    emit sendSuccessMessage("文章已拆分");
}

/*************************************************************************
名称：     mergeRegulatorArticle
功能：     检查子任务是否都已审核通过，若是则合并子任务
参数：     子任务在列表中的index
返回：     无
日期：     20190722
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::mergeRegulatorArticle(int index)
{
    int originArticleId = regulatorSubarticleList.getArticle(index)
            ->originArticleIdOfArticle();
    //获得原文章ID
    QString subTitle = regulatorSubarticleList.getArticle(index)->translatedTitle();
    //通过子文章翻译标题得到原文章翻译标题
    int len = storage->getArticlesLength();
    for (int i = 0; i < len; i++)
    { //寻找原文章的其他子文章
        if (storage->getArticleToEdit(i)
                    ->originArticleIdOfArticle() == originArticleId &&
            storage->getArticleToEdit(i)->statusCodeOfArticle() != 240)
        //排除已经标记为完成的文章
        { //判断其他子文章是否已经审核通过
            if (storage->getArticleToEdit(i)->statusCodeOfArticle() != 230)
            {
                emit sendErrorMessage("其他子文章未审核通过");
                return;
            }
        }
    }

    QList<QString> listOfSubContent; //先用一个list暂存子文章后，再进行排序
    for (int i = 0; i < len; i++)
    { //寻找原文章的其他子文章
        if (storage->getArticleToEdit(i)
                    ->originArticleIdOfArticle() == originArticleId &&
            storage->getArticleToEdit(i)->statusCodeOfArticle() != 240)
        { //判断其他子文章是否已经审核通过
            MyArticleObj *thisSubArticle = storage->getArticleToEdit(i);
            QString subTransContent = thisSubArticle->translatedContent();
            QString subTitle = thisSubArticle->titleOfArticle();
            thisSubArticle->setStatusCodeOfArticle(240);

            storage->sendMessageToRelatedUser(
                QString("%1").arg(storage->decodeStatusCode(230)),
                thisSubArticle);
            qDebug() << subTransContent << subTitle << subTitle[subTitle.length() - 1];
            QChar indexChar = subTitle[subTitle.length() - 1];
            //子文章标题最后的序号
            if (!indexChar.isNumber())
            {
                emit sendErrorMessage("标题序号错误");
                return;
            }
            listOfSubContent.insert(indexChar.unicode() - '0', subTransContent);
        }
    }
    emit sendSuccessMessage("合并成功");
    QString transContent;
    int lenOfList = listOfSubContent.length();
    for (int i = 0; i < lenOfList; i++)
        transContent += listOfSubContent[i]; //合并译文
    MyArticleObj *originAriticle = storage->searchArticleById(originArticleId);
    //查找拆分前的文章
    originAriticle->setTranslatedTitle(subTitle); //修改文章状态，增加译文
    originAriticle->setTranslatedContent(transContent);
    originAriticle->setStatusCodeOfArticle(300);
    storage->sendMessageToRelatedUser( //发送通知给相关用户
        QString("%1").arg(storage->decodeStatusCode(300)),
        originAriticle);
    regulatorArticleList.refreshAll(); //刷新视图
    regulatorSubarticleList.refreshAll();
}

/*************************************************************************
名称：     editArticle
功能：     负责人对文章内容微调
参数：     index以及文章内容
返回：     无
日期：     20190718
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::editArticle(int index, QString title, QString content)
{
    qDebug() << "save" << index;
    regulatorArticleList.editAnArticle(index, title, content);

    emit sendSuccessMessage("已保存");
}

/*************************************************************************
名称：     chooseTranslator
功能：     启动选择翻译者的弹窗
参数：     在列表中的位置index
返回：     无
日期：     20190718
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::chooseTranslator(int index)
{
    loadArticleTranslatorData(regulatorSubarticleList.getArticle(index)
                              ->originArticleIdOfArticle());
    currentInViewIndex = index;
    const QUrl url(QStringLiteral("qrc:/QML/OtherPages/ChooseUserMiniPage.qml"));
    mThisEngine->load(url);
}

/*************************************************************************
名称：     chooseTranslator
功能：     启动查看翻译者的弹窗
参数：     在列表中的位置index
返回：     无
日期：     20190720
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::viewTranslator(int index)
{
    //获取原文章id后，查找报名的译者
    loadArticleTranslatorData(regulatorArticleList.getArticle(index)->articleIdOfArticle());
    currentInViewIndex = -1;
    const QUrl url(QStringLiteral("qrc:/QML/OtherPages/ChooseUserMiniPage.qml"));
    mThisEngine->load(url);
}

/*************************************************************************
名称：     chooseTranslator
功能：     获取原文章id后，查找报名的译者
参数：     原文章id
返回：     无
日期：     20190720
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::loadArticleTranslatorData(int originArticleId)
{
    qDebug() << "choose origin" << originArticleId;

    translatorList.removeAllRequestUsers(); //清空列表
    int numOfRequest = storage->getRequestsLength();
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = storage->getRequest(i);
        if (getRequest->getArticleId() == originArticleId
                && getRequest->getType() == 2) //2代表报名为翻译者的请求
        {
            MyUserObj *requestUser = storage->searchUserById(getRequest->getUserId());
            translatorList.addARequestUser(requestUser); //在列表model中加入一项
        }
    }
}

/*************************************************************************
名称：     translatorChosen
功能：     查看翻译者的弹窗中，选择翻译者后的“回调”函数
参数：     在翻译者列表中的位置index
返回：     无
日期：     20190720
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::translatorChosen(int idx)
{
    if (currentInViewIndex != -1)
    { //-1代表未拆封状态，查看报名的译者，不能选择
        MyArticleObj *articleToChoose = regulatorSubarticleList
                .getArticle(currentInViewIndex);
        articleToChoose->setTranslatorIdOfArticle(
                    translatorList.getRequestUser(idx)->userId());
        qDebug() << translatorList.getRequestUser(idx)->userId() << "choosen";
        articleToChoose->setStatusCodeOfArticle(210); //已选择后改变状态
        storage->sendMessageToRelatedUser(            //发送改变状态的信息
            QString("%1").arg(storage->decodeStatusCode(210)),
            articleToChoose);
        regulatorSubarticleList.editAnArticle(currentInViewIndex);
        //刷新视图
        emit sendSuccessMessage("已确定译者");
    }
    else
        emit sendErrorMessage("请拆分后再选择翻译者");
}

/*************************************************************************
名称：     signForRegulatorArticle
功能：     报名做负责人
参数：     在列表中的位置index
返回：     无
日期：     20190720
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::signForRegulatorArticle(int index)
{
    int thisUserCredit = storage->searchUserById(mThisUserId)->credit();
    if (thisUserCredit >= 45)
    {                              //45积分以上可以报名做负责人
        int alreadySigned = false; //检查是否已报名
        int len = storage->getRequestsLength();
        for (int i = 0; i < len; i++)
        {
            MyRequestObj *selected = storage->getRequest(i);
            if (selected->getUserId() == mThisUserId //该文章的所有“报名为负责人”的请求中
                && selected->getType() == 1          //有无与当前用户ID相同的项
                && selected->getArticleId() ==
                       allSeekingRegulatorArticle.getArticle(index)
                    ->articleIdOfArticle())
            {
                alreadySigned = true;
            }
        }

        if (!alreadySigned)
        {
            qDebug() << "sign up for" << index;
            MyRequestObj *sendNewRequest = new MyRequestObj(
                storage->getARequestId(),
                mThisUserId,
                allSeekingRegulatorArticle.getArticle(index)->articleIdOfArticle(),
                1); //1表示成为负责人的请求
            sendNewRequest->setModifyStatus(StorageUnit::New);
            storage->addARequest(sendNewRequest);
            emit sendSuccessMessage("报名成功");
        }
        else
        {
            emit sendErrorMessage("已经报名");
        }
    }
    else
    {
        emit sendErrorMessage("抱歉，积分45以上才能报名成为负责人");
    }
}

/*************************************************************************
名称：     startRecruitingTranslatorForArticle
功能：     开始招募翻译者
参数：     在列表中的位置index
返回：     无
日期：     20190721
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::startRecruitingTranslatorForArticle(int index)
{
    qDebug() << "chooseTranslatorForArticle" << index;
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(120); //修改状态
    storage->sendMessageToRelatedUser(
        QString("%1").arg(storage->decodeStatusCode(120)),
        regulatorArticleList.getArticle(index));
    regulatorArticleList.editAnArticle(index); //更新视图
    emit sendSuccessMessage("开始招募");
}

/*************************************************************************
名称：     stopRecruitingTranslatorForArticle
功能：     停止招募翻译者
参数：     在列表中的位置index
返回：     无
日期：     20190721
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::stopRecruitingTranslatorForArticle(int index)
{
    qDebug() << "stopRecruitingTranslatorForArticle" << index;
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(130);
    storage->sendMessageToRelatedUser(
        QString("%1").arg(storage->decodeStatusCode(130)),
        regulatorArticleList.getArticle(index));
    regulatorArticleList.editAnArticle(index); //刷新文章状态
    emit sendSuccessMessage("停止招募");
}

/*************************************************************************
名称：     commentToTranslator
功能：     向翻译者发送反馈信息
参数：     在列表中的位置idx，反馈内容comment
返回：     无
日期：     20190722
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::commentToTranslator(int idx, QString comment)
{
    qDebug() << idx << " " << comment;
    MyRequestObj *newRequest = new MyRequestObj(storage->getARequestId(),
                                                mThisUserId,
                                                regulatorSubarticleList.getArticle(idx)->articleIdOfArticle(),
                                                3);
    regulatorSubarticleList.getArticle(idx)->setStatusCodeOfArticle(220);
    storage->sendMessageToRelatedUser( //发送状态更新消息
        QString("%1").arg(storage->decodeStatusCode(220)),
        regulatorSubarticleList.getArticle(idx));
    regulatorSubarticleList.editAnArticle(idx);
    newRequest->setContent(comment);
    newRequest->setModifyStatus(StorageUnit::New);
    storage->addARequest(newRequest);

    emit sendSuccessMessage("反馈成功");
}

/*************************************************************************
名称：     getHistoryComment
功能：     查看历史上发送的反馈信息
参数：     在列表中的位置index
返回：     多条反馈信息组合成的QString
日期：     20190721
*************************************************************************/
Q_INVOKABLE QString RegulatorPageHandler::getHistoryComment(int idx)
{
    qDebug() << "load comment" << idx;

    int numOfRequest = storage->getRequestsLength();
    QString commentStr = "";
    for (int i = 0; i < numOfRequest; i++)
    {
        MyRequestObj *getRequest = storage->getRequest(i);
        if (getRequest->getArticleId() == regulatorSubarticleList.getArticle(idx)
                ->articleIdOfArticle()
                && getRequest->getType() == 3)
        {
            commentStr += getRequest->getContent();
        }
    }
    return commentStr;
}

/*************************************************************************
名称：     acceptSubarticle
功能：     标记子任务为审核通过
参数：     在列表中的位置idx
返回：     无
日期：     20190721 初步实现
          20190727 添加积分功能
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::acceptSubarticle(int idx)
{
    regulatorSubarticleList.getArticle(idx)->setStatusCodeOfArticle(230);
    storage->sendMessageToRelatedUser(
        QString("%1").arg(storage->decodeStatusCode(230)),
        regulatorSubarticleList.getArticle(idx));
    regulatorSubarticleList.editAnArticle(idx);
    int translatorId = regulatorSubarticleList.getArticle(idx)->translatorIdOfArticle();
    storage->searchUserById(translatorId)->addCredit(1);
    emit sendSuccessMessage("已审核通过，译者积分+1");
    storage->sendUserModifiedMessage(translatorId, QString("您的账户余额已改变，请注意"));
}

/*************************************************************************
名称：     submitToSender
功能：     将合并后的文章提交给发布者
参数：     在列表中的位置idx
返回：     无
日期：     20190722
*************************************************************************/
Q_INVOKABLE void RegulatorPageHandler::submitToSender(int index)
{
    regulatorArticleList.getArticle(index)->setStatusCodeOfArticle(310);
    storage->sendMessageToRelatedUser(
        QString("%1").arg(storage->decodeStatusCode(310)),
        regulatorArticleList.getArticle(index));
    regulatorArticleList.editAnArticle(index);
}
