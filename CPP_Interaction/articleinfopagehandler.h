#ifndef ARTICLEINFOPAGEHANDLER_H
#define ARTICLEINFOPAGEHANDLER_H

#include <QObject>
#include "abstractpage.h"

class ArticleInfoPageHandler: public AbstractPage
{
Q_OBJECT
public:
    explicit ArticleInfoPageHandler(MyArticleObj* thisArticle, QObject *parent = nullptr);
    Q_INVOKABLE QString getSenderName();
    Q_INVOKABLE QString getRegulatorName();
    Q_INVOKABLE QString getTranslatorName();
    Q_INVOKABLE int getMoney();

signals:

public slots:
    void startPage(QQmlApplicationEngine *engine);

private:
    MyArticleObj* article;
    MyUserObj* sender;
    MyUserObj* regulator;
    MyUserObj* translator;
};

#endif // ARTICLEINFOPAGEHANDLER_H
