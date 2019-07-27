#include "articleinfopagehandler.h"
#include <QDebug>


ArticleInfoPageHandler::ArticleInfoPageHandler(MyArticleObj* thisArticle, QObject *parent) :
    AbstractPage(-1), article(thisArticle)
{
    qDebug() << "load article info";
    int senderId = article->senderIdOfArticle();
    int regulatorId = article->regulatorIdOfArticle();
    int translatorId = article->translatorIdOfArticle();

    sender = storage->searchUserById(senderId);
    regulator = storage->searchUserById(regulatorId);
    translator = storage->searchUserById(translatorId);

}

void ArticleInfoPageHandler::startPage(QQmlApplicationEngine *engine)
{
    engine->rootContext()->setContextProperty("articleInfoPageHandler", this);
    engine->load(QUrl(QStringLiteral("qrc:/QML/OtherPages/ArticleInfoPage.qml")));
}


Q_INVOKABLE QString ArticleInfoPageHandler::getSenderName()
{
    if (sender != nullptr)
        return sender->username();
    else return QString("无信息");
}
Q_INVOKABLE QString ArticleInfoPageHandler::getRegulatorName()
{
    if (regulator != nullptr)
        return regulator->username();
    else return QString("无信息");
}
Q_INVOKABLE QString ArticleInfoPageHandler::getTranslatorName()
{
    if (translator != nullptr)
        return translator->username();
    else return QString("无信息");
}


Q_INVOKABLE int ArticleInfoPageHandler::getMoney()
{
    return article->fee();
}
