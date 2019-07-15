#include "callremotemysqlthread.h"
#include <QDebug>

CallRemoteMySQLThread::CallRemoteMySQLThread(QObject *parent) : QThread(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("39.106.107.241");
    db.setDatabaseName("cyxcpp");
    db.setUserName("root");
    db.setPassword("cyxserver-2019");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else qDebug() << "open";

    query=new QSqlQuery(db);
}

CallRemoteMySQLThread::~CallRemoteMySQLThread()
{
    qDebug() << "CallRemoteMySQLThread::~CallRemoteMySQLThread()";
}

void CallRemoteMySQLThread::run()
{
    while(1)
    {
        sleep(1);
        qDebug()<<"Thread Reporting";
        emit progress(100);
        emit message(QString("ThreadFromQThread::run times"));


        query->exec(QString("select user_id,password from users where user_name=\"sender1\";"));
        query->next();
        int userId = query->value(0).toInt();
        QString userPassword = query->value(1).toString();
        qDebug() << userId << userPassword;
    }
}


