#ifndef MYARTICLEOBJ_H
#define MYARTICLEOBJ_H

#include <QObject>

class MyArticleObj : public QObject
{
    Q_OBJECT


public:
    MyArticleObj(QString newname, QString newcolor);

signals:

public slots:
};

#endif // MYARTICLEOBJ_H
